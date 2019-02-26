
/* ------------------------------------------------------------
 * Filename ............... Banner.Cpp
 *
 * General Purpose ........ Banner class
 * ------------------------------------------------------------
 * First date ............. 12-20-94 10:56am
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

#include "banner.hpp"
#include "screen.hpp"
#include "setup.hpp"
#include "editor.hpp"
#include "usestamp.hpp"

/*-------------------------------------------------------------------------*/

BannerP TBanner = NULL;

/*-------------------------------------------------------------------------*/

/*
 * Routine   : isScrollLockOn()
 * Purpose   : check if the scroll lock key is ON
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if ON, FALSE if NOT
 *
 */

FlgT isScrollLockOn()
{
   FlgT ret_val = False;

   BytFP KeyState = BytFP(MK_FP(0x0,0x417));
   if((*KeyState & 0x10) == 0x10)
      ret_val = True;

   return(ret_val);
}

/*--] Code [-------------------------------------] Banner [----------------*/

/*
 * Routine   : Banner()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Banner::Banner()
{
   counter    = 0;
   memWarning = False;
   message    = NULL;
   waitTime   = 500;
}

/*
 * Routine   : toggle()
 * Purpose   : toggle the banner
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

void Banner::toggle()
{
   timer.start();
   waitTime = 500;

   counter++;

   redraw();
}

/*
 * Routine   : update()
 * Purpose   : update the banner line if required
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

void Banner::update()
{
   if(timer.elapsed() >= waitTime)
   {
      if(message != NULL)
      {
         free(message);
         message = NULL;
      }

      toggle();
   }
   else
      if(TEditor->lowMemory() != memWarning)
         redraw();
}

/*
 * Routine   : unregistered()
 * Purpose   : let the sysop know that the function is not available
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

void Banner::unregistered()
{
//   putS("-=ð this function is not available in the unregistered version ð=-");
}

/*
 * Routine   : redraw()
 * Purpose   : redraw the banner line
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

void Banner::redraw()
{
   IntT line = TScreen->height - 1;
   IntT attr = TSetup->fields[MenuBar]->attr;

   if(TEditor->lowMemory() == True)
   {
      if(TEditor->outOfMemory() == False)
         TScreen->centerLine(line, Color(Brown|HiLight, Red), "%s", TSetup->language->get(AlmostNoMem));
      else
         TScreen->centerLine(line, Color(Brown|HiLight, Red), "%s %s",
                              TSetup->language->get(OutOfMemory),
                              TSetup->language->get(PleaseSave )
                            );

      memWarning = True;
   }
   else
   {
      memWarning = False;

      if(message == NULL)
         TScreen->centerLine(line, attr, "ProBoard Full Screen Editor  Copyright (c) 1996-1999 TeleGrafix Communications" );
      else
         TScreen->centerLine(TScreen->height - 1, Color(Brown|HiLight, Red), message);
   }
#ifdef PEX
   AdjustTime();              // to be sure PB updates the status bar
#endif
}

/*
 * Routine   : putS()
 * Purpose   : put a string on the banner
 * ------------------------------------------------------------------------
 * Parameters: string
 * Return    : None
 *
 */

void Banner::putS(ChrP str)
{
   if(message != NULL)
      free(message);

   message = strdup(str);

   timer.start();
   waitTime = 60;
   redraw();
}

/*---------------------------] END OF THE CODE [---------------------------*/
