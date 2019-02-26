#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <string.h>
  #include <stdio.h>
#endif

#include <tswin.hpp>

void
Window::gauge(int x,int y,ATTR a,int len,dword val,dword max,bool rewrite)
{
 char s[200];

 memset(s,'А',len);
 s[len] = '\0';

 if(rewrite) direct(x,y,a,s);

 sprintf(s,"%%-.%lds",val*len/max);

 direct(x,y,a,form(s,"ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл"));
}
