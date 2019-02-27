#pragma warn -sig

#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <stdlib.h>
  #include <ctype.h>
#endif

#include <tswin.hpp>

#ifndef max
   #define max(a,b) ( (a)>(b) ? (a) : (b) )
#endif

#ifndef min
   #define min(a,b) ( (a)<(b) ? (a) : (b) )
#endif

int
ViewWindow::process()
{
 int ys,update;

 int x1 = minX;
 int y1 = minY;
 int x2 = maxX;
 int y2 = maxY;

 ys = y2-y1+1;

 if(!end)
   {
   start=0;
   end=min(ys-1,num_entries-1);
   }

 update=1;

 for(;;)
   {
   int i;
   KEY c;

   if(update)
       {
       clear();
       for(i=start;i<=end;i++) (*func)(i,x1,y1+i-start);
       update=0;
       }
   KB.clear();
   c=KB.uget();
   if(c==27) return VL_ESC;
   if(hotkeys)
     for(i=0;hotkeys[i];i++)
       if(c==hotkeys[i])
            {
            hotkey=c;
            return VL_HOTKEY;
            }
   switch(c)
       {
       case KEY_UP: {
 		   if(!start) break;
                    start--; end--;
                    tsw_scroll(DOWN,x1,y1,x2,y2,1,attr);
                    (*func)(start,x1,y1);
                    } break;
       case KEY_DN: {
 		              if(end>=num_entries-1) break;
 		              start++; end++;
                    tsw_scroll(UP,x1,y1,x2,y2,1,attr);
                    (*func)(end,x1,y2);
 		   } break;
        case KEY_PGUP: {
                    if(!start) break;
                    update=1;
                    start-=ys;
                    end-=ys;
                    if(start<0) { start=0; end=start+ys-1; }
 		  } break;
        case KEY_PGDN: {
                    if(end>=num_entries-1) break;
                    update=1;
                    start+=ys;
                    end+=ys;
 		              if(end>=num_entries) { end=num_entries-1; start=end-ys+1; }
                   } break;
        case KEY_HOME: {
                    if(!start) break;
                    start=0;
                    end=ys-1;
                    update=1;
                    } break;
        case KEY_END: {
                    if(end>=num_entries-1) break;
                    end=num_entries-1;
                    start=end-ys+1;
                    update=1;
                    } break;
        }
   }
}

void
ViewWindow::showlast()
{
 end   = num_entries - 1;
 start = end - (maxY-minY);

 if(start < 0) start = 0;
}


ViewWindow::ViewWindow(int num,void(*function)(int,int,int),KEY *hot)
{
 num_entries = num;
 end         = 0;
 hotkeys     = hot;
 func        = function;
}


void
ViewWindow::define(int num,void(*function)(int,int,int),KEY *hot)
{
 num_entries = num;
 end         = 0;
 hotkeys     = hot;
 func        = function;
}

