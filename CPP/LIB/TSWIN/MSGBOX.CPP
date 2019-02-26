#include <stdarg.h>
#include <tswin.hpp>

void
tsw_msgbox(byte attr, char *title , ... )
{
   va_list v;

   va_start(v,title);

   String strings[10];

   int maxlen = 0;

   for(int i=0;i<10;i++)
   {
      char *s = va_arg(v,char*);

      if(!s)
         break;

      strings[i] = s;

      if(strings[i].len() > maxlen)
         maxlen = strings[i].len();
   }

   maxlen--;

   Window w(0,0,maxlen+5,(i*2)+2,attr,EXPLODE|SHADOW|CENTERED_X|CENTERED_Y);
   w.open();

   for( i--; i>=0 ; i--)
      w.centerLine(i*2+2,&strings[i][1],strings[i][0]);

   w.title(title+1,*title);

   KB.get();
}
