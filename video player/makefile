# gcc scaling_video.c -Ic:/libs/ffmpeg2/include -Lc:/libs/ffmpeg2/lib -lavutil -lswscale
all: video_player

video_player: video_player.o
	 gcc -o video_player video_player.o -L/usr/local/lib -lswscale -lavformat -lavcodec -lavutil -lswresample -lm -lpthread -ldl
	 #gcc -o video_player.exe video_player.o -Lc:/libs/ffmpeg2/lib -lavdevice -lavformat -lavcodec -lavutil

video_player.o: video_player.c
	 emcc -c video_player.c -I/usr/local/include

clean:
	 rm video_player.o video_player

video_player_js: video_player.o
	 emcc -o video_player.html video_player.o -L/home/damien/.local/ffmpeg.js/lib -lswscale -lavformat -lavcodec -lavutil -lswresample -lm -lpthread -ldl
