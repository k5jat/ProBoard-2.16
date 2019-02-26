#include <ctype.h>
#include <tslib.hpp>

static word
_crc(byte x,word cur_crc)
{
   byte bcrc = byte(cur_crc);

   _asm  mov    dx,cur_crc
   _asm  mov    al,x
   _asm  xor    dh,al
   _asm  xor    dl,dl
   _asm  mov    cx,8
crc1:
   _asm  shl    dx,1
   _asm  jnc    crc2
   _asm  xor    dx,0x1021
crc2:
   _asm  loop   crc1
   _asm  xor    dh,bcrc

   return _DX;
}

word
crc(const char *s)
{
   word val=0;

   while(*s) val = _crc(*s++,val);

   return val;
}

word
upcrc(const char *s)
{
   word val=0;

   while(*s)
   {
      char c = toupper(*s++);

      val = _crc(c,val);
   }

   return val;
}

word
crc(const void *p,unsigned size)
{
   char *ptr = (char *)p;
   word val  = 0;

   for(unsigned i=0;i<size;i++) val = _crc(*ptr++,val);

   return val;
}
