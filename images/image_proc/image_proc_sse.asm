
global brightness_sse
global contrast_sse
global yuv2rgb_float_sse


section .data align=16

vec0: align 16
dd 0.0, 0.0, 0.0, 0.0

vec255: align 16
dd 255.0, 255.0, 255.0, 255.0

vecn128: align 16
dd -128.0, -128.0, -128.0, -128.0

vec_yuvconst1: align 16
dd 1.13983, 1.13983, 1.13983, 1.13983

vec_yuvconst2: align 16
dd -0.39466, -0.39466, -0.39466, -0.39466

vec_yuvconst3: align 16
dd -0.58060, -0.58060, -0.58060, -0.58060

vec_yuvconst4: align 16
dd 2.03211, 2.03211, 2.03211, 2.03211

blah: align 16
dd 0,0,0,0

vecy: align 16
dd 0, 0, 0, 0

vecu: align 16
dd 0, 0, 0, 0

vecv: align 16
dd 0, 0, 0, 0


section .bss

section .code
bits 32

; void brightness_sse(unsigned char *image, int len, int v)

brightness_sse:
  push ebp
  push edi
  mov ebp, esp
  mov edi, [ebp+12]   ; unsigned char *image
  mov ecx, [ebp+16]   ; int len
  mov eax, [ebp+20]   ; int v

  test eax, 0x80000000 ; check if v is negative
  jz bright_not_neg
  xor al, 255         ; make al abs(v)
  inc al

bright_not_neg:

  shr ecx, 4          ; count = count / 16

  mov ah, al          ; make xmm1 =  (v,v,v,v ,v,v,v,v, ,v,v,v,v, v,v,v,v)
  pinsrw xmm1, ax, 0
  pinsrw xmm1, ax, 1
  pinsrw xmm1, ax, 2
  pinsrw xmm1, ax, 3
  pinsrw xmm1, ax, 4
  pinsrw xmm1, ax, 5
  pinsrw xmm1, ax, 6
  pinsrw xmm1, ax, 7

  test eax, 0xff000000    ; if v was negative, make it darker by abs(v)
  jnz dark_loop

bright_loop:
  movdqa xmm0, [edi]     ; for every 16 byte chunks, add v to all 16 bytes
  paddusb xmm0, xmm1     ; paddusb adds each 16 bytes of xmm0 by v but
  movdqa [edi], xmm0     ; if the byte overflows (more than 255) set to 255

  add edi, 16            ; ptr=ptr+16
  loop bright_loop       ; while (count>0)
  jmp bright_exit

dark_loop:
  movdqa xmm0, [edi]     ; same as above but subtract v from each of the
  psubusb xmm0, xmm1     ; 16 bytes that make up xmm0.  if a byte will
  movdqa [edi], xmm0     ; become negative, set it to 0 (saturation)

  add edi, 16            ; ptr=ptr+16
  loop dark_loop         ; while (count>0)

bright_exit:

  pop edi
  pop ebp
  ret                    ; return


; void contrast_sse(unsigned char *image, int len, int v)

contrast_sse:
  push ebp
  mov ebp, esp

  pop ebp
  ret

; void yuv2rgb_float(unsigned char *yuv_buffer, unsigned char *rgb_buffer, int width, int height)

yuv2rgb_float_sse:
  push ebp
  mov ebp, esp

  push esi
  push edi
  push ebx

  mov esi, [ebp+8]     ; yuv_buffer
  mov edi, [ebp+12]    ; rgb_buffer
  mov eax, [ebp+16]    ; width
  mov ecx, [ebp+20]    ; height
  mul ecx
  mov ecx, eax         ; ecx = width*height
  mov ebx, ecx

  mov ebp, esi
  add ebp, ebx     ; ebp = esi + (height*width,ebx) = u part of buffer
  shr ebx, 1       ; ebx + ebp = v part of buffer

  movaps xmm4, [vec0]          ; xmm4 = vector(0.0,,,)
  movaps xmm5, [vec255]        ; xmm5 = vector(255.0,,,)

  movaps xmm6, [vec_yuvconst1] ; xmm6 = vector(1.13983,,,)
  movaps xmm7, [vec_yuvconst4] ; xmm7 = vector(2.03211,,,)

  shr ecx, 2       ; ecx = width*height/4

yuv_loop:
  mov eax, [esi]    ; place y0,y1,y2,y3 into a vector
  mov [vecy+12], al
  mov [vecy+8], ah
  shr eax, 16
  mov [vecy+4], al
  mov [vecy], ah

  mov ax, [ebp]    ; place u0,u0,u1,u1 into a vector
  mov [vecu], al
  mov [vecu+4], al
  mov [vecu+8], ah
  mov [vecu+12], ah

  mov ax, [ebp+ebx] ; place v0,v0,v1,v1 into a vector
  mov [vecv], al
  mov [vecv+4], al
  mov [vecv+8], ah
  mov [vecv+12], ah

  cvtdq2ps xmm0, [vecy]    ; xmm0 = (float)vecy
  cvtdq2ps xmm1, [vecu]    ; xmm1 = (float)vecu
  cvtdq2ps xmm2, [vecv]    ; xmm2 = (float)vecv

  addps xmm1, [vecn128]     ; vecu = vecu - vec128
  addps xmm2, [vecn128]     ; vecv = vecv - vec128

  movaps xmm3, xmm2        ; vecr = y + 1.13983*(v-128)
  mulps xmm3, xmm6         ; *1.3983
  addps xmm3, xmm0         ; +y
  maxps xmm3, xmm4         ; max(r,0)
  minps xmm3, xmm5         ; min(r,255)
  cvttps2dq xmm3, xmm3     ; float to int
  pextrw eax, xmm3, 6
  mov [edi], al
  pextrw eax, xmm3, 4
  mov [edi+3], al
  pextrw eax, xmm3, 2
  mov [edi+6], al
  pextrw eax, xmm3, 0
  mov [edi+9], al

  movaps xmm3, xmm1        ; vecb = y + 2.03211*(u-128)
  mulps xmm3, xmm7         ; *2.03211
  addps xmm3, xmm0         ; +y
  maxps xmm3, xmm4         ; max(b,0)
  minps xmm3, xmm5         ; min(b,255)
  cvttps2dq xmm3, xmm3     ; float to int
  pextrw eax, xmm3, 6
  mov [edi+2], al
  pextrw eax, xmm3, 4
  mov [edi+5], al
  pextrw eax, xmm3, 2
  mov [edi+8], al
  pextrw eax, xmm3, 0
  mov [edi+11], al

                           ; vecg = y + -0.39466*(u-128) -0.58060*(v-128)
  mulps xmm1, [vec_yuvconst2] ; -0.39466*(u-128)
  mulps xmm2, [vec_yuvconst3] ; -0.58060*(v-128)
  addps xmm0, xmm1         ; +y
  addps xmm0, xmm2         ; +y
  maxps xmm0, xmm4         ; max(g,0)
  minps xmm0, xmm5         ; min(g,255)
  cvttps2dq xmm3, xmm0     ; float to int
  pextrw eax, xmm3, 6
  mov [edi+1], al
  pextrw eax, xmm3, 4
  mov [edi+4], al
  pextrw eax, xmm3, 2
  mov [edi+7], al
  pextrw eax, xmm3, 0
  mov [edi+10], al

  add edi, 12
  add esi, 4
  add ebp, 2

  dec ecx
  jnz yuv_loop

getout:
  ; lea eax, [vecv]
  ; movups xmm0, [vec_yuvconst1+8]
  ; cvttps2dq xmm0, xmm0     ; float to int
  ; cvttps2dq xmm5, [vecn128]     ; float to int
  ; movaps [vec_yuvconst1],xmm5
  ; mov eax, [vec_yuvconst1+0]

  pop ebx
  pop edi
  pop esi

  pop ebp
  ret

; void yuv2rgb_int(unsigned char *yuv_buffer, unsigned char *rgb_buffer, int width, int height);


