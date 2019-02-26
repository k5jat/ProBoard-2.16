#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <string.h>
  #include <stdarg.h>
  #include <process.h>
  #include <stdlib.h>
  #include <stdio.h>
#endif

#include <tslib.hpp>

#ifndef PEX
char *
form(const char *s ...)
{
   static int index = 0;
   static char buffer[1024];
   va_list v;

   if(index >= 512) index = 0;

   va_start(v,s);

   char *ret = &buffer[index];

   index += vsprintf(&buffer[index],s,v) + 1;

   if(index >= 1024)
   {
      _exit(EXIT_FAILURE);
   }

 return ret;
}
#endif

