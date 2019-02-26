
% MODEL MEM_MOD

LOCALS @@

EXTRN   _tsw_videocard  : BYTE
EXTRN   _tsw_vsize      : WORD
EXTRN   _tsw_hsize      : WORD
EXTRN   _tsw_videobase  : FAR PTR
EXTRN   _tsw_cursorstate : BYTE

;PUBLIC  _sound,_nosound,_soundclick
;PUBLIC  _tsw_cursoron
;PUBLIC  _tsw_cursoroff
PUBLIC  _tsw_gotoxy
;PUBLIC  _tsw_initcursor
PUBLIC  _tsw_getcursor
PUBLIC  _tsw_changeatt
PUBLIC  _tsw_check_videocard
PUBLIC  _tsw_fillrect
PUBLIC  _tsw_puttext
PUBLIC  _tsw_gettext
PUBLIC  _tsw_bgetch
PUBLIC  _tsw_maputc
PUBLIC  _tsw_mputc
PUBLIC  _tsw_whaton
PUBLIC  _tsw_mputs
PUBLIC  _tsw_maputs
PUBLIC  _tsw_scroll
PUBLIC  _tsw_printerstat
PUBLIC  _tsw_wait20ms
PUBLIC  _tsw_checkspeed
PUBLIC  _tsw_get_dvseg
PUBLIC  _tsw_checkdv
PUBLIC  _check_key
PUBLIC  _slide

HERCULES  EQU 1
EGA       EQU 2
CGAMDA    EQU 3

LOCALS

  DATASEG
scan1       DB 0
scan2       DB 0
speedvall   DW 0
speedvalh   DW 0
  ENDS

  CODESEG

MACRO IncBP
ENDM IncBP

MACRO DecBP
ENDM DecBP

ncols     dw 0
ncolbytes dw 0

MACRO setcols
      push  ax
      mov   ax,[ds:_tsw_hsize]
      mov   [cs:ncols],ax
      shl   ax,1
      mov   [cs:ncolbytes],ax
      pop   ax
ENDM  setcols

;PROC _tsw_cursoroff
;          IncBP
;          push  bp
;          mov   bp,sp
;          push  si
;          push  di
;
;          mov   cx,2000h
;          mov   ah,1
;          int   10h
;
;          mov   [_tsw_cursorstate],0
;
;          pop   di
;          pop   si
;          pop   bp
;          DecBP
;          ret
;ENDP _tsw_cursoroff
;
;PROC _tsw_cursoron
;          IncBP
;          push  bp
;          mov   bp,sp
;          push  si
;          push  di
;
;          mov   ch,[scan1]
;          mov   cl,[scan2]
;          mov   ah,1
;          int   10h
;
;          mov   [_tsw_cursorstate],1
;
;          pop   di
;          pop   si
;          pop   bp
;          DecBP
;          ret
;ENDP _tsw_cursoron

PROC _tsw_gotoxy
          ARG x:BYTE,y:BYTE
          IncBP
          push  bp
          mov   bp,sp

          push  si
          push  di

          mov   ax,200h
          mov   bx,0
          mov   dl,[x]
          mov   dh,[y]
          int   10h

          pop   di
          pop   si
          pop   bp
          DecBP
          ret
ENDP _tsw_gotoxy


; initcursor: Saves the current scanlines for the cursor

;PROC _tsw_initcursor
;          IncBP
;          push        bp
;          mov         bp,sp
;          xor         ax,ax
;          mov         es,ax
;          mov         bx,460h
;          mov         ah,[es:bx]
;          mov         [scan2],ah
;          inc         bx
;          mov         ah,[es:bx]
;          mov         [scan1],ah
;          pop         bp
;          DecBP
;          ret
;ENDP _tsw_initcursor

PROC _tsw_changeatt
          ARG    attr:BYTE,x1:BYTE,y1:BYTE,x2:BYTE,y2:BYTE
          IncBP
          push   bp
          mov    bp,sp
          push   di

          setcols

          les    di,[_tsw_videobase]

          mov    al,[x1]
          dec    al
          shl    al,1
          xor    ah,ah
          add    di,ax

          mov    al,[y1]
          dec    al
          mov    ah,[BYTE cs:ncols]
          mul    ah
          shl    ax,1
          add    di,ax
          mov    dx,di

          mov    al,[x2]
          mov    ah,[x1]
          sub    al,ah
          inc    al
          mov    bl,al

          mov    al,[y2]
          mov    ah,[y1]
          sub    al,ah
          inc    al

          xor    cx,cx
          cld

          mov    cl,al
@@yloop:  push   cx
          mov    cl,bl
          mov    di,dx

          mov    al,[attr]
@@xloop:  inc    di
          stosb
          loop   @@xloop

          add    dx,[cs:ncolbytes]
          pop    cx
          loop   @@yloop

          pop    di
          pop    bp
          DecBP
          ret
ENDP _tsw_changeatt

PROC _tsw_check_videocard
           IncBP
           push  bp
           mov   bp,sp

check_ega: mov   ax,1200h
           mov   bl,10h
           int   10h
           cmp   bl,10h
           je    check_herc

           mov   [BYTE _tsw_videocard],EGA
           pop   bp
           DecBP
           ret

check_herc:
           call  near ptr check_hercules
           jnc   check_cga

           mov   [BYTE _tsw_videocard],HERCULES
           pop   bp
           DecBP
           ret

check_cga: mov   [BYTE _tsw_videocard],CGAMDA
           pop   bp
           DecBP
           ret
ENDP _tsw_check_videocard

PROC check_hercules near
           mov       dx,3b4h
           call      near ptr find_6845
           jc        no_mono

           mov       dl,0bah
           in        al,dx
           and       al,80h
           mov       ah,al
           mov       cx,8000h
loop1:     in        al,dx
           and       al,80h
           cmp       ah,al
           loope     loop1

           je        no_mono

herc:      stc
           ret

no_mono:   clc
           ret

ENDP check_hercules


PROC find_6845 near

           mov      al,0fh
           out      dx,al
           inc      dx
           in       al,dx
           mov      ah,al
           mov      al,66h
           out      dx,al

           mov      cx,300h

loop2:     loop     loop2

           in       al,dx
           xchg     ah,al
           out      dx,al

           cmp      ah,66h
           je       ok_6845

           stc

ok_6845:   ret

ENDP find_6845

PROC _tsw_fillrect
           ARG    x1:BYTE,y1:BYTE,x2:BYTE,y2:BYTE,char:BYTE,attr:BYTE
           IncBP
           push   bp
           mov    bp,sp
           push   di

           setcols

           les    di,[_tsw_videobase]

           mov    al,[x1]
           dec    al
           shl    al,1
           xor    ah,ah
           add    di,ax

           mov    ah,[BYTE cs:ncols]
           mov    al,[y1]
           dec    al
           mul    ah
           shl    ax,1
           add    di,ax
           mov    dx,di

           mov    al,[x2]
           mov    ah,[x1]
           sub    al,ah
           inc    al
           mov    bl,al

           mov    al,[y2]
           mov    ah,[y1]
           sub    al,ah
           inc    al

           xor    cx,cx
           cld

           mov    cl,al
           mov    ah,[attr]
           mov    al,[char]
@@yloop:   push   cx
           mov    cl,bl
           mov    di,dx
           rep    stosw
           add    dx,[cs:ncolbytes]
           pop    cx
           loop   @@yloop

           pop    di
           pop    bp
           DecBP
           ret
ENDP _tsw_fillrect


PROC _tsw_puttext
           ARG    buf:PTR,x1:BYTE,y1:BYTE,x2:BYTE,y2:BYTE
           IncBP
           push   bp
           mov    bp,sp
           push   di
           push   si
           push   ds

           setcols

           les    di,[_tsw_videobase]

if @DataSize
           lds    si,[buf]
else
           mov    si,[buf]
endif

           mov    al,[x1]
           dec    al
           shl    al,1
           xor    ah,ah
           add    di,ax

           mov    al,[y1]
           dec    al
           mov    ah,[BYTE cs:ncols]
           mul    ah
           shl    ax,1
           add    di,ax
           mov    dx,di

           mov    al,[x2]
           mov    ah,[x1]
           sub    al,ah
           inc    al
           mov    bl,al

           mov    al,[y2]
           mov    ah,[y1]
           sub    al,ah
           inc    al

           xor    cx,cx
           cld

           mov    cl,al
@@yloop:   push   cx
           mov    cl,bl
           mov    di,dx
           rep    movsw
           add    dx,[cs:ncolbytes]
           pop    cx
           loop   @@yloop

           pop    ds
           pop    si
           pop    di
           pop    bp
           DecBP
           ret
ENDP _tsw_puttext

PROC _tsw_gettext
           ARG    buf:PTR,x1:BYTE,y1:BYTE,x2:BYTE,y2:BYTE
           IncBP
           push   bp
           mov    bp,sp
           push   di
           push   si
           push   ds

           setcols

if @DataSize
           les    di,[buf]
else
           mov    ax,ds
           mov    es,ax
           mov    di,[buf]
endif

           lds    si,[_tsw_videobase]

           mov    al,[x1]
           dec    al
           shl    al,1
           xor    ah,ah
           add    si,ax

           mov    al,[y1]
           dec    al
           mov    ah,[BYTE cs:ncols]
           mul    ah
           shl    ax,1
           add    si,ax
           mov    dx,si

           mov    al,[x2]
           mov    ah,[x1]
           sub    al,ah
           inc    al
           mov    bl,al

           mov    al,[y2]
           mov    ah,[y1]
           sub    al,ah
           inc    al

           xor    cx,cx
           cld

           mov    cl,al
@@yloop:   push   cx
           mov    cl,bl
           mov    si,dx
           rep    movsw
           add    dx,[cs:ncolbytes]
           pop    cx
           loop   @@yloop

           pop    ds
           pop    si
           pop    di
           pop    bp
           DecBP
           ret
ENDP _tsw_gettext

PROC _tsw_bgetch
           IncBP
           push  bp
           mov   bp,sp
           xor   ah,ah
           int   16h
           or    al,al
           jz    @@skip
           xor   ah,ah
@@skip:    pop   bp
           DecBP
           ret
ENDP _tsw_bgetch

PROC _check_key
           IncBP
           push  bp
           mov   bp,sp
           mov   ax,100h
           int   16h
           jz    @@nokey
           mov   ax,1
           pop   bp
           DecBP
           ret
@@nokey:   mov   ax,0
           pop   bp
           DecBP
           ret
ENDP _check_key

PROC _tsw_maputc
           ARG    x:BYTE,y:BYTE,attr:BYTE,char:BYTE
           IncBP
           push   bp
           mov    bp,sp
           push   di

           setcols

           xor    ax,ax

           les    di,[_tsw_videobase]

           mov    al,[x]
           dec    al
           shl    ax,1
           add    di,ax

           mov    al,[y]
           dec    al
           mov    ah,[BYTE cs:ncols]
           mul    ah
           shl    ax,1
           add    di,ax

           mov    ah,[attr]
           mov    al,[char]
           stosw

           pop    di
           pop    bp
           DecBP
           ret
ENDP _tsw_maputc


PROC _tsw_mputc
           ARG    x:BYTE,y:BYTE,char:BYTE
           IncBP
           push   bp
           mov    bp,sp
           push   di

           setcols

           xor    ax,ax

           les    di,[_tsw_videobase]

           mov    al,[x]
           dec    al
           shl    ax,1
           add    di,ax

           mov    al,[y]
           dec    al
           mov    ah,[BYTE cs:ncols]
           mul    ah
           shl    ax,1
           add    di,ax

           mov    al,[char]
           stosb

           pop    di
           pop    bp
           DecBP
           ret
ENDP _tsw_mputc

PROC _tsw_whaton
           ARG    x:BYTE,y:BYTE
           IncBP
           push   bp
           mov    bp,sp
           push   di

           setcols

           xor    ax,ax

           les    di,[_tsw_videobase]

           mov    al,[x]
           dec    al
           shl    ax,1
           add    di,ax

           mov    al,[y]
           dec    al
           mov    ah,[BYTE cs:ncols]
           mul    ah
           shl    ax,1
           add    di,ax

           mov    al,[es:di]
           xor    ah,ah

           pop    di
           pop    bp
           DecBP
           ret
ENDP _tsw_whaton


PROC _tsw_mputs
           ARG    x:BYTE,y:BYTE,string:PTR
           IncBP
           push   bp
           mov    bp,sp
           push   ds
           push   si
           push   di

           setcols

           xor    ax,ax

           les    di,[_tsw_videobase]

           mov    al,[x]
           dec    al
           shl    ax,1
           add    di,ax

           mov    al,[y]
           dec    al
           mov    ah,[BYTE cs:ncols]
           mul    ah
           shl    ax,1
           add    di,ax

if @DataSize
           lds    si,[string]
else
           mov    si,[string]
endif

           cld

@@st_loop: lodsb
           or     al,al
           jz     @@skip_loop
           stosb
           inc    di
           jmp    short @@st_loop
@@skip_loop:
           pop    di
           pop    si
           pop    ds
           pop    bp
           DecBP
           ret
ENDP _tsw_mputs


PROC _tsw_maputs
           ARG    x:BYTE,y:BYTE,attr:BYTE,string:PTR
           IncBP
           push   bp
           mov    bp,sp
           push   ds
           push   si
           push   di

           setcols

           xor    ax,ax

           les    di,[_tsw_videobase]

           mov    al,[x]
           dec    al
           shl    ax,1
           add    di,ax

           mov    al,[y]
           dec    al
           mov    ah,[BYTE cs:ncols]
           mul    ah
           shl    ax,1
           add    di,ax

if @DataSize
           lds    si,[string]
else
           mov    si,[string]
endif

           mov    ah,[attr]
           cld
@@st_loop: lodsb
           or     al,al
           jz     @@skip_loop
           stosw
           jmp short @@st_loop

@@skip_loop:
           pop    di
           pop    si
           pop    ds
           pop    bp
           DecBP
           ret
ENDP _tsw_maputs

PROC _tsw_scroll
           ARG mode:BYTE,x1:BYTE,y1:BYTE,x2:BYTE,y2:BYTE,lines:BYTE,attr:BYTE
           IncBP
           push bp
           mov  bp,sp

           mov ch,[y1]
           dec ch
           mov cl,[x1]
           dec cl
           mov al,[lines]
           mov ah,[mode]
           mov dh,[y2]
           dec dh
           mov dl,[x2]
           dec dl
           mov bh,[attr]
           int 10h

           pop bp
           DecBP
           ret
ENDP _tsw_scroll

PROC _tsw_printerstat
           arg  printer:WORD
           IncBP
           push   bp
           mov    bp,sp
           mov    dx,[printer]
           mov    ah,2
           int    17h
           xchg   al,ah
           sub    al,90h
           xor    ah,ah
           pop    bp
           DecBP
           ret
ENDP _tsw_printerstat


;PROC _sound
;        arg     freq:WORD
;        IncBP
;        push    bp
;        mov     bp,sp
;        mov     bx,[freq]
;        mov     ax,034DDh
;        mov     dx,012h
;        cmp     dx,bx
;        jae     @@exit
;        div     bx
;        mov     bx,ax
;        in      al,061h
;        test    al,3
;        jne     lab
;
;        or      al,3
;        out     061h,al
;        mov     al,0B6h
;        out     043h,al
;
;lab:    mov     al,bl
;        out     042h,al
;        mov     al,bh
;        out     042h,al
;@@exit: pop     bp
;        DecBP
;        ret
;ENDP _sound
;
;PROC _nosound
;        IncBP
;        push    bp
;        mov     bp,sp
;        in      al,061h
;        and     al,0FCh
;        out     061h,al
;        pop     bp
;        DecBP
;        ret
;ENDP _nosound
;
;PROC _soundclick
;             IncBP
;             push    bp
;             mov     bp,sp
;             in      al,061h
;             push    ax
;             and     al,0fch
;             out     061h,al
;             mov     cx,200h
;@@x:         loop    @@x
;             or      al,2
;             out     061h,al
;             mov     cx,200h
;@@y:         loop    @@y
;             pop     ax
;             out     061h,al
;             pop     bp
;             DecBP
;             ret
;ENDP _soundclick

PROC _tsw_wait20ms
        push      ax
        push      dx
        mov       ax,[speedvall]
        mov       dx,[speedvalh]
        clc
@@l:    sub       ax,1
        sbb       dx,0
        jnc       @@l
        pop       dx
        pop       ax
        ret
ENDP _tsw_wait20ms

PROC _tsw_checkspeed
            IncBP
            push bp
            mov  bp,sp
            mov  ax,40H
            mov  es,ax
            mov  ax,[es:6cH]

@@waiting:  cmp  ax,[es:6ch]
            je   @@waiting

            mov  ax,[es:6ch]

            clc
            xor  cx,cx
            xor  dx,dx
@@loop:     add  cx,1
            adc  dx,0
            cmp  ax,[es:6ch]
            je   @@loop

            sar  dx,1
            rcr  cx,1
            sar  dx,1
            rcr  cx,1

            mov  [speedvall],cx    ; And magic! We have a speed value!
            mov  [speedvalh],dx
            pop  bp
            DecBP
            ret
ENDP _tsw_checkspeed

PROC _tsw_getcursor
            IncBP
            push bp
            mov  bp,sp
            push si
            push di
            xor  bx,bx
            mov  ax,0300h
            int  10h
            mov  ax,dx
            pop  di
            pop  si
            pop  bp
            DecBP
            ret
ENDP _tsw_getcursor

PROC _tsw_get_dvseg
            ARG origseg:WORD
            IncBP
            push bp
            mov  bp,sp
            push di

            mov  es,[origseg]
            xor  di,di
            mov  ah,0FEh
            int  10h             ; Get DesqView/TopView video segment

            mov  ax,es
            pop  di
            pop  bp
            DecBP
            ret
ENDP _tsw_get_dvseg

PROC _tsw_checkdv
            IncBP
            push    bp
            mov     bp,sp
            mov     ax,2B00h
            mov     cx,4445h
            mov     dx,5351h
            int     21h
            inc     al
            xor     ah,ah
            pop     bp
            DecBP
            ret
ENDP _tsw_checkdv

PROC _slide
      ARG  p:FAR PTR
      LOCAL siz:WORD
      push   bp
      mov    bp,sp
      sub    sp,2
      push   si
      push   di
      push   ds

      setcols

      ; ax -> number of chars to scroll
      ; bx -> index for logo-line to write
      ; cx -> counter
      ; dx -> datasegment

      les    si,[_tsw_videobase]
      mov    cx,[_tsw_vsize]
      dec    cx
      mov    [siz],cx
      mov    ax,[cs:ncols]
      mul    cl                      ; ax = # words to move
      lds    bx,[p]
      ASSUME DS:NOTHING
      mov    dx,ds

      push   ax
      mov    ax,[cs:ncols]
      mov    cx,[siz]
      mul    cl
      shl    ax,1
      add    bx,ax
      pop    ax
      inc    cx
      cld

@@lp:
      push  cx
      mov   si,[cs:ncolbytes]
      mov   di,0
      push  es
      pop   ds
      mov   cx,ax

      rep   movsw    ; scroll one line up

      mov   ds,dx
      mov   si,bx
      mov   cx,[cs:ncols]

      rep   movsw   ; write my line

      push ax
      mov  ax,@data
      mov  ds,ax
      pop  ax

      call  _tsw_wait20ms

      sub   ax,[cs:ncols]
      sub   bx,[cs:ncolbytes]

      pop   cx
      loop  @@lp

      pop    ds
      pop    di
      pop    si
      mov    sp,bp
      pop    bp
      ret
ENDP _slide

ENDS

END
