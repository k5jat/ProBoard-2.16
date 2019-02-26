
/* ------------------------------------------------------------
 * Filename ............... Language.Cpp
 *
 * General Purpose ........ Language class
 * ------------------------------------------------------------
 * First date ............. 12-19-94 07:09pm
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

#include "language.hpp"

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : Language()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Language::Language()
{
   strings = new ChrP[EndOfLanguage];
   for(IntT cnt = 0; cnt < EndOfLanguage; cnt++)
      strings[cnt] = NULL;
}

/*
 * Routine   : ~Language()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Language::~Language()
{
   if(strings != NULL)
   {
      for(IntT cnt = 0; cnt < EndOfLanguage; cnt++)
         if(strings[cnt] != NULL)
            free(strings[cnt]);

      delete strings;
   }
}

/*
 * Routine   : read
 * Purpose   : read the language file
 * ------------------------------------------------------------------------
 * Parameters: path and language
 * Return    : None
 *
 */

void Language::read(ChrP path, ChrP lang)
{
   FlgT status = False;
   ChrT buffer[256];

   sprintf(buffer,"%s%s.LNG", path, lang);
   FILE *handle = fopen(buffer, "rt");
   if(handle == NULL)
   {
      sprintf(buffer,"%s%s.LNG", path, projectName);
      handle = fopen(buffer, "rt");
   }

   if(handle != NULL)
   {
      IntT counter = 0;
      while(!feof(handle))
      {
         fgets(buffer, 255, handle);
         if(strlen(buffer) > 0 && buffer[0] != ';')
         {
            ChrP token = strtok(buffer,";");
            IntT len = strlen(token);
            for(IntT cnt = len - 1; cnt >= 0; cnt--)
            {
               if(token[cnt] != ' ')
               {
                  token[cnt + 1] = '\0';
                  len = cnt + 1;
                  break;
               }
            }

            if(counter < EndOfLanguage)
            {
               strings[counter++] = strdup(token);
            }
         }
      }

      if(counter == EndOfLanguage)
         status = True;

      fclose(handle);
   }

   if(status == False)
   {
      printf("%s: No (valid) language file found!", projectName);
#ifdef PEX
      Log(LOG_FRIEND, "%s: No (valid) language file found!", projectName);
      exit();
#else
      exit(1);
#endif
   }
}

/*
 * Routine   : get()
 * Purpose   : get the specified language string
 * ------------------------------------------------------------------------
 * Parameters: language string number
 * Return    : string or NULL if undefined
 *
 */

ChrP Language::get(IntT langId)
{
   LCL ChrT buffer[513];
   LCL IntT index = 0;

   ChrP ret_val = NULL;
   if(langId < EndOfLanguage)
   {
      index += 128;
      if(index > (512 - 128) || index < 0)
         index = 0;

      ret_val = buffer + index;

#ifdef PEX
      ParseStringVars(strings[langId], ret_val, 128);
#else
      strcpy(ret_val, strings[langId]);
#endif
   }

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
