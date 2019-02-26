#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <tslib.hpp>
#include "pb_lib.hpp"

_LanguageString::_LanguageString()
{
   CLEAR_OBJECT(*this);

   s        = new char[1];
   hotkeys  = new char[1];

   *s       = '\0';
   *hotkeys = '\0';
}

_LanguageString::_LanguageString(char *string , char *hk , byte c , byte hl , byte f , byte p)
{
   CLEAR_OBJECT(*this);

   if(string)
      len = strlen(string);

   s   = new char[len + 1];

   if(string)
      strcpy( s , string );
   else
      *s = '\0';

   if(hk)
      numHotkeys = strlen(hk);

   hotkeys = new char[numHotkeys + 1];

   if(hk)
      strcpy( hotkeys , hk );
   else
      *hotkeys = '\0';

   color     = c;
   highlight = hl;

   flags       = f;
   promptcolor = p;
}

_LanguageString::~_LanguageString()
{
   delete [] s;
   delete [] hotkeys;
}

void
_LanguageString::clear()
{
   delete [] s;
   delete [] hotkeys;

   CLEAR_OBJECT( *this );

   s        = new char[1];
   hotkeys  = new char[1];

   *s       = '\0';
   *hotkeys = '\0';
}

void
_LanguageString::operator=( const _LanguageString& lang )
{
   color       = lang.color;
   highlight   = lang.highlight;
   promptcolor = lang.promptcolor;

   flags       = lang.flags;

   len         = lang.len;
   numHotkeys  = lang.numHotkeys;

   delete [] s;
   s = new char[len + 1];
   strcpy(s,lang.s);

   delete [] hotkeys;
   hotkeys = new char[numHotkeys+1];
   strcpy(hotkeys,lang.hotkeys);
}

_LanguageString::_LanguageString( const _LanguageString& lang )
{
   CLEAR_OBJECT(*this);

   color       = lang.color;
   highlight   = lang.highlight;
   promptcolor = lang.promptcolor;

   len         = lang.len;
   numHotkeys  = lang.numHotkeys;
   flags       = lang.flags;

   s = new char[len + 1];
   strcpy(s,lang.s);

   hotkeys = new char[numHotkeys + 1];
   strcpy(hotkeys,lang.hotkeys);
}

bool
_LanguageString::read( File& f)
{
   char tmp_buf[8];

   if(f.read(tmp_buf,8) != 8) return FALSE;

   delete [] s;
   delete [] hotkeys;

   memcpy(this,tmp_buf,8);

   s = new char[len + 1];

   if(len)
      f.read(s,len);

   s[len] = '\0';

   hotkeys = new char[numHotkeys+1];

   if(numHotkeys)
      f.read(hotkeys,numHotkeys);

   hotkeys[numHotkeys] = '\0';

   return TRUE;
}

bool
_LanguageString::write( File& f )
{
   f.write(this,8);

   if(len)
      f.write(s,len);

   if(numHotkeys)
      f.write(hotkeys,numHotkeys);

   return TRUE;
}


String
parse_lang(char *fmt , ...)
{
   char *valid_chars = "-1234567890.";
   String result;

   if(!strchr(fmt,'%'))
   {
      result = fmt;
   }
   else
   {
      char *s;
      int param_count;
      bool in_par;

      for( s=fmt , param_count=0 , in_par = FALSE; *s ; s++ )
      {
         if(in_par)
         {
            if(islower(*s))
            {
               param_count = *s - 'a' + 1;
            }

            if(!strchr(valid_chars,*s))
            {
               in_par = FALSE;
            }
         }

         if(*s == '%') in_par = TRUE;
      }

      static char *param[26];
      String fmt_str;

      va_list arglist;
      va_start(arglist,fmt);

      for(int i=0; i <param_count ; i++)
         param[i] = va_arg(arglist,char *);

      for(s = fmt , in_par = FALSE; *s ; s++)
      {
         if(in_par)
         {
            if(strchr(valid_chars,*s))
            {
               fmt_str << *s;
            }
            else
            {
               if(islower(*s))
               {
                  fmt_str << 's';
                  result  << form(fmt_str , param[*s - 'a']);
                  in_par = FALSE;
               }
               else
               {
                  result << fmt_str << *s;
                  in_par = FALSE;
               }
            }
         }
         else
         {
            if(*s == '%')
            {
               in_par = TRUE;
               fmt_str = '%';
            }
            else
            {
               result << *s;
            }
         }
      }
   }

   return result;
}
