#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <string.h>
  #include <ctype.h>
  #include <stdlib.h>
#endif

#include <tswin.hpp>

int
Window::scan(char *string,unsigned count,unsigned w,byte mode)
{
 return lowlevel_scanstring(string,count,mode,w);
}

int
Window::scan(long& v,unsigned l,unsigned w,byte mode)
{
 char s[80];
 int ret,x,y;

 strcpy(s,form("%ld",v));

 ret = scan(s,l,w,byte((mode & 0xF8) | SCAN_NUM));

 v = (s[0]) ? atol(s) : 0;

 getPos(x,y);

 (*this) << form("%-*ld",l,v);

 setPos(x,y);

 return ret;
}

int
Window::scan(int& v,unsigned l,unsigned w,byte mode)
{
 long temp = v;

 int ret = scan(temp,l,w,byte(mode & 0xF8));

 if(temp >= -32768L && temp <= 32767L) v = int(temp);

 return ret;
}

int
Window::scan(unsigned& v,unsigned l,unsigned w,byte mode)
{
 long temp = v;

 int ret = scan(temp,l,w,byte(mode & 0xF8));

 if(temp >= 0 && temp <= 0xFFFF) v = unsigned(temp);

 return ret;
}

int
Window::lowlevel_scanstring(char *string,unsigned count,byte mode,unsigned width)
{
 int len,current,start;
 int x,y,i;
 ATTR attr,oldattr;
 bool moved;
 KEY c;
 int ret;

 if(!width) width = count;

 int x1 = minX;
 int y1 = minY;

 getPos(x,y);

 attr = oldattr = attrib();

 if(fieldAttr) attr = fieldAttr;

 len     = strlen(string);
 current = start
         = 0;

 moved = FALSE;

 for(;;)
   {
    for(i=0 ; i<len && i<width ; i++) direct(x+i,y,attr,string[start+i]);

    for(; i<width ; i++) direct(x+i,y,attr,(mode & SCAN_FIELD)?'°':' ');

    setPos(x + current - start , y);

    switch(mode & 0x7)
      {
       case SCAN_ALL   : c = KB.get();
                         break;
       case SCAN_UPPER : c = KB.uget();
                         break;
       case SCAN_NUM   : {
                          c = KB.get();
                          if(strchr("0123456789.-+\r\x08\x09\x1b",(char)(c%256)) || c>256)
                            {
                             break;
                            }
                           else
                            {
                             tsw_beep(1000,50);
                             continue;
                            }
                        }
      }

    if(!strchr("\r\t\b\x1b",c) && c < 256)
      {
       if(!moved)
         {
          current = len = 0;

          string[0] = '\0';
         }

       if(len >= count)
         {
          tsw_beep(2000,100);
          continue;
         }

       if(current == len)
         {
          current++;

          if(current >= start + width) start++;

          string[len++] = char(c);
          string[len  ] = '\0';
         }
        else
         {
          memmove(&string[current+1],&string[current],len-current+1);

          string[current++] = char(c);

          if(current >= start + width) start++;

          len++;
         }
      }

    moved = TRUE;

    scanHotKey=0;

    if(scanHotKeys)
      {
       for(int i=0;scanHotKeys[i];i++) if(scanHotKeys[i]==c)
         {
          scanHotKey = c;

          ret = SF_HOT;

          goto escape;
         }
      }

    switch(c)
      {
       case KEY_RET  : ret = SF_RETURN;
                       goto escape;
       case KEY_ESC  : ret=SF_ESC;
                       goto escape;
       case KEY_STAB :
       case KEY_UP   : ret=SF_UP;
                       goto escape;
       case KEY_TAB  :
       case KEY_DN   : ret=SF_DOWN;
                       goto escape;
       case 8        : {
                        if(!current) continue;

                        if(len == current)
                          {
                           current--;

                           if(current < start) start = current;

                           string[--len] = '\0';

                           continue;
                          }

                        memmove(&string[current-1],&string[current],len - current + 2);
                        len--;
                        current--;
                       }
                       break;

       case KEY_LT   : if(current > 0)
                         {
                          current--;
                          if(current < start) start = current;
                         }
                       break;

       case KEY_RT   : if(current < len)
                         {
                          current++;
                          if(current >= width + start) start++;
                         }
                       break;

       case KEY_DEL  : if(len == current) continue;
                       memmove(&string[current],&string[current+1],len-current+1);
                       len--;
                       break;

       case KEY_END  : current=len;
                       if(current >= width+start) start = len-width+1;
                       break;

       case KEY_HOME : current = start = 0;
                       break;
      }
   }

escape:

 for(i=0 ; i<len && i<width ; i++) direct(x+i,y,attr,string[i]);

 for(;i<width;i++) direct(x+i,y,attr,(mode & SCAN_FIELD)?'°':' ');

 setPos(x,y);

 if(fieldAttr) tsw_selbar(y+y1-1,x+x1-1,x+x1+width-2,oldattr);

 return ret;
}

int
Window::scan(Date& date,char *fmt)
//edit_date(Window& w, int x , int y, ATTR attr, KEY *hotkeys , Date& date , char *fmt)
{
   ATTR attr,oldattr;
   int x,y;
   int ret;
   char s[9];
   int fmt_ar[3];

    int x1 = minX;
    int y1 = minY;

   if(toupper(fmt[0] == 'D')) fmt_ar[0] = 0;
   if(toupper(fmt[0] == 'M')) fmt_ar[0] = 1;
   if(toupper(fmt[0] == 'Y')) fmt_ar[0] = 2;
   if(toupper(fmt[3] == 'D')) fmt_ar[1] = 0;
   if(toupper(fmt[3] == 'M')) fmt_ar[1] = 1;
   if(toupper(fmt[3] == 'Y')) fmt_ar[1] = 2;
   if(toupper(fmt[6] == 'D')) fmt_ar[2] = 0;
   if(toupper(fmt[6] == 'M')) fmt_ar[2] = 1;
   if(toupper(fmt[6] == 'Y')) fmt_ar[2] = 2;

   strncpy(s,date.format(fmt),8);
   s[8] = '\0';

   getPos(x,y);

   attr = oldattr = attrib();

   if(fieldAttr)
      attr = fieldAttr;

   attrib(attr);
   (*this) << (char *)s;
   setPos(x,y);

   int curpos = 0;
   KEY k;

   for(;;)
   {
      setPos(x,y);
      (*this) << s;
      setPos(x+curpos,y);

      k = KB.get();

      if(scanHotKeys)
      {
         for(int j=0;scanHotKeys[j];j++)
            if(k==scanHotKeys[j])
               break;

         if(k == scanHotKeys[j]) break;
      }

      if(curpos < 8 && isdigit((char)k))
      s[curpos++] = (char)k;

      if(curpos >= 8)
         curpos = 7;

      if(   k==KEY_RET
         || k==KEY_DN
         || k==KEY_UP
         || k==KEY_TAB
         || k==KEY_STAB
         || k==KEY_ESC
         ) break;



      if(k==KEY_RT && curpos<7) curpos++;

      if((k==KEY_LT || k==8) && curpos>0)
      {
         curpos--;
         if(curpos==2 || curpos==5) curpos--;
      }

      if(curpos==2 || curpos==5) curpos++;
   }

   date[fmt_ar[0]] = byte(atoi(strtok(s,"-/")));
   date[fmt_ar[1]] = byte(atoi(strtok(NULL,"-/")));
   date[fmt_ar[2]] = byte(atoi(strtok(NULL,"-/")));

   if(scanHotKeys)
      for(int j=0;scanHotKeys[j];j++)
         if(k==scanHotKeys[j])
         {
            scanHotKey = k;
            ret = SF_HOT;
            goto escape;
         }

   switch(k)
      {
      case KEY_STAB:
      case KEY_UP  : ret = SF_UP;
                     goto escape;
      case KEY_TAB :
      case KEY_RET :
      case KEY_DN  : ret = SF_DOWN;
                     goto escape;
      default      : ret = SF_ESC;
                     goto escape;
      }

   escape:

   attrib(oldattr);
   if(fieldAttr)
      tsw_selbar(y+y1-1,x+x1-1,x+x1+6,oldattr);

   return ret;
}

