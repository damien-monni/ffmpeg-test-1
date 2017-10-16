#include <stdio.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int  y;

  // Open file
  sprintf(szFilename, "frame%d.ppm", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;

  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);

  // Write pixel data
  for(y=0; y<height; y++)
    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

  // Close file
  fclose(pFile);
}

int main(int argc, char *argv[]) {
  printf("hello");
  // Registers all available file formats and codecs with the library so they
  // will be used automatically when a file with the corresponding format/codec is opened.
  av_register_all();

  AVFormatContext *pFormatCtx = NULL;

  // Open video file
  if (avformat_open_input(&pFormatCtx, argv[1], NULL, NULL) != 0) {
    printf("Echec1");
    return -1; // Couldn't open file
  }

  // Retrieve stream information
  if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
    printf("Echec2");
    return -1; // Couldn't find stream information
  }

  // Dump information about file onto standard error
  av_dump_format(pFormatCtx, 0, argv[1], 0);

  int i;
  AVCodecContext *pCodecCtxOrig = NULL;
  AVCodecContext *pCodecCtx = NULL;

  // Find the first video stream
  int videoStream = -1;
  for (i = 0; i < pFormatCtx->nb_streams; i++) {
    if (pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
      videoStream=i;
      break;
    }
  }
  if (videoStream == -1) {
    printf("Echec3");
    return -1; // Didn't find a video stream
  }

  // Get a pointer to the codec context for the video stream
  pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;

  AVCodec *pCodec = NULL;

  // Find the decoder for the video stream
  pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
  if (pCodec == NULL) {
    fprintf(stderr, "Unsupported codec!\n");
    return -1; // Codec not found
  }
  // Copy context
  pCodecCtx = avcodec_alloc_context3(pCodec);
  if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
    fprintf(stderr, "Couldn't copy codec context");
    return -1; // Error copying codec context
  }
  // Open codec
  if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
    printf("Could not open codec");
    return -1; // Could not open codec
  }

  // Allocate video frame
  AVFrame *pFrame = NULL;
  pFrame=av_frame_alloc();

  // Allocate an AVFrame structure
  AVFrame *pFrameRGB = NULL;
  pFrameRGB=av_frame_alloc();
  if(pFrameRGB==NULL) {
    return -1;
  }

  uint8_t *buffer = NULL;
  int numBytes;
  // Determine required buffer size and allocate buffer
  numBytes = avpicture_get_size(3, pCodecCtx->width,
                              pCodecCtx->height);
  buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)pFrameRGB, buffer, 3,
                  pCodecCtx->width, pCodecCtx->height);

  struct SwsContext *sws_ctx = NULL;
  int frameFinished;
  AVPacket packet;
  // initialize SWS context for software scaling
  sws_ctx = sws_getContext(pCodecCtx->width,
      pCodecCtx->height,
      pCodecCtx->pix_fmt,
      pCodecCtx->width,
      pCodecCtx->height,
      3,
      SWS_BILINEAR,
      NULL,
      NULL,
      NULL
      );

  i=0;
  int y = 0;
  while(av_read_frame(pFormatCtx, &packet)>=0) {
    // Is this a packet from the video stream?
    if(packet.stream_index==videoStream) {
  	// Decode video frame
      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

      // Did we get a video frame?
      if(frameFinished) {
      // Convert the image from its native format to RGB
          sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
  		  pFrame->linesize, 0, pCodecCtx->height,
  		  pFrameRGB->data, pFrameRGB->linesize);

          // Save the frame to disk
          if (++i <= 500) {
            for(y = 0; y < pCodecCtx->height; y++) { // First line only
              if (y == 0) {
                // printf("%d: %d\n", i, *pFrame->data[0]+y*pFrame->linesize[0]);
                uint8_t r = *pFrameRGB->data[0];
                uint8_t g = *(pFrameRGB->data[0] + 1);
                uint8_t b = *(pFrameRGB->data[0] + 2);
                printf("%d: %u, %u, %u\n", i, r, g, b);
                int a [3] = {r, g, b};
                return a;
              }
            }
          }
      }
    }

    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
  }

  printf("Fini");
  return 0;
}
