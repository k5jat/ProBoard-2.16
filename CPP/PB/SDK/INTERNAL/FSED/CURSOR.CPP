
/* ------------------------------------------------------------
 * Filename ............... Cursor.Cpp
 *
 * General Purpose ........ Cursor class
 * ------------------------------------------------------------
 * First date ............. 12-07-94 01:08pm
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

#include "cursor.hpp"
#include "videoram.hpp"

/*-------------------------------------------------------------------------*/

BytT  vrScanTop    = 0;
BytT  vrScanBottom = 0;

/*--] Code [-------------------------------------] Cursor [----------------*/

/*
 * Routine   : Cursor()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Cursor::Cursor()
{
   get();
}

/*
 * Routine   : gotoXY()
 * Purpose   : move the cursor to the specified location
 * ------------------------------------------------------------------------
 * Parameters: X and Y coordinate
 * Return    : None
 *
 */

void Cursor::gotoXY(BytT xp, BytT  yp)
{
   x = xp;
   y = yp;

   asm{
      mov   ax, 200h
      mov   bx, 0
      mov   dl, [xp]
      mov   dh, [yp]
      int   10h
   }
}

/*
 * Routine   : hide()
 * Purpose   : hide the cursor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Cursor::hide()
{
   asm{
      mov   cx, 2000h
      mov   ah, 1
      int   10h
   }
}

/*
 * Routine   : unhide()
 * Purpose   : make the cursor visible again
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Cursor::unhide()
{
   asm{
      mov   ch, [vrScanTop]
      mov   cl, [vrScanBottom]
      mov   ah, 1
      int   10h
   }
}

/*
 * Routine   : get()
 * Purpose   : get the current X and Y location from the BIOS
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Cursor::get()
{
   BytT xp, yp;

   asm{
      xor   bx, bx
      mov   ax, 0300h
      int   10h
      mov   [xp], dl
      mov   [yp], dh
   }

   x = xp;
   y = yp;
}

/*
 * Routine   : left()
 * Purpose   : move the cursor a number of character left
 * ------------------------------------------------------------------------
 * Parameters: number of characters
 * Return    : True if OK, False if NOT
 *
 */

FlgT Cursor::left(BytT chars)
{
   FlgT ret_val = True;
   for(register IntT cnt = 0; cnt < chars && ret_val == True; cnt++)
   {
      if(x > 0)
         x--;
      else
         ret_val = False;
   }

   return(ret_val);
}

/*
 * Routine   : right()
 * Purpose   : move the cursor a number of character right
 * ------------------------------------------------------------------------
 * Parameters: number of characters
 * Return    : True if OK, False if NOT
 *
 */

FlgT Cursor::right(BytT chars)
{
   FlgT ret_val = True;
   for(register IntT cnt = 0; cnt < chars && ret_val == True; cnt++)
   {
      if(x < vrHorSize - 1)
         x++;
      else
         ret_val = False;
   }

   return(ret_val);
}

/*
 * Routine   : up()
 * Purpose   : move the cursor a number of lines up
 * ------------------------------------------------------------------------
 * Parameters: number of lines
 * Return    : True if OK, False if NOT
 *
 */

FlgT Cursor::up(BytT lines)
{
   FlgT ret_val = True;
   for(register IntT cnt = 0; cnt < lines && ret_val == True; cnt++)
   {
      if(y > 0)
         y--;
      else
         ret_val = False;
   }

   return(ret_val);
}

/*
 * Routine   : down()
 * Purpose   : move the cursor a number of lines down
 * ------------------------------------------------------------------------
 * Parameters: number of line
 * Return    : True if OK, False if NOT
 *
 */

FlgT Cursor::down(BytT lines)
{
   FlgT ret_val = True;
   for(register IntT cnt = 0; cnt < lines && ret_val == True; cnt++)
   {
      if(y < vrVerSize - 1)
         y++;
      else
         ret_val = False;
   }

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
