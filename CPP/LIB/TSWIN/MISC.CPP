#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <dos.h>
  #include <time.h>
  #include <string.h>
#endif

#ifdef __OS2__
   #define INCL_DOSPROCESS
   #include <os2.h>
#endif

#include <tswin.hpp>

void
tsw_prependbs(char *s)
{
 int i;

 if(*s == '\\') return;

 for(i = strlen(s) ; i >= 0 ; i--) s[i+1] = s[i];

 *s = '\\';
}

void
tsw_beep(word freq,word duration)
{
 sound(freq);

 #ifdef __OS2__
   DosSleep(duration);
 #else
   #ifdef PB_SDK
      msleep(duration);
   #else
      delay(duration);
   #endif
 #endif

 stopsound();
}

void
tsw_centerline(int line,char *s,ATTR attr)
{
   if(attr)
      tsw_maputs(tsw_hsize/2+1-strlen(s)/2,line,attr,s);
   else
      tsw_mputs (tsw_hsize/2+1-strlen(s)/2,line     ,s);
}
