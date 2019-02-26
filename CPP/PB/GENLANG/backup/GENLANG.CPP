#include <ctype.h>
#include <string.h>
#include <tslib.hpp>
#include <pb_lib.hpp>
#include <deflang.hpp>

typedef _LanguageString LanguageString;

static LanguageFile default_lang_file;
static word         num_lstrings;

void
main()
{
   File f;

   CLEAR_OBJECT(default_lang_file);

   strcpy(default_lang_file.name,"English");
   strcpy(default_lang_file.copyright,"Copyright (c) 1999 TeleGrafix Communications, Inc.  All Rights Reserved");
   default_lang_file.avail = TRUE;

   for( num_lstrings=0 ; default_lstrings[num_lstrings].len ; num_lstrings++ ) {}

   f.open("DEFLANG.PB" , fmode_create , 4096);
   f.write(&default_lang_file,sizeof(LanguageFile));
   f << num_lstrings;

   for(int i=0 ; i<num_lstrings ; i++)
   {
      default_lstrings[i].write(f);
   }
}

