#include <string.h>
#include <ctype.h>
#include <tswin.hpp>

void
edit_date(int mode,Window& w,int x,int y,char attr,void *value)
{
 tsw_cursoron();

 w.attrib(attr);
 w.setPos(x,y);
 w << (char *)value;
 w.setPos(x,y);

 if(mode)
   {
    char c;

    switch(KB.getlast())
      {
       case KEY_RT: c=0;
                    break;
       case '0':
       case '1':
       case '2':
       case '3':
       case '4':
       case '5':
       case '6':
       case '7':
       case '8':
       case '9': c = KB.getlast();
                 break;
       default : return;
      }

    int curpos = 1;
    char *s = (char *)value;
    s[2] = s[5] = '-';

    if(c) s[0] = c;

    for(;;)
      {
       w.setPos(x,y);
       w << s;
       w.setPos(x+curpos,y);

       KEY k = KB.get();

       if(curpos < 8 && isdigit((char)k))
         s[curpos++] = (char)k;

       if(k==KEY_RET || k==KEY_DN) return;

       if(k==KEY_RT && curpos<7) curpos++;

       if((k==KEY_LT || k==8) && curpos>0)
         {
          curpos--;
          if(curpos==2 || curpos==5) curpos--;
         }

       if(curpos==2 || curpos==5) curpos++;
      }
   }
}



main()
{
 char date[20];
 char normstr[20];

 strcpy(date,"00-00-00");
 strcpy(normstr,"");

 static Field frm[] = {
                { FRM_FUNCTION   , date    , 0, edit_date , FRM_ANYKEY , 8 , 9  , 18,2 },
                { FRM_STRING     , normstr , 0, 0         , 0          , 8 , 9  , 18,4 }
               };

 FormWindow w(10,6,40,16,0x3F,EXPLODE);
 w.open();
 w << "\nDate          :\n"
      "\nNormal string :";

 w.define(frm,2,0x3E);
 w.process();

 return 0;
}

