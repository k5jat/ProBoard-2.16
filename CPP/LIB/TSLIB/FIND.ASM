
% MODEL MEM_MOD

  CODESEG

public  _f_first,_f_next

PROC _f_first ; f_first(struct f_struc *,char *fname,int attr)
           ARG struct:PTR,fname:PTR,attr:WORD
           push  bp
           mov   bp,sp
           push  ds

if @DataSize
           lds   dx,[struct]
else
           mov   dx,[struct]
endif

           mov   ah,1ah
           int   21h

if @DataSize
           lds   dx,[fname]
else
           mov   dx,[fname]
endif

           mov   cx,[attr]
           mov   ah,4eh
           int   21h
           jc    @@error

           xor   ax,ax
@@error:   pop   ds
           pop   bp
           ret
ENDP _f_first

PROC _f_next ; f_next(struct f_struc *)
           ARG struct:PTR
           push  bp
           mov   bp,sp
           push  ds

if @DataSize
           lds   dx,[struct]
else
           mov   dx,[struct]
endif

           mov   ah,1ah
           int   21h

if @DataSize
           lds   dx,[fname]
else
           mov   dx,[fname]
endif

           mov   cx,[attr]
           mov   ah,4fh
           int   21h
           jc    @@error

           xor   ax,ax
@@error:   pop   ds
           pop   bp
           ret
ENDP _f_next

END

