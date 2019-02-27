
/* ------------------------------------------------------------
 * Filename ............... Console.Cpp
 *
 * General Purpose ........ Console class
 * ------------------------------------------------------------
 * First date ............. 12-06-94 10:31am
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

#include "console.hpp"
#include "screen.hpp"

/*--] Code [-------------------------------------] Console [---------------*/

/*
 * Routine   : Console()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: coordinates, default attribute
 * Return    : None
 *
 */

Console::Console(BytT ux, BytT uy, BytT lx, BytT ly, BytT defAttr)
{
   upperX = ux;
   upperY = uy;
   lowerX = lx;
   lowerY = ly;

   width  = (lowerX - upperX) + 1;
   height = (lowerY - upperY) + 1;

   defaultAttr = defAttr;

   lines = new ConLineP[height];
   for(IntT cnt = 0; cnt < height; cnt++)
      lines[cnt] = new ConLineT(width, defAttr);
}

/*
 * Routine   : ~Console()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Console::~Console()
{
   for(IntT cnt = 0; cnt < height; cnt++)
      delete lines[cnt];

   delete lines;
}

/*
 * Routine   : setLine()
 * Purpose   : set the contents of the specified line
 * ------------------------------------------------------------------------
 * Parameters: line number, text, attribute
 * Return    : None
 *
 */

void Console::setLine(BytT lineNo, ChrP text, BytT attr)
{
   ConLineP line = lines[lineNo];
   line->set(text);
   line->set(attr);
}

/*
 * Routine   : clearLine()
 * Purpose   : clear the specified line
 * ------------------------------------------------------------------------
 * Parameters: line number, attribute
 * Return    : None
 *
 */

void Console::clearLine(BytT lineNo)
{
   ConLineP line = lines[lineNo];
   line->set("");
   line->set(attrib());
   line->get();
}

/*
 * Routine   : gotoXY()
 * Purpose   : move the cursor to a speciied X and Y location
 * ------------------------------------------------------------------------
 * Parameters: X and Y location
 * Return    : None
 *
 */

void Console::gotoXY(BytT x, BytT y)
{
   TScreen->gotoXY(upperX + x, upperY + y);
}

/*
 * Routine   : draw()
 * Purpose   : draw the whole screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Console::draw()
{
   TScreen->cursorOn();

   for(IntT cnt = 0; cnt < height; cnt++)
   {
      ChrT buffer[128];

      ConLineP line = lines[cnt];
      IntT x = line->get(buffer);
      if(x != -1)
      {
         BytT attrib = line->attrib();
         TScreen->color(attrib);

         if(x == -2)                // full redraw, attrib changed!
         {
            gotoXY(0, cnt);
            TScreen->clrEol();

            if(!line->isEmpty())
               TScreen->putS(buffer);
         }
         else
         {
            gotoXY(x, cnt);

            if(line->isEmpty())
               TScreen->clrEol();
            else
               TScreen->putS(buffer);
         }
      }
   }

   TScreen->cursorOn();
}

/*
 * Routine   : redraw()
 * Purpose   : redraw the whole screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Console::redraw()
{
   TScreen->cursorOff();

   for(IntT cnt = 0; cnt < height; cnt++)
   {
      ChrT buffer[81];

      ConLineP line = lines[cnt];

      strcpy(buffer, line->get());
      IntT l = strlen(buffer);
      for(ChrP p = buffer + l - 1; *p == ' '; p--)
         *p = '\0';

      gotoXY(0, cnt);
      TScreen->color(line->attrib());
      TScreen->clrEol();
      TScreen->putS(buffer);
   }

   TScreen->cursorOn();
}

/*
 * Routine   : attrib()
 * Purpose   : return the default attribute
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : default attribute
 *
 */

BytT Console::attrib()
{
   return(defaultAttr);
}

/*
 * Routine   : cls()
 * Purpose   : clear the whole screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Console::cls()
{
   TScreen->cls();
}

/*
 * Routine   : scrollUp()
 * Purpose   : try to scroll the window one line up
 * ------------------------------------------------------------------------
 * Parameters: upperY, height
 * Return    : None
 *
 */

void Console::scrollUp(BytT uy, BytT h)
{
   FlgT canScroll = TScreen->scrollUp(upperY + uy, upperY + uy + h - 1, 1);
   if(canScroll == True)
   {
      IntT max = uy + h;
      ConLineP swap = lines[uy];
      for(IntT i = uy; i < max - 1; i++)
         lines[i] = lines[i + 1];

      lines[max - 1] = swap;
      clearLine(max - 1);
   }
}

/*
 * Routine   : scrollDown()
 * Purpose   : try to scroll the window one line down
 * ------------------------------------------------------------------------
 * Parameters: upper Y and height
 * Return    : None
 *
 */

void Console::scrollDown(BytT uy, BytT h)
{
   FlgT canScroll = TScreen->scrollDown(upperY + uy, upperY + uy + h - 1, 1);
   if(canScroll == True)
   {
      IntT max = uy + h;
      ConLineP swap = lines[max - 1];
      for(IntT i = max - 1; i > uy; i--)
         lines[i] = lines[i - 1];

      lines[uy] = swap;
      clearLine(uy);
   }
}

/*---------------------------] END OF THE CODE [---------------------------*/
