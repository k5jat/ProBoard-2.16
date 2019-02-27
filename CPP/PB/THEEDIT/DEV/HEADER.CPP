
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

#ifdef PEX
   ChrT status = ShowHotkeyFile(form("%s%s.HED", path, TSetup->userLang), NULL);
   if(status == 2)
      status = ShowHotkeyFile(form("%s%s.HED", path, projectName), NULL);
#endif

   TScreen->color(0);

   for(IntT cnt = 0; cnt < TextWindow; cnt++)
   {
      FormFieldP field = TSetup->fields[cnt];
      if(field->length != 0 && field->text != NULL)
      {
         TScreen->gotoXY(field->x, field->y);
         TScreen->color(field->attr);
         TScreen->printf("%-*.*s", field->length, field->length, field->text);
      }
   }

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
         FormFieldP field = TSetup->fields[Insert];
         free(field->text);
         field->text = strdup(TSetup->insertMode
                                 ? TSetup->language->get(InInsert    )
                                 : TSetup->language->get(InOverwrite )
                             );

         if(field->length != 0)
         {
            TScreen->gotoXY(field->x, field->y);
            TScreen->color(field->attr);
            TScreen->printf("%-*.*s", field->length, field->length, field->text);
         }

         insertMode = TSetup->insertMode;
      }

      if(abbrevMode != TSetup->abbrevMode)
      {
         FormFieldP field = TSetup->fields[Abbrevs];
         free(field->text);
         field->text = strdup(TSetup->abbrevMode
                                 ? TSetup->language->get(InAbbrevs   )
                                 : TSetup->language->get(InNoAbbrevs )
                             );

         if(field->length != 0)
         {
            TScreen->gotoXY(field->x, field->y);
            TScreen->color(field->attr);
            TScreen->printf("%-*.*s", field->length, field->length, field->text);
         }

         abbrevMode = TSetup->abbrevMode;
      }

      if(rawMode != TSetup->rawMode)
      {
         FormFieldP field = TSetup->fields[Raw];
         free(field->text);
         field->text = strdup(TSetup->rawMode
                                 ? TSetup->language->get(InRawMode)
                                 : TSetup->language->get(InNormal )
                             );

         if(field->length != 0)
         {
            TScreen->gotoXY(field->x, field->y);
            TScreen->color(field->attr);
            TScreen->printf("%-*.*s", field->length, field->length, field->text);
         }

         rawMode = TSetup->rawMode;
      }

      TScreen->color(tmpAttr);
      TScreen->gotoXY(tmpCursor.x, tmpCursor.y);
   }
}

/*---------------------------] END OF THE CODE [---------------------------*/
