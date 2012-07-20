rem @echo off

set DEBUG= -DDEBUG
set CC=gcc
set FLAGS=%DEBUG% -O2 -Wall -m32 -g -march=k8

nasm -f elf image_proc_sse.asm
%CC% -c bmp.c %FLAGS%
%CC% -c fileoutput.c %FLAGS%
%CC% -c image_proc.c %FLAGS%
%CC% -c parse_bmp.c %FLAGS%
%CC% -c brighter.c %FLAGS%
%CC% -o brighter brighter.o bmp.o fileoutput.o image_proc.o image_proc_sse.o parse_bmp.o %FLAGS%
%CC% -o yuv2rgb yuv2rgb.c bmp.o fileoutput.o image_proc.o image_proc_sse.o %FLAGS%

pause
