
% MODEL MEM_MOD

PUBLIC _msdos_error
PUBLIC _msdos_open
PUBLIC _msdos_create
PUBLIC _msdos_close
PUBLIC _msdos_read
PUBLIC _msdos_write
PUBLIC _msdos_lock
PUBLIC _msdos_unlock
PUBLIC _msdos_seek

MACRO calldos func
  mov ah,func
  int 21h
ENDM  calldos

TRUE  EQU 1
FALSE EQU 0

DATASEG

_msdos_error dw 0

CODESEG

PROC _msdos_open
     ARG fname : PTR , openmode : WORD

     push    bp
     mov     bp,sp

if @DataSize
     push    ds
     lds     dx,[fname]
else
     mov     dx,[fname]
endif

     mov     ax,[openmode]

     calldos 3Dh

if @DataSize
     pop     ds
endif

     jc      @@error

     mov     [_msdos_error],0

     jmp short @@exit

@@error:

     mov     [_msdos_error],ax
     mov     ax,-1

@@exit:

     pop     bp

     ret

ENDP _msdos_open

PROC _msdos_create
     ARG fname : PTR
     push    bp
     mov     bp,sp

if @DataSize
     push    ds
     lds     dx,[fname]
else
     mov     dx,[fname]
endif

     xor     cx,cx

     calldos 3Ch

if @DataSize
     pop     ds
endif

     jc      @@error

     mov     [_msdos_error],0

     jmp short @@exit

@@error:

     mov     [_msdos_error],ax
     mov     ax,-1

@@exit:

     pop     bp
     ret
ENDP _msdos_create

PROC _msdos_close
     ARG fh : WORD

     push    bp
     mov     bp,sp

     mov     bx,[fh]

     calldos 3Eh

     pop     bp
     ret
ENDP _msdos_close

PROC _msdos_read
     ARG fh : WORD , buf : PTR , len : WORD

     push    bp
     mov     bp,sp

if @DataSize
     push    ds
     lds     dx,[buf]
else
     mov     dx,[buf]
endif

     mov     cx,[len]
     mov     bx,[fh]

     calldos 3Fh

if @DataSize
     pop     ds
endif

     jc      @@error

     mov     [_msdos_error],0

     jmp short @@exit

@@error:

     mov     [_msdos_error],ax
     mov     ax,-1

@@exit:

     pop     bp

     ret

ENDP _msdos_read

PROC _msdos_write
     ARG fh : WORD , buf : PTR , len : WORD

     push    bp
     mov     bp,sp

if @DataSize
     push    ds
     lds     dx,[buf]
else
     mov     dx,[buf]
endif

     mov     cx,[len]
     mov     bx,[fh]

     calldos 40h

if @DataSize
     pop     ds
endif

     jc      @@error

     mov     [_msdos_error],0

     jmp short @@exit

@@error:

     mov     [_msdos_error],ax
     mov     ax,-1

@@exit:

     pop     bp

     ret

ENDP _msdos_write


PROC _msdos_seek
     ARG fh:WORD,off:DWORD,mode:BYTE

     push    bp
     mov     bp,sp

     mov     al,[mode]
     mov     bx,[fh]
     les     dx,[off]
     mov     cx,es

     calldos 42h

     jc      @@error

     mov     [_msdos_error],0

     jmp short @@exit

@@error:

     mov     [_msdos_error],ax
     mov     ax,-1
     mov     dx,-1

@@exit:
     pop     bp
     ret
ENDP _msdos_seek



PROC _msdos_lock
     ARG fh:WORD,off:DWORD,len:DWORD

     push    bp
     mov     bp,sp
     push    si
     push    di

     xor     al,al
     mov     bx,[fh]
     les     dx,[off]
     mov     cx,es
     les     di,[len]
     mov     si,es

     calldos 5Ch

     jc      @@error

     mov     [_msdos_error],0
     mov     ax,TRUE

     jmp short @@exit

@@error:

     mov     [_msdos_error],ax
     mov     ax,FALSE

@@exit:
     pop     di
     pop     si
     pop     bp
     ret
ENDP _msdos_lock

PROC _msdos_unlock
     ARG fh:WORD,off:DWORD,len:DWORD

     push    bp
     mov     bp,sp
     push    si
     push    di

     mov     al,1
     mov     bx,[fh]
     les     dx,[off]
     mov     cx,es
     les     di,[len]
     mov     si,es

     calldos 5Ch

     jc      @@error

     mov     [_msdos_error],0
     mov     ax,TRUE

     jmp short @@exit

@@error:

     mov     [_msdos_error],ax
     mov     ax,FALSE

@@exit:
     pop     di
     pop     si
     pop     bp
     ret
ENDP _msdos_unlock

  END
