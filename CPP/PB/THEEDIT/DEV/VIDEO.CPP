
/* ------------------------------------------------------------
 * Filename ............... Video.Cpp
 *
 * General Purpose ........ Video class
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

#include "video.hpp"
#include "videoram.hpp"

/*-------------------------------------------------------------------------*/

BytT  vrVideoCard;
WrdT  vrHorSize;
WrdT  vrVerSize;
ChrFP vrVideoAddr;

/*--] Code [-------------------------------------] Video [-----------------*/

/*
 * Routine   : Video()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Video::Video()
{
   attr = 0;
   vrInit();
   setVideoAddr();

   vrHorSize = 80;
}

/*
 * Routine   : setVideoAddr()
 * Purpose   : set the video address at the location used by the cursor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Video::setVideoAddr()
{
   videoAddr = vrVideoAddr + ( ((cursor.y * vrHorSize) + cursor.x) * 2);
}

/*
 * Routine   : scrollUp()
 * Purpose   : scroll a number of lines up
 * ------------------------------------------------------------------------
 * Parameters: start line, end line, number of lines
 * Return    : None
 *
 */

void Video::scrollUp(BytT start, BytT end, BytT lines)
{
   BytT a = attr;
   BytT width = vrHorSize - 1;

   asm{
      mov   ch, [start]
      mov   dh, [end]
      mov   cl, 0
      mov   dl, [width]
      mov   al, [lines]
      mov   ah, 6                      // up
      mov   bh, [a]
      int   10h
   }
}

/*
 * Routine   : scrollDown()
 * Purpose   : scroll a number of lines down
 * ------------------------------------------------------------------------
 * Parameters: start line, end line, number of lines
 * Return    : None
 *
 */

void Video::scrollDown(BytT start, BytT end, BytT lines)
{
   BytT a = attr;
   BytT width = vrHorSize - 1;

   asm{
      mov   ch, [start]
      mov   dh, [end]
      mov   cl, 0
      mov   dl, [width]
      mov   al, [lines]
      mov   ah, 7                      // down
      mov   bh, [a]
      int   10h
   }
}

/*
 * Routine   : cls()
 * Purpose   : clear the whole screen using the current attribute
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Video::cls()
{
   scrollUp(0, vrVerSize - 1, 0);
   gotoXY(0, 0);
}

/*
 * Routine   : gotoXY()
 * Purpose   : set the cursor to a specified X and Y position
 * ------------------------------------------------------------------------
 * Parameters: X and Y position
 * Return    : None
 *
 */

void Video::gotoXY(BytT x, BytT y)
{
   cursor.gotoXY(x, y);
   setVideoAddr();
}

/*
 * Routine   : putC()
 * Purpose   : put a character at the current X and Y position
 * ------------------------------------------------------------------------
 * Parameters: character
 * Return    : None
 *
 */

void Video::putC(ChrT c)
{
   *(videoAddr++) = c;
   *(videoAddr++) = attr;
   cursor.right();
}


/*
 * Routine   : putS()
 * Purpose   : put a string at the current X and Y position
 * ------------------------------------------------------------------------
 * Parameters: string
 * Return    : None
 *
 */

void Video::putS(ChrP s)
{
   IntT size = strlen(s);
   for(register IntT cnt = 0; cnt < size; cnt++)
   {
      *(videoAddr++) = *(s++);
      *(videoAddr++) = attr;
   }

   cursor.right(size);
}

/*
 * Routine   : printf()
 * Purpose   : print a formatted string to the video ram
 * ------------------------------------------------------------------------
 * Parameters: format string, parameters
 * Return    : None
 *
 */

void Video::printf(ChrP fs, ...)
{
   ChrT varbuf[1024];
   va_list v;

   va_start(v, fs);
   vsprintf(varbuf, fs, v);
   va_end(v);

   ChrP p = varbuf;
   for(;*p;p++)
   {
      BytT c = BytT(*p);
      switch(c)
      {
         case '\n':
         {
            cursor.down();
         }

         case '\r':
         {
            gotoXY(0, cursor.y);
         }
         break;

         case '\t':
         {
            BytT xp = cursor.x;
            xp = ((xp / 8) + 1) * 8;
            cursor.right(xp - cursor.x);
         }
         break;

         default  :
         {
            if(c >= 32)
               putC(c);
         }
      }
   }
}

/*
 * Routine   : color()
 * Purpose   : change the current attribute
 * ------------------------------------------------------------------------
 * Parameters: new attribute
 * Return    : None
 *
 */

void Video::color(BytT a)
{
   attr = a;
}

/*
 * Routine   : clrEol()
 * Purpose   : clear till end of line
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Video::clrEol()
{
   ChrP tmpAddr = videoAddr;

   BytT size = vrHorSize - cursor.x - 1;
   for(IntT cnt = 0; cnt < size; cnt++)
   {
      *(tmpAddr++) = ' ';
      *(tmpAddr++) = attr;
   }
}

/*
 * Routine   : attrib()
 * Purpose   : return the current attribute
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : current attribute
 *
 */

BytT Video::attrib()
{
   return(attr);
}

/*
 * Routine   : cursorOff()
 * Purpose   : disable the cursor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Video::cursorOff()
{
   cursor.hide();
}

/*
 * Routine   : cursorOn()
 * Purpose   : enable the cursor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Video::cursorOn()
{
   cursor.unhide();
}

/*---------------------------] END OF THE CODE [---------------------------*/
