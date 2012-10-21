@echo off

set APP=.\Debug\SampleContainer.exe

set INPUT=test.wav
if not "%~1"=="" set INPUT=%~1

if not exist "%INPUT%" (
  echo the input file "%INPUT%" doesn't exist.
  goto END
)

echo input file name: "%INPUT%"

%APP% "%INPUT%" pcmu8.wav 8
%APP% "%INPUT%" pcm16.wav 16
%APP% "%INPUT%" pcm24.wav 24
%APP% "%INPUT%" pcm32.wav 32

%APP% pcmu8.wav pcmu8_u8.wav 8
%APP% pcmu8.wav pcmu8_16.wav 16
%APP% pcmu8.wav pcmu8_24.wav 24
%APP% pcmu8.wav pcmu8_32.wav 32

%APP% pcm16.wav pcm16_u8.wav 8
%APP% pcm16.wav pcm16_16.wav 16
%APP% pcm16.wav pcm16_24.wav 24
%APP% pcm16.wav pcm16_32.wav 32

%APP% pcm24.wav pcm24_u8.wav 8
%APP% pcm24.wav pcm24_16.wav 16
%APP% pcm24.wav pcm24_24.wav 24
%APP% pcm24.wav pcm24_32.wav 32

%APP% pcm32.wav pcm32_u8.wav 8
%APP% pcm32.wav pcm32_16.wav 16
%APP% pcm32.wav pcm32_24.wav 24
%APP% pcm32.wav pcm32_32.wav 32

:END
pause
