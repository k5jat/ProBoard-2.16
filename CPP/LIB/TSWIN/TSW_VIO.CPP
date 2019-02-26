#include <string.h>

#ifdef __OS2__
   #define INCL_VIO
   #define INCL_KBD
   #include <os2.h>
#endif

//#include <tswin.hpp>


void
tsw_mputc(int x,int y,char c)
{
   VioWrtCharStr(&c,1,word(y-1),word(x-1),0);
}

void
tsw_maputc(int x,int y,ATTR attr,char c)
{
   VioWrtCharStrAtt(&c,1,word(y-1),word(x-1),&attr,0);
}

void
tsw_wait20ms()
{
}

char
tsw_whaton(int x,int y)
{
   return (char)tsw_videobase[(y-1)*160+(x-1)*2];
}

void
tsw_changeatt(ATTR attr,int x1,int y1,int x2,int y2)
{
   for(int y=y1 ; y<=y2 ; y++)
      VioWrtNAttr(&attr,word(x2-x1+1),word(y-1),word(x1-1),0);
}

void
tsw_scroll(char mode,char x1,char y1,char x2,char y2,char lines,ATTR attr)
{
   byte cell[2];

   cell[0] = ' ';
   cell[1] = attr;

   if(mode == UP)
      VioScrollUp(word(y1-1),word(x1-1),word(y2-1),word(x2-1),lines,cell,0);
   else
      VioScrollDn(word(y1-1),word(x1-1),word(y2-1),word(x2-1),lines,cell,0);
}

void
tsw_fillrect(int x1,int y1,int x2,int y2,char c,ATTR attr)
{
   byte cell[2];

   cell[0] = c;
   cell[1] = attr;

   for(int y=y1 ; y<=y2 ; y++)
      VioWrtNCell(cell,word(x2-x1+1),word(y-1),word(x1-1),0);
}

void
tsw_maputs(int x,int y,ATTR attr,char *s)
{
   VioWrtCharStrAtt(s,word(strlen(s)),word(y-1),word(x-1),&attr,0);
}

void
tsw_mputs(int x,int y,char *s)
{
   VioWrtCharStr(s,word(strlen(s)),word(y-1),word(x-1),0);
}

void
tsw_gettext(word *buf,int x1,int y1,int x2,int y2)
{
   for(int y=y1 ; y<=y2 ; y++)
   {
      word *p = (word *)&tsw_videobase[(y-1)*160+(x1-1)*2];

      memcpy(buf,p,(x2-x1+1)*2);

      buf += x2-x1+1;
   }
}

void
tsw_puttext(word *buf,int x1,int y1,int x2,int y2)
{
   for(int y=y1 ; y<=y2 ; y++)
   {
      word base = word((y-1)*160+(x1-1)*2);
      word size = word((x2-x1+1)*2);

      word *p = (word *)&tsw_videobase[base];

      memcpy(p,buf,size);

      buf += size/2;

      VioShowBuf(base,size,0);
   }
}

void
tsw_cursoron()
{
   VIOCURSORINFO cursor;

   VioGetCurType(&cursor,0);

   if(cursor.attr == USHORT(0xFFFF))
   {
      cursor.attr = 0;
      VioSetCurType(&cursor,0);
   }
}

void
tsw_cursoroff()
{
   VIOCURSORINFO cursor;

   VioGetCurType(&cursor,0);

   if(cursor.attr != USHORT(0xFFFF))
   {
      cursor.attr = USHORT(0xFFFF);
      VioSetCurType(&cursor,0);
   }
}


int
tsw_printerstat(int)
{
   return 0;
}


void
tsw_gotoxy(int x,int y)
{
   VioSetCurPos(word(y-1),word(x-1),0);
}


int
check_key()
{
   KBDKEYINFO kb_info;

   KbdPeek(&kb_info,0);

   if(kb_info.fbStatus & 0xC0)
      return 1;
   else
      return 0;
}

KEY
tsw_bgetch()
{
   KBDKEYINFO kb_info;

   KbdCharIn(&kb_info,0,0);

   if(kb_info.fbStatus & 2)
      return word(kb_info.chScan * 0x100);
   else
      return word(kb_info.chChar);
}


void
sound(unsigned)
{
}

void
stopsound()
{
}

void
soundclick()
{
}

void
slide(void _far *)
{
}
