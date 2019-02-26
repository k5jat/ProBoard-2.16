
/* ------------------------------------------------------------
 * Filename ............... Terminal.Cpp
 *
 * General Purpose ........ Terminal class
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

#include "terminal.hpp"
#include "keyboard.hpp"

/*--] Code [-------------------------------------] Terminal [--------------*/

/*
 * Routine   : Terminal()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Terminal::Terminal()
{
   attr = 0x0;
}

/*
 * Routine   : send()
 * Purpose   : send a formatted string directly to the modem
 * ------------------------------------------------------------------------
 * Parameters: string to send
 * Return    : None

 *
 */

void Terminal::send(ChrP fs, ...)
{
   ChrT varbuf[128];
   va_list v;

   va_start(v, fs);
   vsprintf(varbuf, fs, v);
   va_end(v);

#ifdef PEX
   if(BaudRate > 0)
      if(IO_Carrier())
         for(ChrP p = varbuf;*p;p++)
            IO_SendByte(*p);
#endif
}

/*
 * Routine   : putC()
 * Purpose   : put a character at the current X and Y position
 * ------------------------------------------------------------------------
 * Parameters: character
 * Return    : None
 *
 */

void Terminal::putC(ChrT c)
{
   ::printf("%c", c);
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

void Terminal::putS(ChrP s)
{
   IntT size = strlen(s);
   ::printf("%s", s);
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

void Terminal::printf(ChrP fs, ...)
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
            gotoXY(0, cursor.y + 1);
         }
         break;

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
 * Routine   : attrib()
 * Purpose   : return the current attribute
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : current attribute
 *
 */

BytT Terminal::attrib()
{
   return(attr);
}

/*---------------------------] END OF THE CODE [---------------------------*/
