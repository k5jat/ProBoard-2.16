#include <dos.h>
#include <string.h>
#include <dir.h>
#include <io.h>
#include <tslib.hpp>

static char *
dellast ( char *text, char chr )
{
   int len = strlen (text) - 1;

   if(text[len] == chr)
      text[len] = '\0';

   return text;
}


bool
MakeFullDir(char *path)
{
   char str[128];
   char *p = path;

   str[0] = '\0';

   strcpy(str,path);

   p = strrchr(str,':');

   if(p)
   {
      *(p+1) = '\0';

      p = strrchr(path,':') + 1;
   }
   else
   {
      str[0] = '\0';

      p = path;
   }

   /* Vanaf hier wijst p naar de pathname zonder drive, en str[] bevat */
   /* de drive (als er een is)                                         */

   if(*p == '\\')
   {
      str[strlen(str)+1] = '\0';
      str[strlen(str)]   = *p++;
   }

   for(;;p++)
   {
      if(*p == '\\' || *p == '\0')
      {
         dellast(str,'\\');

         if(access(str,0))
            if(mkdir(str) < 0)
               return FALSE;
      }

      str[strlen(str)+1] = '\0';
      str[strlen(str)  ] = *p;

      if(*p == '\0') break;
   }

   return TRUE;
}
