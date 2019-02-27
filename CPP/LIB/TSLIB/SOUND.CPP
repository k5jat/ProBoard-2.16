#include <dos.h>
#include <tslib.hpp>

void
sound(word freq)
{
   if(freq > 20000)
      freq = 20000;
    if(freq < 20)
      freq = 20;

   word cycle = 1193180L / freq;

   byte p61 = inportb(0x61);

   if(!(p61 & 3))
   {
      outportb(0x61,p61|3);
      outportb(0x43,0xB6);
   }

   outportb(0x42, byte(cycle));
   outportb(0x42, byte(cycle>>8));
}

void
stopsound()
{
   outportb(0x61,inportb(0x61) & 0xFC);
}

void
soundclick()
{
}
/*
PROC _nosound
        push    bp
        mov     bp,sp
        in      al,061h
        and     al,0FCh
        out     061h,al
        pop     bp
        ret
ENDP _nosound

PROC _soundclick
        push    bp
        mov     bp,sp
        in      al,061h
        push    ax
        and     al,0fch
        out     061h,al
        mov     cx,200h
@@x:    loop    @@x
        or      al,2
        out     061h,al
        mov     cx,200h
@@y:    loop    @@y
        pop     ax
        out     061h,al
        pop     bp
        ret
ENDP _soundclick
*/
