#include <dos.h>
#include <tswin.hpp>

static byte scan1,scan2;

void
tsw_cursoroff()
{
   REGS r;

   r.x.cx = 0x2000;
   r.h.ah = 1;

   int86(0x10,&r,&r);

   tsw_cursorstate = FALSE;
}


void
tsw_cursoron()
{
   REGS r;

   r.h.ah = 1;
   r.h.cl = scan1;
   r.h.ch = scan2;

   int86(0x10,&r,&r);

   tsw_cursorstate = TRUE;
}


void
tsw_initcursor()
{
   byte _far *cstate = (byte _far *)(0x460L);

   scan1 = *cstate++;
   scan2 = *cstate;
}

