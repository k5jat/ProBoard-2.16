
/* ------------------------------------------------------------
 * Filename ............... Avatar.Cpp
 *
 * General Purpose ........ Avatar class
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

#include "avatar.hpp"
#include "videoram.hpp"

/*--] Code [-------------------------------------] Avatar [----------------*/

/*
 * Routine   : gotoXY()
 * Purpose   : move the cursor to the specified position
 * ------------------------------------------------------------------------
 * Parameters: X and Y position
 * Return    : True if supported, False if not
 *
 */

FlgT Avatar::gotoXY(BytT x, BytT y)
{
   cursor.get();
   ChrT moveX = x - cursor.x;
   ChrT moveY = y - cursor.y;

   if(abs(moveX) > 1 || abs(moveY) > 1)            // ^V^H<row><col>
      ::printf("%c%c", y + 1, x + 1);
   else
   {
      if(abs(moveY) == 1)
         if(moveY < 0)
            ::printf("");                        // ^V^C
         else
            ::printf("");                        // ^V^D

      if(abs(moveX) == 1)
         if(moveX < 0)
            ::printf("");                        // ^V^E
         else
            ::printf("");                        // ^V^F
   }

   cursor.x = x;
   cursor.y = y;

   return(True);
}

/*
 * Routine   : color()
 * Purpose   : change the color
 * ------------------------------------------------------------------------
 * Parameters: color value (BIOS equivalent)
 * Return    : True if supported, False if not
 *
 */

FlgT Avatar::color(BytT a)
{
   if(attr != a)
   {
      attr = a;
      if(attr != 0)
      {
         ::printf("%c%s",
                  (attr & 0x7F),                      // ^V^A<attr>
                  ((attr & 0x80) != 0 ? "" : "")    // ^V^B if blink mode
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
 * Return    : True if supported, False if not
 *
 */

FlgT Avatar::clrEol()
{
   ::printf("\x07");
   return(True);
}

/*
 * Routine   : scrollUp()
 * Purpose   : scroll the window one line up
 * ------------------------------------------------------------------------
 * Parameters: coordinates
 * Return    : True if supported, False if not
 *
 */

FlgT Avatar::scrollUp(BytT start, BytT end, BytT lines)
{
   send("\x0A%c%c%c%c%c",            // ^V^J<lines><up><left><low><right>
         lines, start + 1, 1, end + 1, vrHorSize
        );

   return(True);
}

/*
 * Routine   : scrollDown()
 * Purpose   : scroll the window one line down
 * ------------------------------------------------------------------------
 * Parameters: coordinates
 * Return    : True if supported, False if not
 *
 */

FlgT Avatar::scrollDown(BytT start, BytT end, BytT lines)
{
   send("\x0B%c%c%c%c%c",            // ^V^K<lines><up><left><low><right>
         lines, start + 1, 1, end + 1, vrHorSize
       );

   return(True);
}

/*---------------------------] END OF THE CODE [---------------------------*/
