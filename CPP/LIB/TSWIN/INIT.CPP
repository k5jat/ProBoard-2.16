#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <dos.h>
#endif

#ifdef __OS2__
   #define INCL_VIO
   #include <os2.h>
#endif

#include <tswin.hpp>

const char *DOUBLE_BORDER = "…Õª∫ºÕ»∫";
const char *SINGLE_BORDER = "⁄ƒø≥Ÿƒ¿≥";
const char *BLOCK_BORDER  = "‹‹‹€ﬂﬂﬂ€";
const char *CHISEL_BORDER = "⁄ƒø≥Ÿƒ¿≥";
const char *RECESS_BORDER = "‹‹‹ﬁﬂﬂﬂ›";

Window SCREEN;
KeyBoard KB;

byte tsw_scrmode   = COLOR;
byte tsw_videocard = HERCULES;
int  tsw_vsize     = 25;
int  tsw_hsize     = 0;
bool tsw_cursorstate = TRUE;

bool tsw_shutup = FALSE;

char tsw_vs[81] = "C++ User Interface Library v1.4  (c) 1988-1995 Philippe Leybaert";

int  tsw_OS=OS_DOS;

char far *tsw_videobase=(char far *)0L;

#ifdef __OS2__
   inline void tsw_checkspeed() {}
   inline void tsw_initcursor() {}
   inline void tsw_check_videocard() { tsw_videocard = EGA ; }
   inline word tsw_get_dvseg(word) { return 0; }
   inline bool tsw_checkdv() { return FALSE; }
   inline word tsw_getcursor() { return 0; }
#else
   extern "C"
      {
         void tsw_checkspeed();
         void tsw_check_videocard();
         word tsw_get_dvseg(word);
         bool tsw_checkdv();
         word tsw_getcursor();
      }
#endif

void tsw_initcursor();


KeyBoard::KeyBoard()
{
#ifdef __OS2__
   ULONG os2_v_addr;
   USHORT os2_v_size;

   VioGetBuf(&os2_v_addr,&os2_v_size,0);

   tsw_videobase = (byte *)(byte __far16 *)os2_v_addr;
#else
   byte far *video_mode = (byte far *)0x449;

   word videoseg = 0xB800;

   if(*video_mode==7)
   {
      videoseg    = 0xB000;
      tsw_scrmode = MONOCHROME;
   }

   videoseg = tsw_get_dvseg(videoseg);

   tsw_videobase = (char _far *) MK_FP(videoseg , *((unsigned _far *)0x44E));

   if(tsw_checkdv()) tsw_OS = OS_DESQVIEW;
#endif

 tsw_initcursor();
 tsw_check_videocard();
 tsw_checkspeed();

#ifdef __OS2__
   tsw_hsize = 80;
   tsw_vsize = 25;
#else
   if(tsw_videocard == EGA) tsw_vsize = *((byte _far *)0x484)+1;

   tsw_hsize = *((byte far *)0x44A);
#endif

 word pos = tsw_getcursor();

 SCREEN.open(1,1,tsw_hsize,tsw_vsize,0x7,NOBORDER|NOSAVE|NOCLEAR);

 SCREEN.setPos(pos%256+1,pos/256+1);

 help = NULLFUNC;
 last = 0;
}
