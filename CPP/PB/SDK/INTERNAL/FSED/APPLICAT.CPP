
/* ------------------------------------------------------------
 * Filename ............... Applicat.Cpp
 *
 * General Purpose ........ Application class
 * ------------------------------------------------------------
 * First date ............. 12-16-94 03:16pm
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

#include "abbrev.hpp"
#include "applicat.hpp"
#include "banner.hpp"
#include "editor.hpp"
#include "header.hpp"
#include "keyboard.hpp"
#include "menu.hpp"
#include "screen.hpp"
#include "setup.hpp"
#include "timer.hpp"
#include "usestamp.hpp"
#include "videoram.hpp"

/*-------------------------------------------------------------------------*/

ApplicationP TApp = NULL;

/*--] Code [-------------------------------------] Application [-----------*/

/*
 * Routine   : Application()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Application::Application(IntT argc, ChrP argv[])
{
#ifdef PEX
   strcpy(startPath, StartupPath);
#endif

#ifndef PEX
   strcpy(startPath, ".\\");
#endif

   redrawScreen   = True;
   redrawBanner   = True;

   // --- find the startup path -------------------------------------------
   strcpy(pexPath, argv[0]);
   IntT max = strlen(pexPath);
   for(IntT cnt = max - 1; cnt >= 0; cnt--)
      if(pexPath[cnt] == '\\')
      {
         pexPath[cnt + 1] = '\0';
         break;
      }

   // --- process command parameters --------------------------------------
   deleteSource = True;
   sprintf(loadName,"%sMSGTMP", startPath);
   sprintf(saveName,"%sMSGTMP", startPath);

   if(argc > 1)
   {
      for(IntT cnt = 1; cnt < argc; cnt++)
      {
         if(argv[cnt][0] == '-' && toupper(argv[cnt][1]) == 'L')
         {
            strcpy(loadName, argv[cnt + 1]);
            deleteSource = False;
         }

         if(argv[cnt][0] == '-' && toupper(argv[cnt][1]) == 'S')
            strcpy(saveName, argv[cnt + 1]);
      }
   }

   TUseStamp = new UseStampT(pexPath, projectName);
   TSetup    = new SetupT   ;  TSetup->read(pexPath, projectName);
   TKeyboard = new KeyboardT;
   TScreen   = new ScreenT  ;
   THeader   = new HeaderT(pexPath);
   TBanner   = new BannerT  ;
   TEditor   = new EditorT(0,
                           TSetup->fields[TextWindow]->y,
                           vrHorSize - 2,
                           TScreen->height - 2,
                           TSetup->fields[TextWindow]->attr
                          );
}

/*
 * Routine   : ~Application()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

Application::~Application()
{
   if(TScreen != NULL)
   {
      TScreen->color(Color(White|HiLight, Black));
      TScreen->cls();
   }

   if(TUseStamp != NULL) delete TUseStamp;
   if(TSetup    != NULL) delete TSetup   ;
   if(TKeyboard != NULL) delete TKeyboard;
   if(THeader   != NULL) delete THeader  ;
   if(TEditor   != NULL) delete TEditor  ;
   if(TBanner   != NULL) delete TBanner  ;
   if(TScreen   != NULL) delete TScreen  ;
}

/*
 * Routine   : askFileName()
 * Purpose   : ask for a IMPORT/EXPORT/UPLOAD filename
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if OK, False if NOT
 *
 */

FlgT Application::askFileName()
{
   FlgT ret_val = True;

   strcpy(buffer,"TESTFILE.ASC");

   return(ret_val);
}

/*
 * Routine   : run()
 * Purpose   : the run loop
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

void Application::run()
{
   ChrP pokePtr = NULL;

   //phl//AbbrevList * abbrevs = new AbbrevList;
   //phl//abbrevs->read(pexPath, TSetup->userLang);
   //phl//TEditor->message->setAbbrev(abbrevs);

   TScreen->color(TSetup->fields[TextWindow]->attr);
   TScreen->cls();

   FlgT isQuoteHandled = False;
   FlgT loop = True;
   while(loop)
   {
      if(redrawScreen == True)
      {
         TScreen->color(0);
         TScreen->color(TSetup->fields[TextWindow]->attr);
         TScreen->cls();

         THeader->redraw();
         TEditor->redraw();

         redrawScreen = False;
         redrawBanner = True;
      }

      if(isQuoteHandled == False)
      {
         handleQuote();
         isQuoteHandled = True;
      }

      if(redrawBanner == True)
      {
         TBanner->redraw();
         redrawBanner = False;
      }

      if(!TSetup->stopBanner)
         TBanner->update();

      for(;pokePtr != NULL;)
      {
         FlgT pokeStat;

         ChrT pokeChar = *(pokePtr);
         if(pokeChar == '\n')
            pokeStat = TKeyboard->poke(KEY_ENT, True);
         else
            pokeStat = TKeyboard->poke(pokeChar, True);

         if(pokeStat == True)
         {
            pokePtr++;

            if(pokeChar == 0)
               pokePtr = NULL;
         }
         else
            break;
      }

      THeader->update();

      WrdT key = TEditor->handle();
      if(key != 0)
      {
         IntT import = -1;

         switch(key)
         {
            // --- ALT-Fx -------------------------------------------------
            case KEY_AF1      :  import = 0;       break;
            case KEY_AF2      :  import = 1;       break;
            case KEY_AF3      :  import = 2;       break;
            case KEY_AF4      :  import = 3;       break;
            case KEY_AF5      :  import = 4;       break;
            case KEY_AF6      :  import = 5;       break;
            case KEY_AF7      :  import = 6;       break;
            case KEY_AF8      :  import = 7;       break;
            case KEY_AF9      :  import = 8;       break;
            case KEY_AF10     :  import = 9;       break;

            // --- ALT-B --------------------------------------------------
            case KEY_ALTB:
            {
               TBanner->toggle();
            }
            break;

            // --- ALT-P --------------------------------------------------
            case KEY_ALTP:
            {
               if(TSetup->ptrPort != NULL)
                  TEditor->message->export(TSetup->ptrPort,"");
            }
            break;

            // --- ^E -----------------------------------------------------
            case 0x05         :
            {
               //phl//if(abbrevs != NULL)
               //phl//{
               //phl//   redrawScreen = abbrevs->list();
               //phl//}
            }
            break;

            // --- ^L -----------------------------------------------------
            case 0x0C:  redrawScreen    = True;    break;

            // --- ^N -----------------------------------------------------
            case 0x0E:
            {
               MenuP menu   = new Menu;
               redrawScreen = menu->help(pexPath);
               redrawBanner = True;
               delete menu;
            }
            break;

            // --- ^Z -----------------------------------------------------
            case 0x1A:
            {
               MenuP menu = new Menu;
               menu->save(saveName);
               delete menu;
               loop = False;
            }
            break;

            // --- ^O or ESC ----------------------------------------------
            case 0x0F:
            case 0x1B:
            {
               MenuP menu = new Menu;
               IntT choice = menu->get();

               switch(choice)
               {
                  case SaveItem     :
                  {
                     menu->save(saveName);
                     loop = False;
                  }
                  break;

                  case AbortItem    :
                  {
                     loop = False;
                  }
                  break;

                  case HelpItem     :
                  {
                     redrawScreen = menu->help(pexPath);
                     redrawBanner = True;
                  }
                  break;

                  case RedrawItem   :
                  {
                     redrawScreen = True;
                  }
                  break;

                  case UploadItem   :
                  {
                     redrawScreen = menu->upload(startPath);
                  }
                  break;

                  case ImportItem   :
                  {
                  //   if(askFileName() == True)
                  //   {
                  //      TEditor->message->import(buffer);
                  //      TEditor->draw();
                  //   }
                  }
                  break;

                  case ExportItem   :
                  {
                  //   if(askFileName() == True)
                  //      TEditor->message->export(buffer);
                  }
                  break;

                  case QuoteItem    :
                  case MessageItem  :
                  {
                     TEditor->toggleQuote();
                     TEditor->draw();
                  }
                  break;
               }

               redrawBanner = True;
               delete menu;
            }
            break;
         }

         if(import != -1)
         {
            ChrP fn = TSetup->importFiles[import];
            if(fn != NULL)
            {
               if(import < 3 || TSetup->isRegistered)
               {
                  TEditor->message->import(fn);
                  TEditor->draw();
               }
               else
                  TBanner->unregistered();
            }
            else
            {
#ifdef PEX
               ChrP p = ChrP(Config->sysopkeys[import]);
               if(p[0] == '`')
               {
                  ParseStringVars(p + 1, pokeStr, 255);
                  pokePtr = pokeStr;
               }
#endif
            }
         }

      }
   }

   //phl//delete abbrevs;

   showEnding();
}

/*
 * Routine   : handleQuote()
 * Purpose   : load the quote file if there's one
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Application::handleQuote()
{
   FlgT importQuote = True;

   if(TSetup->fullQuote)
   {
      struct find_t find;

#ifndef PEX
#  define dos_findfirst(a,b,c) _dos_findfirst(a,b,c)
#endif

      if(dos_findfirst(loadName, 0, &find) == 0)
      {
         if(deleteSource == True)   // if not a command-line forced import...
         {
            ChrT yesKey = *(TSetup->language->get(YesKey));
            ChrT noKey  = *(TSetup->language->get(NoKey ));

            BytT tmpAttr = TScreen->attrib();

            TScreen->gotoXY(0, TScreen->height - 1);
            TScreen->color(TSetup->fields[MenuBar]->attr);
            TScreen->clrEol();
            TScreen->printf("  %s [%c/%c]",
                              TSetup->language->get(WannaLoadQuote),
                              toupper(yesKey), tolower(noKey)
                           );

            for(;;)
            {
               ChrT key = TKeyboard->get();
               if(toupper(key) == toupper(yesKey) || key == KEY_ENT)
                  break;

               if(toupper(key) == toupper(noKey)  || key == KEY_ESC)
               {
                  importQuote = False;
                  break;
               }
            }

            TScreen->color(tmpAttr);
         }
      }
      else
         importQuote = False;

      if(importQuote == False)
      {
         if(dos_findfirst("TheEdit.Tmp", 0, &find) == 0)
         {
            TEditor->message->import("TheEdit.Tmp");
            unlink("TheEdit.Tmp");
         }
      }
   }
   else
      importQuote = False;

   if(importQuote)
      TEditor->message->import(loadName);
   else
   {
      TEditor->quote->import(loadName);
      TEditor->openQuote();
   }


   TEditor->draw();

   if(deleteSource)
      unlink(loadName);

   redrawBanner = True;
}

/*
 * Routine   : showEnding()
 * Purpose   : show the ending screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

void Application::showEnding()
{
}

/*---------------------------] END OF THE CODE [---------------------------*/
