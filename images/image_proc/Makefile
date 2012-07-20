
DEBUG = -DDEBUG
CC = gcc
FLAGS = $(DEBUG) -O2 -Wall -m32 -g -march=k8
#FLAGS = $(DEBUG) -O2 -Wall -m64 -g -march=k8
#FLAGS = $(DEBUG) -O2 -Wall -march=pentium4

#FLAGS = $(DEBUG) -O2 -Wall -g -mcpu=iwmmxt
#FLAGS = $(DEBUG) -O2 -Wall -g -mcpu=xscale
#PATH=/usr/bin:/storage/gumstix/gumstix-buildroot/build_arm_nofpu/staging_dir/bin
#CC=arm-linux-gcc


default:
	nasm -f elf image_proc_sse.asm
	$(CC) -c bmp.c $(FLAGS)
	$(CC) -c fileoutput.c $(FLAGS)
	$(CC) -c image_proc.c $(FLAGS)
	$(CC) -c parse_bmp.c $(FLAGS)
	$(CC) -o brighter brighter.c bmp.o fileoutput.o \
	         image_proc.o image_proc_sse.o parse_bmp.o $(FLAGS)
	$(CC) -o yuv2rgb yuv2rgb.c bmp.o fileoutput.o \
	         image_proc.o image_proc_sse.o $(FLAGS)

nonsse:
	$(CC) -c bmp.c $(FLAGS)
	$(CC) -c fileoutput.c $(FLAGS)
	$(CC) -c image_proc.c $(FLAGS)
	$(CC) -c parse_bmp.c $(FLAGS)
	$(CC) -o brighter brighter.c bmp.o fileoutput.o \
	         image_proc.o parse_bmp.o $(FLAGS)

xscale:
	$(CC) -c bmp.c $(FLAGS)
	$(CC) -c fileoutput.c $(FLAGS)
	$(CC) -c image_proc.c $(FLAGS)
	$(CC) -c image_proc_xscale_sse.c $(FLAGS)
	$(CC) -S image_proc_xscale_sse.c $(FLAGS)
	$(CC) -c parse_bmp.c $(FLAGS)
	$(CC) -o brighter brighter.c bmp.o fileoutput.o image_proc.o \
	         image_proc_xscale_sse.o parse_bmp.o $(FLAGS)

clean:
	@if [ -e bmp.o ]; then rm bmp.o; fi;
	@if [ -e fileoutput.o ]; then rm fileoutput.o; fi;
	@if [ -e image_proc.o ]; then rm image_proc.o; fi;
	@if [ -e image_proc_sse.o ]; then rm image_proc_sse.o; fi;
	@if [ -e image_proc_xscale_sse.o ]; then rm image_proc_xscale_sse.o; fi;
	@if [ -e parse_bmp.o ]; then rm parse_bmp.o; fi;
	@if [ -e brighter ]; then rm brighter; fi;
	@if [ -e brighter.exe ]; then rm brighter.exe; fi;
	@if [ -e yuv2rgb ]; then rm yuv2rgb; fi;
	@if [ -e yuv2rgb.exe ]; then rm yuv2rgb.exe; fi;
	@echo "Clean!"


