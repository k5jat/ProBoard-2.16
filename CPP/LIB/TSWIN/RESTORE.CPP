#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#endif

#include <tswin.hpp>

extern "C" unsigned tsw_getcursor();

ScreenRestore::ScreenRestore(int m)
{
   mode = byte(m);

   buffer = new word[tsw_vsize*tsw_hsize];

   if(buffer)
   {
      tsw_gettext(buffer,1,1,tsw_hsize,tsw_vsize);

      pos = word(tsw_getcursor());
   }
}

ScreenRestore::~ScreenRestore()
{
   if(buffer)
   {
      if(mode) slide(buffer);
         else  tsw_puttext(buffer,1,1,tsw_hsize,tsw_vsize);

      tsw_gotoxy(pos&0xFF,pos>>8);
   }

   tsw_cursoron();
}

