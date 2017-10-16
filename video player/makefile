# gcc scaling_video.c -Ic:/libs/ffmpeg2/include -Lc:/libs/ffmpeg2/lib -lavutil -lswscale
all: video_player.exe

video_player.exe: video_player.o
	 gcc -o video_player.exe video_player.o -Lc:/libs/ffmpeg2/lib -Lc:/MinGW/lib -lswscale -lavdevice -lavformat -lavcodec -liconv -lavutil -lswresample
	 #gcc -o video_player.exe video_player.o -Lc:/libs/ffmpeg2/lib -lavdevice -lavformat -lavcodec -lavutil

video_player.o: video_player.c
	 gcc -c video_player.c -Ic:/libs/ffmpeg2/include

clean:
	 rm video_player.o video_player.exe
