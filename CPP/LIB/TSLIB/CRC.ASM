% MODEL MEM_MOD

  CODESEG

public _crc

PROC _crc
         ARG x:BYTE,cur_crc:WORD

         push   bp
         mov    bp,sp

         mov    dx,[cur_crc]
         mov    al,[x]
         xor    dh,al
         xor    dl,dl
         mov    cx,8
crc1:    shl    dx,1
         jnc    crc2
         xor    dx,1021h
crc2:    loop   crc1
         xor    dh,[BYTE cur_crc]
         mov    ax,dx

         pop    bp
         ret
ENDP _crc

  END

