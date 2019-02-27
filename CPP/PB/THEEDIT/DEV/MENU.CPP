
/* ------------------------------------------------------------
 * Filename ............... Menu.Cpp
 *
 * General Purpose ........ Menu class
 * ------------------------------------------------------------
 * First date ............. 12-20-94 11:31am
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

#include "menu.hpp"
#include "setup.hpp"
#include "screen.hpp"
#include "keyboard.hpp"
#include "editor.hpp"
#include "usestamp.hpp"

/*--] Code [-------------------------------------] Menu [------------------*/

/*
 * Routine   : Menu()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Menu::Menu()
{
   darkAttr =  TSetup->fields[MenuBar]->attr & 0x77;
   highAttr = (TSetup->fields[MenuBar]->attr & 0xF0) | (0x08 | ((TSetup->fields[MenuBar]->attr & 0x70) >> 4));
   normAttr =  TSetup->fields[MenuBar]->attr;
}

/*
 * Routine   : drawItem()
 * Purpose   : draw one menu item
 * ------------------------------------------------------------------------
 * Parameters: item number
 * Return    : hotkey to handle or 0x1B (ESC) if there's no hotkey
 *
 */

ChrT Menu::drawItem(WrdT item)
{
   ChrT ret_val = 0x1B;

   TScreen->color(normAttr);
   TScreen->printf("  ");

   ChrP p = TSetup->language->get(item);
   IntT len = strlen(p);
   for(IntT cnt = 0; cnt < len;)
   {
      ChrT c = p[cnt];
      if(c == '[')
      {
         ret_val = toupper(p[cnt + 1]);

         TScreen->color(darkAttr);
         TScreen->printf("[");
         TScreen->color(highAttr);
         TScreen->printf("%c", ret_val);
         TScreen->color(darkAttr);
         TScreen->printf("]");

         TScreen->color(normAttr);
         cnt += 2;
      }
      else
         TScreen->printf("%c", c);

      cnt++;
   }

   return(ret_val);
}

/*
 * Routine   : get()
 * Purpose   : get the user's choice
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : the event number (-1 is ESCAPE)
 *
 */

IntT Menu::get()
{
   IntT  ret_val = -2;

   BytT    tmpAttr = TScreen->attrib();
   Cursor  tmpCursor;

   TScreen->gotoXY(0, TScreen->height - 1);
   TScreen->color(normAttr);
   TScreen->clrEol();

   ChrT keys[32];
   memset(keys, 0, 32);

   IntT keyCount = 0;
   keys[keyCount++] = drawItem(SaveItem  );
   keys[keyCount++] = drawItem(AbortItem );
   keys[keyCount++] = drawItem(HelpItem  );
   keys[keyCount++] = drawItem(RedrawItem);

   if(TSetup->bpsRate > 0)
      keys[keyCount++] = drawItem(UploadItem);
   else
      keys[keyCount++] = 0x1B;

   if(TSetup->isSysop)
   {
      keys[keyCount++] = drawItem(ImportItem);
      keys[keyCount++] = drawItem(ExportItem);
   }
   else
   {
      keys[keyCount++] = 0x1B;
      keys[keyCount++] = 0x1B;

      if(TEditor->quote->len() != 0)
      {
         if(TEditor->quote->isActive)
         {
            keys[keyCount++] = 0x1B;
            keys[keyCount++] = drawItem(MessageItem);
         }
         else
         {
            keys[keyCount++] = drawItem(QuoteItem);
            keys[keyCount++] = 0x1B;
         }
      }
      else
      {
         keys[keyCount++] = 0x1B;
         keys[keyCount++] = 0x1B;
      }
   }

   while(ret_val == -2)
   {
      WrdT k = toupper(TKeyboard->get());
      if(k == 0x1B || k == 0x0F)            // ESC or ^O
         ret_val = -1;
      else
      {
         for(IntT cnt = 0; cnt < keyCount && ret_val == -2; cnt++)
            if(k == keys[cnt])
               ret_val = cnt + SaveItem;
      }
   }

   TScreen->gotoXY(tmpCursor.x, tmpCursor.y);
   TScreen->color(tmpAttr);

   return(ret_val);
}

/*
 * Routine   : save()
 * Purpose   : save the message
 * ------------------------------------------------------------------------
 * Parameters: filename for the message file
 * Return    : Always TRUE
 *
 */

FlgT Menu::save(ChrP name)
{
   if(TSetup->addTag)
   {
      ChrT tag[32];
      ChrT buffer[80];

      sprintf(tag, "___%s v%s", projectName, versionText);
      if(!TSetup->isRegistered)
      {
         if(TUseStamp->days < 45)
            sprintf(buffer,"%s [unregistered]", tag);
         else
            sprintf(buffer,"%s [unregistered for %d days!]", tag, TUseStamp->days);
      }
      else
         sprintf(buffer, "%s", tag);

      TEditor->message->export(name, buffer);
   }
   else
      TEditor->message->export(name, NULL);

   return(True);
}

/*
 * Routine   : help()
 * Purpose   : display the help screen
 * ------------------------------------------------------------------------
 * Parameters: path
 * Return    : none
 *
 */

FlgT Menu::help(ChrP path)
{
   FlgT ret_val = True;

   ChrT status = 2;

#ifdef PEX
   status = ShowHotkeyFile(form("%s%s.HLP", path, TSetup->userLang), NULL);
   if(status == 2)
      status = ShowHotkeyFile(form("%s%s.HLP", path, projectName), NULL);
#endif

   TScreen->color(0);
   if(status == 2)
   {
      TScreen->centerLine( TScreen->height - 1,
                           TSetup->fields[MenuBar]->attr,
                           "%s %s",
                              TSetup->language->get(SorryNoHelp),
                              TSetup->language->get(PressAnyKey)
                         );

      TKeyboard->get();
      ret_val = False;
   }

   return(ret_val);
}

/*
 * Routine   : upload()
 * Purpose   : let the user upload a message
 * ------------------------------------------------------------------------
 * Parameters: startup path
 * Return    : always TRUE
 *
 */

FlgT Menu::upload(ChrP path)
{
#ifdef PEX
   ChrT logFile[81];
   ChrT fname[81];
   ChrT command[129];

   sprintf(fname  ,"%s%s.MSG", path, projectName);
   sprintf(logFile,"%s%s.LOG", path, projectName);
   unlink(logFile);

   sprintf(command, "/D /F=%s /L=%s", fname, logFile);
   MenuFunction(MENU_UPLOAD, ChrP(command));

   FILE *handle = fopen(logFile, "rt");
   if(handle != NULL)
   {
      fgets(command, 129, handle);
      ChrP p = command + 4;
      for(;*p != ' ' && *p;p++);
         *p = '\0';

      WrdT curPos = TEditor->message->getPos();
      WrdT added  = TEditor->message->import(command + 4);
      if(added != 0)
         TEditor->message->seek(curPos + added);

      unlink(command + 4);

      fclose(handle);
   }
#else
   WrdT curPos = TEditor->message->getPos();
   WrdT added  = TEditor->message->import("M:\TESTFILE.ASC");
   if(added != 0)
      TEditor->message->seek(curPos + added);
#endif

   return(True);
}

/*---------------------------] END OF THE CODE [---------------------------*/
