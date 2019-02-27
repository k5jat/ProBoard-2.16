
/* ------------------------------------------------------------
 * Filename ............... Screen.Cpp
 *
 * General Purpose ........ Screen class
 * ------------------------------------------------------------
 * First date ............. 12-08-94 09:37am
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

#include "screen.hpp"
#include "ansi.hpp"
#include "avatar.hpp"
#include "videoram.hpp"

/*-------------------------------------------------------------------------*/

ScreenP TScreen = NULL;

/*-------------------------------------------------------------------------*/

LCL ChrT varbuf[256];

/*--] Code [-------------------------------------] Screen [----------------*/

/*
 * Routine   : Screen()
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Screen::Screen()
{
   local  = new VideoT;
   remote = NULL;

   width  = vrHorSize;

#ifdef PEX
   if(BaudRate > 0)
   {
      mode = 0;
      if( (CurUser->uFlags & (UFLAG_AVATAR | UFLAG_AVTPLUS)) != 0)
         mode = 2;
      else
         if( (CurUser->uFlags & (UFLAG_ANSI)) != 0)
            mode = 1;

      if(mode == 0)
      {
         CurUser->uFlags |= UFLAG_ANSI;
         mode = 1;
      }
   }
   else
      mode = 2;

   height = min(vrVerSize - 1, CurUser->screenLength);
#else
   mode = 1;                          // use ANSI in EXE mode
   height   = vrVerSize - 1;
#endif

   if(mode == 1)
      remote = new AnsiT;

   if(mode == 2)
      remote = new AvatarT;

   remoteEnabled = True;
}

/*
 * Routine   : ~Screen()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Screen::~Screen()
{
   if(local != NULL)
      delete local;

   if(remote != NULL)
      delete remote;
}

/*
 * Routine   : cls()
 * Purpose   : clear the screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Screen::cls()
{
   if(remote != NULL && remoteEnabled)
   {
      remote->color(local->attrib());
      remote->cls();
   }
   else
      local->cls();
}

/*
 * Routine   : color()
 * Purpose   : change the current attribute
 * ------------------------------------------------------------------------
 * Parameters: new attribute
 * Return    : None
 *
 */

void Screen::color(BytT attr)
{
   if(remote != NULL && remoteEnabled)
      remote->color(attr);

   local->color(attr);
}

/*
 * Routine   : gotoXY()
 * Purpose   : move to the specified X and Y location
 * ------------------------------------------------------------------------
 * Parameters: X and Y location
 * Return    : None
 *
 */

void Screen::gotoXY(BytT x, BytT y)
{
   if(remote != NULL && remoteEnabled)
      remote->gotoXY(x, y);
   else
      local->gotoXY(x, y);
}

/*
 * Routine   : scrollUp()
 * Purpose   : try to scroll the window a numberof lines up
 * ------------------------------------------------------------------------
 * Parameters: start, end, number of lines
 * Return    : True if supported, False if NOT
 *
 */

FlgT Screen::scrollUp(BytT start, BytT end, BytT lines)
{
   FlgT ret_val = True;

   if(remote != NULL && remoteEnabled)
      ret_val = remote->scrollUp(start, end, lines);

   if(ret_val == True)
      local->scrollUp(start, end, lines);

   return(ret_val);
}

/*
 * Routine   : scrollDown()
 * Purpose   : try to scroll the window a numberof lines down
 * ------------------------------------------------------------------------
 * Parameters: start, end, number of lines
 * Return    : True if supported, False if NOT
 *
 */

FlgT Screen::scrollDown(BytT start, BytT end, BytT lines)
{
   FlgT ret_val = True;

   if(remote != NULL && remoteEnabled)
      ret_val = remote->scrollDown(start, end, lines);

   if(ret_val == True)
      local->scrollDown(start, end, lines);

   return(ret_val);
}

/*
 * Routine   : clrEol()
 * Purpose   : clear 'till the end of the line
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Screen::clrEol()
{
   if(remote != NULL && remoteEnabled)
      remote->clrEol();
   else
      local->clrEol();
}

/*
 * Routine   : putC()
 * Purpose   : put a character on the screen
 * ------------------------------------------------------------------------
 * Parameters: character
 * Return    : None
 *
 */

void Screen::putC(ChrT c)
{
   if(remote != NULL && remoteEnabled)
      remote->putC(c);
   else
      local->putC(c);
}

/*
 * Routine   : putS()
 * Purpose   : put a string on the screen
 * ------------------------------------------------------------------------
 * Parameters: character
 * Return    : None
 *
 */

void Screen::putS(ChrP s)
{
   if(remote != NULL && remoteEnabled)
      remote->putS(s);
   else
      local->putS(s);
}

/*
 * Routine   : printf()
 * Purpose   : print a formatted string on the screen
 * ------------------------------------------------------------------------
 * Parameters: formatted string
 * Return    : None
 *
 */

void Screen::printf(ChrP fs, ...)
{
   va_list v;

   va_start(v, fs);
   vsprintf(varbuf, fs, v);
   va_end(v);

   if(remote != NULL && remoteEnabled)
      remote->printf(varbuf);
   else
      local->printf(varbuf);
}

/*
 * Routine   : cursorOn()
 * Purpose   : hide the cursor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Screen::cursorOff()
{
   local->cursorOff();
}

/*
 * Routine   : cursorOn()
 * Purpose   : unhide the cursor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Screen::cursorOn()
{
   local->cursorOn();
}


/*
 * Routine   : attrib()
 * Purpose   : ask the current attribute (local screen)
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : current attribute
 *
 */

BytT Screen::attrib()
{
   return(local->attrib());
}

/*
 * Routine   : centerLine()
 * Purpose   : center the formatted string on the specified line
 * ------------------------------------------------------------------------
 * Parameters: line number, attribute, formatted text
 * Return    : None
 *
 */

void Screen::centerLine(BytT lineNo, BytT attr, ChrP fs, ...)
{
   BytT tmpAttr = attrib();
   Cursor tmpCursor;

   va_list v;

   va_start(v, fs);
   vsprintf(varbuf, fs, v);
   va_end(v);

   gotoXY(0, lineNo);
   color(attr);
   clrEol();

   BytT left = (width - strlen(varbuf)) / 2;
   gotoXY(left, lineNo);
   putS(varbuf);

   color(tmpAttr);
   gotoXY(tmpCursor.x, tmpCursor.y);
}

/*---------------------------] END OF THE CODE [---------------------------*/
