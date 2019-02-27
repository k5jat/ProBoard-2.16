
/* ------------------------------------------------------------
 * Filename ............... Keyboard.Cpp
 *
 * General Purpose ........ Keyboard class
 * ------------------------------------------------------------
 * First date ............. 12-07-94 11:24am
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
#include "timer.hpp"
#include "setup.hpp"
#include "editor.hpp"
#include "banner.hpp"

/*-------------------------------------------------------------------------*/

KeyboardP TKeyboard = NULL;

/*-------------------------------------------------------------------------*/

/*
 * Routine   : peekKey()
 * Purpose   : get a key
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : keycode or 0 if none
 *
 */

LCL WrdT peekKey()
{
   WrdT ret_val = 0;

#ifdef PEX
   ret_val = BytT(PeekChar());
#else
   if(bioskey(1) != 0)
   {
      ret_val = bioskey(0);
      if((ret_val & 0xFF) != 0)
         ret_val &= 0xFF;
   }
#endif

   return(ret_val);
}

/*
 * Routine   : isExternal()
 * Purpose   : check if the keystroke is from an external source
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if external, False if NOT
 *
 */

LCL FlgT isExternal()
{
   FlgT ret_val = False;

#ifdef PEX
   ret_val = ExternalInput();
#endif

   return(ret_val);
}

/*
 * Routine   : resetInactive()
 * Purpose   : reset the inactivity timer
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

LCL void resetInactive()
{
#ifdef PEX
   ResetInactivity();
   TimeLeft();                // to be sure PB updates the status bar
#endif
}

/*--] Code [-------------------------------------] Keyboard [--------------*/

/*
 * Routine   : Keyboard()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Keyboard::Keyboard()
   : local (8)
   , remote(8)
{
   fromLocal = False;

#ifdef PEX
   InstallHandler(HANDLER_SYSOPKEY, LocalKeyHandler);
#endif
}

/*
 * Routine   : ~Keyboard()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Keyboard::~Keyboard()
{
#ifdef PEX
   RemoveHandler(HANDLER_SYSOPKEY, LocalKeyHandler);
#endif
}

/*
 * Routine   : scan()
 * Purpose   : scan for a key. Local keys are poked in the buffer
 * ------------------------------------------------------------------------
 * Parameters: maximum clockticks to wait
 * Return    : The remote keystroke (0 if none)
 *
 */

WrdT Keyboard::scan(BytT wait)
{
   WrdT ret_val = peekKey();
   if(ret_val != 0 && isExternal() == False)
   {
      local.poke(ret_val);
      ret_val = 0;
   }

   if(wait > 0)
   {
      Timer timer;

      while(ret_val == 0 && timer.elapsed() < wait)
      {
         BytT key = peekKey();
         if(key != 0)
         {
            if(isExternal() == False)
            {
               local.poke(key);
            }
            else
            {
               ret_val = key;
            }
         }
      }
   }

/*
   // --- to release time slices under OS/2!
   if(ret_val == 0)
   {
      union REGS r;
      r.x.ax = 0x1680;
      int86(0x2F, &r, &r);
   }
*/

   return(ret_val);
}

/*
 * Routine   : handle()
 * Purpose   : handle every keystroke (local and remote)
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Keyboard::handle()
{
   BytT key = scan(0);
   if(key != 0)
   {
      if(key == 0x1B)
      {
         key = scan(9);
         if(key == 0x5B)
         {
            key = scan(9);
            switch(key)
            {
               case 0x41   :  remote.poke(KEY_UP  );  break;
               case 0x42   :  remote.poke(KEY_DN  );  break;
               case 0x43   :  remote.poke(KEY_RT  );  break;
               case 0x44   :  remote.poke(KEY_LT  );  break;
               case 0x48   :  remote.poke(KEY_HOME);  break;
               case 0x4B   :  remote.poke(KEY_END );  break;
               default     :
               {
                  remote.poke(0x1B);
                  remote.poke(0x5B);
                  remote.poke(key );
               }
               break;
            }
         }
         else
         {
            remote.poke(0x1B);
            remote.poke(key);
         }
      }
      else
      {
         remote.poke(key);
      }
   }
}

/*
 * Routine   : peek()
 * Purpose   : look at the next available key
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : next avaiable key (0 if none)
 *
 */

WrdT Keyboard::peek()
{
   WrdT ret_val = remote.peek();
   fromLocal = False;
   if(ret_val == 0)
   {
      fromLocal = True;
      ret_val = local.peek();
   }

   if(ret_val == 0)
      handle();

   return(ret_val);
}

/*
 * Routine   : get()
 * Purpose   : get the next avaialble key (wait 'till there's one)
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : next available key
 *
 */

WrdT Keyboard::get()
{
   WrdT ret_val = 0;
   while(ret_val == 0)
   {
      ret_val = remote.get();
      fromLocal = False;
      if(ret_val == 0)
      {
         fromLocal = True;
         ret_val = local.get();
      }

      resetInactive();

      handle();
   }

   return(ret_val);
}

/*
 * Routine   : isEmpty()
 * Purpose   : check if both queue's are empty
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if empty, False if not
 *
 */

FlgT Keyboard::isEmpty()
{
   FlgT ret_val = False;
   if(local.isEmpty() && remote.isEmpty())
      ret_val = True;

   return(ret_val);
}

/*
 * Routine   : purge()
 * Purpose   : purge both local and remote queue's
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Keyboard::purge()
{
   local.purge();
   remote.purge();
}

/*
 * Routine   : poke()
 * Purpose   : poke a key in the local or remote queue
 * ------------------------------------------------------------------------
 * Parameters: key, local (True) or remote (False)
 * Return    : True if OK, False if NOT
 *
 */

FlgT Keyboard::poke(WrdT key, FlgT inLocal)
{
   FlgT ret_val;

   if(inLocal)
      ret_val = local.poke(key);
   else
      ret_val = remote.poke(key);

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
