#include <string.h>
#include <tswin.hpp>

static char **ch;
static int maxlen;

static void
selbox_disp(int i,int x,int y)
{
   tsw_mputs(x+1,y,form("%-*.*s",maxlen,maxlen,ch[i]));
}

int
Window::selBox(char **choices,byte battr,int defChoice)
{
   SelectWindow sw;

   for(int i = 0; choices[i] ; i++)
   {
   }

   maxlen--;

   sw.open(minX,minY,maxX,maxY,attr,NOSAVE|NOBORDER);
   sw.define(i,battr,selbox_disp);

   ch     = choices;
   maxlen = maxX - minX;

   sw.show(defChoice);

   bool state = tsw_cursorstate;

   if(state)
      tsw_cursoroff();

   int ret = sw.process();

   if(state)
      tsw_cursoron();

   return ret;
}


int
tsw_selbox( int         x,
            int         y,
            int         maxh,
            byte        attr,
            char      **choices,
            int         mode,
            int         defChoice,
            const char *border,
            ATTR        b_attr,
            char       *title,
            ATTR        t_attr )
{
   Window w;
   int l = 0;

   for(int i = 0; choices[i] ; i++)
   {
      if(strlen(choices[i]) > l)
         l = strlen(choices[i]);
   }

   w.open( x,
           y,
           x + l + 5,
           ( i + 2  >  maxh ) 
               ?  ( y + maxh - 1 )
               :  ( y + i + 1 ),
           attr,
           mode,
           border,
           b_attr,
           title,
           t_attr );

   return w.selBox(choices,0x70,defChoice);
}
