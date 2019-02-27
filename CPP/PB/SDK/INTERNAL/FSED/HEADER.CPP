
/* ------------------------------------------------------------
 * Filename ............... Header.Cpp
 *
 * General Purpose ........ Header class
 * ------------------------------------------------------------
 * First date ............. 12-08-94 10:04pm
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

#include "header.hpp"
#include "cursor.hpp"
#include "setup.hpp"
#include "screen.hpp"

/*-------------------------------------------------------------------------*/

HeaderP THeader = NULL;

/*--] Code [-------------------------------------] Header [----------------*/

/*
 * Routine   : Header()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: path to the PEX directory
 * Return    : None
 *
 */

Header::Header(ChrP p)
{
   insertMode = 2;
   rawMode    = 2;
   abbrevMode = 2;
   path       = p;
}

/*
 * Routine   : redraw()
 * Purpose   : redraw the header
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Header::redraw()
{
   insertMode = 2;
   rawMode    = 2;
   abbrevMode = 2;
   draw();
}

/*
 * Routine   : draw();
 * Purpose   : display the header
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Header::draw()
{
   BytT tmpAttr = TScreen->attrib();
   Cursor tmpCursor;

   printf("\f");
   SetFullColor(0x1F);
   printf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n");
   printf("³   To:                                        ³  Private Msg:                ³\n");
   printf("³ Subj:                                        ³  Typing Mode:                ³\n");
   printf("³ Area:                                        ³ Display Menu:                ³\n");
   printf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n");

   TScreen->color(0);

   TScreen->color(0x1E);
   TScreen->gotoXY(8,1);
   TScreen->printf("%-36.36s",TSetup->fields[To]->text);
   TScreen->gotoXY(8,2);
   TScreen->printf("%-36.36s",TSetup->fields[Subj]->text);
   TScreen->gotoXY(8,3);
   TScreen->printf("%-36.36s",TSetup->fields[Area]->text);
   TScreen->gotoXY(63,1);
   TScreen->printf("%-15.15s",TSetup->fields[PvtMsg]->text);
   TScreen->gotoXY(63,3);
   TScreen->printf("%-15.15s","Esc");

   update();

   TScreen->color(tmpAttr);
   TScreen->gotoXY(tmpCursor.x, tmpCursor.y);
}

/*
 * Routine   : update()
 * Purpose   : update the display for INSERT and RAW mode
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Header::update()
{
   if(    (insertMode != TSetup->insertMode)
       || (abbrevMode != TSetup->abbrevMode)
       || (rawMode    != TSetup->rawMode   )
     )
   {
      BytT tmpAttr = TScreen->attrib();
      Cursor tmpCursor;

      if(insertMode != TSetup->insertMode)
      {
         TScreen->gotoXY(63,2);
         SetFullColor(0x1E);
         TScreen->printf( TSetup->insertMode ? "Insert   ":"Overwrite");

         insertMode = TSetup->insertMode;
      }

      TScreen->color(tmpAttr);
      TScreen->gotoXY(tmpCursor.x, tmpCursor.y);
   }
}

/*---------------------------] END OF THE CODE [---------------------------*/
