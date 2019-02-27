#pragma warn -sig

#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <stdlib.h>
  #include <string.h>
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
SelectWindow::process()
{
   int ys,update=1;
   int previous=current;

   int x1=minX;
   int y1=minY;
   int x2=maxX;
   int y2=maxY;

   ys=y2-y1+1;

   if(!end)
   {
      start = 0;
      end   = ys-1;

      if(end>=num_entries) end=num_entries-1;
      current=previous=0;
   }

   for(;;)
   {
      int i;
      unsigned c;

      if(update)
      {
         if(emptyfunc)
            for(i=0;i<ys;i++) (*emptyfunc)(x1,y1+i);
         else
            clear();
      }

      if(update && num_entries)
      {
         for(i=start;i<=end;i++)
         {
            (*func)(i,x1,y1+i-start);

            if(selected && selected[i])
               tsw_selbar(y1+i-start,x1,x2,selAttr);
         }
      }

      setPos(1,current-start+1);
      if((current!=previous || update) && num_entries)
      {
         tsw_selbar(previous+y1-start,x1,x2,attr);

         (*func)(previous,x1,previous+y1-start);

         if(selected && selected[previous])
            tsw_selbar(previous+y1-start,x1,x2,selAttr);

         tsw_selbar(current+y1-start,x1,x2,bar_attr|((selected && selected[current])?0x80:0));

         previous=current;

         update=0;
      }

      if(action!=NULLFUNC && num_entries) (*action)(current);

      KB.clear();
      c = KB.uget();

      if(c==KEY_ESC) return SL_ESC;
      if(c==KEY_RET && num_entries)
      {
         if(selected)
         {
            selected[current] = !selected[current];
            c = KEY_DN;
         }
         else
         {
            return current;
         }
      }

      if(c==32 && num_entries)
         if(selected)
         {
            selected[current] = !selected[current];
            update=1;
         }

      if(hotkeys!=NULL)
         for(i=0;hotkeys[i];i++)
            if(c==hotkeys[i])
            {
               hotkey=c;
               return SL_HOTKEY;
            }

      if(!num_entries)
         continue;

      switch(c)
      {
         case KEY_UP:
            {
               if(!current)
                  break;

               previous = current--;

               tsw_selbar(previous+y1-start,x1,x2,attr);

               (*func)(previous,x1,previous+y1-start);

               if(selected && selected[previous]) tsw_selbar(previous+y1-start,x1,x2,selAttr);

               if(current<start)
               {
                  start--;
                  end--;

                  tsw_scroll(DOWN,x1,y1,x2,y2,1,attr);

                  (*func)(start,x1,y1);

                  if(selected && selected[start])
                     tsw_selbar(y1,x1,x2,selAttr);
               }
            }
            break;

         case KEY_DN:
            {
               if(current >= num_entries-1)
                  break;

               previous=current++;
               tsw_selbar(previous+y1-start,x1,x2,attr);
               (*func)(previous,x1,previous+y1-start);
               if(selected && selected[previous])
                  tsw_selbar(previous+y1-start,x1,x2,selAttr);

               if(current>end)
               {
                  start++;
                  end++;

                  tsw_scroll(UP,x1,y1,x2,y2,1,attr);

                  (*func)(y2-y1+start,x1,y2);

                  if(selected && selected[y2-y1+start])
                     tsw_selbar(y2,x1,x2,selAttr);
               }
            }
            break;

      case KEY_PGUP:
         {
            if(!current)
               break;

            update=1;

            if(!start)
            {
               current = previous = 0;
               break;
            }

            current -= ys;
            current  = max(0,current);
            start   -= ys;
            end     -= ys;

            if(start<0)
            {
               start = 0;
               end   = start+ys-1;
            }

            previous=current;
         }
         break;

      case KEY_PGDN:
         {
            if(current >= num_entries-1)
               break;

            update = 1;

            if(end >= num_entries-1)
            {
               current = previous = num_entries-1;
               break;
            }
            current += ys;
            current  = (current >= num_entries) ? (num_entries-1) : current;
            start   += ys;
            end     += ys;

            if(end >= num_entries)
            {
               end   = num_entries-1;
               start = end-ys+1;
            }
            previous = current;
         }
         break;

      case KEY_HOME:
         {
            if(!current)
               break;

            start = current = previous = 0;
            end   = start+ys-1;

            if(end >= num_entries)
               end = num_entries-1;

            update=1;
         }
         break;

      case KEY_END:
         {
            if(current == num_entries-1)
               break;

            end   = current = previous = num_entries-1;
            start = end-ys+1;

            if(start < 0)
               start = 0;

            update = 1;
         }
         break;
      }
   }
}

void
SelectWindow::show(int n)
{
   if(n>=num_entries || n<0) return;

   end   = n   + (maxY-minY);
   if(end>=num_entries) end = num_entries - 1;

   start = end - (maxY-minY);
   if(start<0) start = 0;

   current = n;
}

/*
void
SelectWindow::showlast()
{
 end   = num_entries-1;
 start = end-(maxY-minY);

 if(start<0) start=0;

 current = end;
}
*/


SelectWindow::SelectWindow(int num,ATTR battr,void(*f)(int,int,int),ATTR sattr,char *sel,KEY *hot,void(*actionf)(int),void(*blankfunc)(int,int))
{
   bar_attr    = battr;
   selAttr    = sattr;
   num_entries = num;
   end         = 0;
   hotkeys     = hot;
   func        = f;
   action      = actionf;
   emptyfunc   = blankfunc;
   selected    = sel;
}

void
SelectWindow::define(int num,ATTR battr,void(*f)(int,int,int),ATTR sattr,char *sel,KEY *hot,void(*actionf)(int),void(*blankfunc)(int,int))
{
   bar_attr    = battr;
   selAttr    = sattr;
   num_entries = num;
   end         = 0;
   hotkeys     = hot;
   func        = f;
   action      = actionf;
   emptyfunc   = blankfunc;
   selected    = sel;
}

void
SelectWindow::redefine(int num)
{
   num_entries=num;

   if(num_entries-1<end)
   {
      end=num_entries-1;
      start=end-(maxY-minY);
      if(start<0) start=0;
   }
   else if(end && end<maxY-minY) end=num_entries-1;

   if(current>end) current=end;
}
