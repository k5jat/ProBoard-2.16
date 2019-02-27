
/* ------------------------------------------------------------
 * Filename ............... Abbrev.Cpp
 *
 * General Purpose ........ Abbrev class
 * ------------------------------------------------------------
 * First in version ....... 1.00
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
#include "setup.hpp"
#include "banner.hpp"
#include "keyboard.hpp"

/*--] Code [-------------------------------------] AbbrevEntry [-----------*/

/*
 * Routine   : AbbrevEntry()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

AbbrevEntry::AbbrevEntry()
{
   abbrev  = NULL;
   replace = NULL;
   next    = NULL;
}

/*
 * Routine   : ~AbbrevEntry()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

AbbrevEntry::~AbbrevEntry()
{
   if(abbrev != NULL)
      free(abbrev);

   if(replace != NULL)
      free(replace);
}

/*
 * Routine   : set()
 * Purpose   : set the value of the entry
 * ------------------------------------------------------------------------
 * Parameters: abbrev and replace string
 * Return    : None
 *
 */

void AbbrevEntry::set(ChrP a, ChrP r)
{
   if(a != NULL)
      abbrev = strdup(a);

   if(r != NULL)
      replace = strdup(r);
}

/*
 * Routine   : check()
 * Purpose   : check if we got the right abbreviation.
 * ------------------------------------------------------------------------
 * Parameters: abbreviation
 * Return    : replace string if ok, NULL if NOT
 *
 */

ChrP AbbrevEntry::check(ChrP a)
{
   ChrP ret_val = NULL;

   if(stricmp(abbrev, a) == 0)
      ret_val = replace;

   return(ret_val);
}

/*--] Code [-------------------------------------] AbbrevList [------------*/

/*
 * Routine   : AbbrevList()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

AbbrevList::AbbrevList()
{
   first = NULL;
   last = NULL;
}

/*
 * Routine   : ~AbbrevList()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

AbbrevList::~AbbrevList()
{
   AbbrevEntry * entry = first;
   while(entry != NULL)
   {
      AbbrevEntry * next = entry->next;
      delete entry;
      entry = next;
   }

   first = NULL;
}

/*
 * Routine   : read()
 * Purpose   : read the list
 * ------------------------------------------------------------------------
 * Parameters: path, language
 * Return    : None
 *
 */

void AbbrevList::read(ChrP path, ChrP lang)
{
   ChrT buffer[256];

   sprintf(buffer,"%s%s.ABR", path, lang);
   FILE *handle = fopen(buffer, "rt");
   if(handle == NULL)
   {
      sprintf(buffer,"%s%s.ABR", path, projectName);
      handle = fopen(buffer, "rt");
   }

   if(handle != NULL)
   {
      AbbrevEntry * entry = NULL;

      while(!feof(handle))
      {
         if(fgets(buffer, 255, handle) == NULL)
            break;

         int lastPos = strlen(buffer) - 1;
         if(buffer[lastPos] == '\n')
            buffer[lastPos] = '\0';

         if( (buffer[0] != '\0') && (buffer[0] != ';') )
         {
            if(entry == NULL)
            {
               entry = new AbbrevEntry;

               if(first == NULL)
                  first = entry;
               else
                  last->next = entry;

               last = entry;

               entry->set(buffer, NULL);
            }
            else
            {
               entry->set(NULL, buffer);
               entry = NULL;
            }
         }
      }

      fclose(handle);
   }
}

/*
 * Routine   : get()
 * Purpose   : get the replace string for the specified abbreviation
 * ------------------------------------------------------------------------
 * Parameters: abbreviation
 * Return    : replace string or NULL
 *
 */

ChrP AbbrevList::get(ChrP abbrev)
{
   ChrP ret_val = NULL;

   AbbrevEntry * entry = first;
   while(entry != NULL && ret_val == NULL)
   {
      ret_val = entry->check(abbrev);
      entry = entry->next;
   }

   return(ret_val);
}

/*
 * Routine   : list()
 * Purpose   : list the abbreviations
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if screen is cleared, False if NOT
 *
 */

FlgT AbbrevList::list()
{
   FlgT ret_val = True;

   if(first != NULL)
   {
      ChrP title = TSetup->language->get(AbbrevTitle);
      ChrT underline[81] = "컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴";
      underline[strlen(title)] = '\0';

      InitLineCounter();
      printf("\f%c%s\n%c%s\n\n", Brown, title, White, underline);

      LineCounter();
      LineCounter();
      LineCounter();

      AbbrevEntry * entry = first;
      while(entry != NULL)
      {
         printf("%c%-*.*s%c%-*.*s\n",
                     Cyan, 12, 12, entry->abbrev,
                     Magenta, 65, 65, entry->replace
               );

         if(LineCounter() == False)
            break;

         entry = entry->next;
      }

      printf("%c\n%s\n", White, TSetup->language->get(PressAnyKey));
      TKeyboard->get();

   }
   else
   {
      TBanner->putS(TSetup->language->get(NoAbbrevsFound));
      ret_val = False;
   }

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
