
/* ------------------------------------------------------------
 * Filename ............... ClpBoard.Cpp
 *
 * General Purpose ........ Clipboard class
 * ------------------------------------------------------------
 * First date ............. 12-12-94 06:12pm
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

#include "clpboard.hpp"
#include "keyboard.hpp"

/*-------------------------------------------------------------------------*/

#define AllocStep 8

/*--] Code [-------------------------------------] Clipboard [-------------*/

/*
 * Routine   : Clipboard()
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: console object
 * Return    : None
 *
 */

Clipboard::Clipboard(ConsoleP con)
{
   lines       = NULL;
   console     = con;

   used        = 0;
   size        = 0;
   topLine     = 0;
   cursorY     = 0;
   upperY      = 0;
   height      = 0;

   isOpen      = False;
   isActive    = False;
   needsUpdate = True;
}

/*
 * Routine   : ~Clipboard()
 * Purpose   : The destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Clipboard::~Clipboard()
{
   release();
}

/*
 * Routine   : release()
 * Purpose   : release the scrap memory
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Clipboard::release()
{
   if(size != 0)
   {
      for(IntT cnt = 0; cnt < size; cnt++)
         if(lines[cnt] != NULL)
            free(lines[cnt]);

      free(lines);
   }
}

/*
 * Routine   : set()
 * Purpose   : set the data for the next available line
 * ------------------------------------------------------------------------
 * Parameters: data
 * Return    : None
 *
 */

void Clipboard::set(ChrP data)
{
   if(used == size)
   {
      if(lines == NULL)
      {
         size = AllocStep;
         lines = ChrI(malloc(size * SIZ(ChrP)));
      }
      else
      {
         size = (((used / AllocStep) + 1) * AllocStep);
         lines = ChrI(realloc(lines, size * SIZ(ChrP)));
      }

      for(IntT cnt = used; cnt < size; cnt++)
      {
         lines[cnt] = NULL;
      }
   }

   if(lines[used] != NULL)
   {
      free(lines[used]);
      lines[used] = NULL;
   }

   if(data != NULL)
   {
      lines[used] = strdup(data);
      used++;
   }
}

/*
 * Routine   : get()
 * Purpose   : get the data for a specific line
 * ------------------------------------------------------------------------
 * Parameters: line number
 * Return    : data or NULL if illegal line number
 *
 */

ChrP Clipboard::get(IntT lineNo)
{
   ChrP ret_val = NULL;
   if(lineNo < used)
      ret_val = lines[lineNo];

   return(ret_val);
}

/*
 * Routine   : get()
 * Purpose   : get (and remove !) the data from the last added line
 * ------------------------------------------------------------------------
 * Parameters: buffer to fill
 * Return    : True if OK, False if NOT
 *
 */

FlgT Clipboard::get(ChrP buffer)
{
   FlgT ret_val = False;
   if(used > 0)
   {
      used--;

      if(lines[used] != NULL)
      {
         strcpy(buffer, lines[used]);
         free(lines[used]);
         lines[used] = NULL;
         ret_val = True;
      }
   }

   return(ret_val);
}

/*
 * Routine   : len()
 * Purpose   : return the number of lines used
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : number of lines used
 *
 */

IntT Clipboard::len()
{
   return(used);
}

/*
 * Routine   : curLine()
 * Purpose   : get the current line number
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : current line number
 *
 */

IntT Clipboard::curLine()
{
   return(topLine + cursorY);
}

/*
 * Routine   : scroll()
 * Purpose   : scroll all lines one up and free up unused memory
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

void Clipboard::scroll()
{
   if(used > 0)
   {
      free(lines[0]);

      for(IntT cnt = 1; cnt < used; cnt++)
         lines[cnt - 1] = lines[cnt];

      used--;
      lines[used] = NULL;
   }
}

/*
 * Routine   : up()
 * Purpose   : move the cursor one line up
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if moved, False if NOT
 *
 */

FlgT Clipboard::up()
{
   FlgT ret_val = True;
   if(cursorY > 0)
      cursorY--;
   else
   {
      if(pageUp() == True)
         cursorY = height - 1;
      else
         ret_val = False;
   }

   return(ret_val);
}

/*
 * Routine   : down()
 * Purpose   : move the cursor one line down
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if moved, False if NOT
 *
 */

FlgT Clipboard::down()
{
   FlgT ret_val = True;
   if(curLine() + 1 < len())
   {
      if(cursorY < height - 1)
      {
         cursorY++;
      }
      else
      {
         if(pageDown() == True)
         {
            cursorY = 0;
         }
         else
         {
            ret_val = False;
         }
      }
   }

   return(ret_val);
}

/*
 * Routine   : pageUp()
 * Purpose   : move the cursor one page up
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if moved, False if NOT
 *
 */

FlgT Clipboard::pageUp()
{
   FlgT ret_val = False;

   IntT cl = curLine();
   if(cl != 0)
   {
      for(IntT cnt = 0; cnt < height; cnt++)
         if(topLine > 0)
            topLine--;
         else
            if(cursorY > 0)
               cursorY--;

      if(cl != curLine())
         ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : pageDown()
 * Purpose   : move the cursor one page down
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if OK, False if NOT
 *
 */

FlgT Clipboard::pageDown()
{
   FlgT ret_val = False;

   IntT cl = curLine();
   if(cl < len() - 1)
   {
      for(IntT cnt = 0; cnt < height; cnt++)
         if(topLine < len() - 1)
            topLine++;

      if(cl != curLine())
         ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : import()
 * Purpose   : import a clipboard file
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if OK, False if NOT
 *
 */

FlgT Clipboard::import(ChrP fileName)
{
   FlgT ret_val = False;

   FILE *handle = fopen(fileName, "rt");
   if(handle != NULL)
   {
      while(!feof(handle))
      {
         ChrT line[81];
         if(fgets(line, 80, handle) != NULL)
            set(line);
      }

      set("");

      fclose(handle);
      ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : update()
 * Purpose   : update the console
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Clipboard::update()
{
   if(isOpen)
   {
      for(IntT cnt = 0; cnt < height; cnt++)
      {
         ChrP p = get(topLine + cnt);
         if(p != NULL)
            console->setLine(upperY + cnt, p, attrib);
         else
            console->setLine(upperY + cnt, "", attrib);
      }
   }

   needsUpdate = False;
}

/*
 * Routine   : drawCursor()
 * Purpose   : draw the cursor at the current location
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Clipboard::drawCursor()
{
   console->gotoXY(0, upperY + cursorY);
}

/*
 * Routine   : handle()
 * Purpose   : handle a keystroke
 * ------------------------------------------------------------------------
 * Parameters: key to handle
 * Return    : unhandled key (or 0)
 *
 */

WrdT Clipboard::handle(WrdT key)
{
   WrdT ret_val = 0;

   switch(key)
   {
      // --- ^C -----------------------------------------------------------
      case 0x03         :  needsUpdate = pageDown        ();         break;
      // --- ^E -----------------------------------------------------------
      case 0x05         :  needsUpdate = up              ();         break;
      // --- ^R -----------------------------------------------------------
      case 0x12         :  needsUpdate = pageUp          ();         break;
      // --- ^X -----------------------------------------------------------
      case 0x18         :  needsUpdate = down            ();         break;

      // --- PGDOWN -------------------------------------------------------
      case KEY_PGDN     :  needsUpdate = pageDown        ();         break;
      // --- PGUP ---------------------------------------------------------
      case KEY_PGUP     :  needsUpdate = pageUp          ();         break;
      // --- UP -----------------------------------------------------------
      case KEY_UP       :  needsUpdate = up              ();         break;
      // --- DOWN ---------------------------------------------------------
      case KEY_DN       :  needsUpdate = down            ();         break;

      default           :
      {
         ret_val = key;
      }
      break;
   }

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
