
/* ------------------------------------------------------------
 * Filename ............... Ansi.Cpp
 *
 * General Purpose ........ Ansi class
 * ------------------------------------------------------------
 * First date ............. 12-05-94 03:04pm
 *
 * First in version ....... 2.00
 *
 * Written by ............. Alain Schellinck
 *
 * Copyright .............. Nephilim software development
 * ------------------------------------------------------------
 * Revisions:
 * ----------
 *
 *   Date   | By |                  Purpose                   |
 * ---------+----+--------------------------------------------+
 *          |    |                                            |
 *          |    |                                            |
 *          |    |                                            |
 */

/*-------------------------------------------------------------------------*/

#include "main.hpp"
#pragma  hdrstop

/*-------------------------------------------------------------------------*/

#include "ansi.hpp"

/*--] Code [-------------------------------------] Ansi [------------------*/

/*
 * Routine   : gotoXY()
 * Purpose   : move the cursor to the specified X and Y location
 * ------------------------------------------------------------------------
 * Parameters: X and Y location
 * Return    : True if supported, False if NOT
 *
 */

FlgT Ansi::gotoXY(BytT x, BytT y)
{
   cursor.get();
   ChrT moveX = x - cursor.x;
   ChrT moveY = y - cursor.y;

   if(moveX != 0 && moveY != 0)
      ::printf("\x1b[%d;%dH", y + 1, x + 1);
   else
   {
      if(moveY != 0)
         ::printf("\x1b[%d%c", abs(moveY), (moveY < 0 ? 'A' : 'B'));

      if(moveX != 0)
         ::printf("\x1b[%d%c", abs(moveX), (moveX < 0 ? 'D' : 'C'));
   }

   cursor.x = x;
   cursor.y = y;

   return(True);
}

/*
 * Routine   : color()
 * Purpose   : change the color to the specified attribute
 * ------------------------------------------------------------------------
 * Parameters: color attribute
 * Return    : True if supported, False if not
 *
 */

FlgT Ansi::color(BytT a)
{
   LCL IntT ansiAttr[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };

   if(attr != a)
   {
      attr = a;

      if(attr != 0)
      {
         BytT fgAtt = ((attr & 0x07)     );
         BytT bgAtt = ((attr & 0x70) >> 4);

         ChrP blink = "";
         if(attr & 0x80)
            blink = ";5";

         ChrP bold = "";
         if(attr & 0x08)
            bold = ";1";

         ::printf("\x1b[0%s%s;%d;%dm", blink, bold,
                                     ansiAttr[fgAtt] + 30,
                                     ansiAttr[bgAtt] + 40
                 );
      }
   }

   return(True);
}

/*
 * Routine   : clrEol()
 * Purpose   : clear till end of line
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if supported, False if NOT
 *
 */

FlgT Ansi::clrEol()
{
   ::printf("\x1b[K");
   return(True);
}

/*
 * Routine   : cls()
 * Purpose   : clear the screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if supported, False if NOT
 *
 */

FlgT Ansi::cls()
{
   ::printf("\x1b[2J");
   return(True);
}

/*---------------------------] END OF THE CODE [---------------------------*/
