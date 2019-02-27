
/* ------------------------------------------------------------
 * Filename ............... Handler.Cpp
 *
 * General Purpose ........ The local key handler
 * ------------------------------------------------------------
 * First date ............. 12-28-94 02:06pm
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

#include "keyboard.hpp"
#include "setup.hpp"

/*--] Code [-----------------> keyboard handling <-------------------------*/

/*
 * Routine   : LocalKeyHandler()
 * Purpose   : The local keyboard handler
 * ------------------------------------------------------------------------
 * Parameters: the local key
 * Return    : True if handled, False if NOT
 *
 */

IntT LocalKeyHandler(WrdT key)
{
   IntT ret_val = False;

   if(TSetup->rawMode == False)
   {
      switch(key)
      {
         case KEY_AF1   :
         case KEY_AF2   :
         case KEY_AF3   :
         case KEY_AF4   :
         case KEY_AF5   :
         case KEY_AF6   :
         case KEY_AF7   :
         case KEY_AF8   :
         case KEY_AF9   :
         case KEY_AF10  :
         case KEY_DEL   :
         case KEY_INS   :
         case KEY_HOME  :
         case KEY_END   :
         case KEY_UP    :
         case KEY_DN    :
         case KEY_LT    :
         case KEY_RT    :
         case KEY_PGUP  :
         case KEY_PGDN  :
         case 0x7300    :        // CTRL-LEFT and RIGHT
         case 0x7400    :
         case KEY_ALTB  :        // toggle banner
         case KEY_ALTK  :        // raw vs normal
         case KEY_ALTP  :        // print
         case KEY_ALTA  :        // abbrevs
         {
            TKeyboard->poke(key, True);
            ret_val = True;
         }
         break;
      }
   }
   else
   {
      if(key == KEY_ALTK)
      {
         TKeyboard->poke(key, True);
         ret_val = True;
      }
   }

   return (ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
