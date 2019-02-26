
/* ------------------------------------------------------------
 * Filename ............... Editor.Cpp
 *
 * General Purpose ........ Editor class
 * ------------------------------------------------------------
 * First date ............. 12-06-94 03:10pm
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

#include "editor.hpp"
#include "setup.hpp"
#include "keyboard.hpp"
#include "screen.hpp"

/*-------------------------------------------------------------------------*/

EditorP TEditor = NULL;

/*--] Code [-------------------------------------] Editor [----------------*/

/*
 * Routine   : Editor()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: coordinates
 * Return    : None
 *
 */

Editor::Editor(BytT ux, BytT uy, BytT lx, BytT ly, BytT att)
{
   console             = new ConsoleT(ux, uy, lx, ly, att);
   message             = new MessageT(console);
   message->upperY     = 0;
   message->height     = console->height;
   message->width      = console->width;
   message->editable   = True;
   message->attrib     = TSetup->fields[TextWindow]->attr;
   message->qAttrib    = TSetup->fields[TextWindow]->length;

   quote               = new ClipboardT(console);
   quote->attrib       = TSetup->fields[TextWindow]->length;
   quote->upperY       = ((console->height / 3) * 2);
   quote->height       = console->height - quote->upperY;
   quote->isOpen       = False;
   quote->isActive     = False;

   clipboard = NULL;

   if(TScreen->mode != 1)
      TSetup->fastScroll = False;
}

/*
 * Routine   : ~Editor()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Editor::~Editor()
{
   delete message;
   delete quote;
   delete console;

   if(clipboard != NULL)
      delete clipboard;
}

/*
 * Routine   : cutLine()
 * Purpose   : cut a line to the clipboard;
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if changed, False if NOT
 *
 */

FlgT Editor::cutLine()
{
   FlgT ret_val = False;

   if(message->editable)
   {
      ChrP p = message->get(message->curLine());
      if(p != NULL)
      {
         if(strlen(p) != 0)
         {
            if(clipboard == NULL)
               clipboard = new ClipboardT(console);

            clipboard->set(p);

            // --- save memory ! -----------------------------
            if(clipboard->len() > 64)
               clipboard->scroll();
         }
      }

      ret_val = message->cutLine();
   }

   return(ret_val);
}

/*
 * Routine   : pasteLine()
 * Purpose   : paste a line from the clipboard
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if changed, False if NOT
 *
 */

FlgT Editor::pasteLine()
{
   FlgT ret_val = False;

   if(clipboard != NULL)
   {
      if(message->editable)
         if(clipboard->get(buffer) == True)
            ret_val = message->pasteLine(buffer);
   }

   return(ret_val);
}

/*
 * Routine   : update()
 * Purpose   : update the console buffer
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Editor::update()
{
   message->update();

   if(quote->isOpen)
      quote->update();
}

/*
 * Routine   : openQuote()
 * Purpose   : open the quote window
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if OK, False if NOT
 *
 */

FlgT Editor::openQuote()
{
   FlgT ret_val = True;

   if(quote->len() != 0)
   {
      quote  ->isOpen       = True;
      quote  ->needsUpdate  = True;

      message->height       -= (quote->height + 1);
      message->needsUpdate  = True;

      sprintf(buffer, " %s", TSetup->language->get(QuoteHeader));
      console->setLine( quote->upperY - 1, buffer, TSetup->fields[MenuBar]->attr);
   }
   else
   {
      ret_val = False;
   }

   return(ret_val);
}

/*
 * Routine   : closeQuote()
 * Purpose   : close the quote window
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

FlgT Editor::closeQuote()
{
   FlgT ret_val = True;

   if(quote->isOpen)
   {
      quote->isOpen         = False;
      quote->isActive       = False;
      message->needsUpdate  = True;
      message->height       = console->height;
   }
   else
      ret_val = False;

   return(ret_val);
}

/*
 * Routine   : toggleQuote()
 * Purpose   : toggle between quote and message window
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : Always TRUE
 *
 */

FlgT Editor::toggleQuote()
{
   if(quote->len() != 0)
   {
      message->needsUpdate = True;

      if(!quote->isActive)
      {
         if(!quote->isOpen)
            openQuote();

         quote->isActive    = True;
         quote->needsUpdate = True;
      }
      else
         quote->isActive    = False;
   }

   return(True);
}

/*
 * Routine   : cls()
 * Purpose   : clear the WHOLE (!!!) screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Editor::cls()
{
   console->cls();
}

/*
 * Routine   : lowMemory()
 * Purpose   : check if have low memory alarm!
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True for low memory alarm, False for not
 *
 */

FlgT Editor::lowMemory()
{
   return(message->percent() >= 90);
}

/*
 * Routine   : outOfMemory()
 * Purpose   : check if we are out of memory
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    :
 *
 */

FlgT Editor::outOfMemory()
{
   FlgT ret_val = False;
   if(message->percent() >= 99)
   {
      message->editable = False;
      ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : draw()
 * Purpose   : draw the screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Editor::draw()
{
   if(quote->needsUpdate == True || message->needsUpdate == True)
   {
      update();
      console->draw();
   }

   if(quote->isActive)
      quote->drawCursor();
   else
      message->drawCursor();
}

/*
 * Routine   : redraw()
 * Purpose   : redraw the screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Editor::redraw()
{
   if(quote->needsUpdate == True || message->needsUpdate == True)
      update();

   console->redraw();

   if(quote->isActive)
      quote->drawCursor();
   else
      message->drawCursor();
}

/*
 * Routine   : handle()
 * Purpose   : handle a keystroke
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : unhandle key  (0 = no unhandled key)
 *
 */

WrdT Editor::handle()
{
   WrdT ret_val = 0;

   if(!TKeyboard->isEmpty())
   {
      while(!TKeyboard->isEmpty() && ret_val == 0)
      {
         WrdT key = TKeyboard->get();

         if(quote->isActive)
         {
            switch(key)
            {
               // --- ^K -----------------------------------------------------
               case 0x0B         :           closeQuote();              break;
               // --- ^Q -----------------------------------------------------
               case 0x11         :           toggleQuote();             break;
               // --- ENTER --------------------------------------------------
               case KEY_ENT      :
               {
                  if(message->editable)
                  {
                     ChrP p = quote->get(quote->curLine());
                     if(p != NULL)
                     {
                        if(strlen(p) > 0)
                        {
                           message->pasteLine(p);
                           message->down();
                           message->needsUpdate = True;
                           quote  ->down();
                        }
                     }
                  }
               }
               break;

               default           :  ret_val = quote->handle(key);       break;
            }
         }
         else
         {
            switch(key)
            {
               // --- ^K -----------------------------------------------------
               case 0x0B         :
               {
                  if(quote->isOpen)
                     closeQuote();
                  else
                     openQuote();
               }
               break;

               // --- ^Q -----------------------------------------------------
               case 0x11         :           toggleQuote();             break;
               // --- ^U -----------------------------------------------------
               case 0x15         :  message->needsUpdate = pasteLine(); break;
               // --- ^Y -----------------------------------------------------
               case 0x19         :  message->needsUpdate = cutLine();   break;

               default           :  ret_val = message->handle(key);     break;
            }
         }

         draw();
      }
   }
   else
      TKeyboard->peek();

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
