# gcc scaling_video.c -Ic:/libs/ffmpeg2/include -Lc:/libs/ffmpeg2/lib -lavutil -lswscale
all: scaling_video.exe

scaling_video.exe: scaling_video.o
	 gcc -o scaling_video.exe scaling_video.o -Lc:/libs/ffmpeg2/lib -lavutil -lswscale

scaling_video.o: scaling_video.c
	 gcc -c scaling_video.c -Ic:/libs/ffmpeg2/include

clean:
	 rm scaling_video.o scaling_video.exe
