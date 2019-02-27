#ifdef PEX
  #define NO_KEY_DEF
 #include <pb_sdk.h>
#endif

#include <tswin.hpp>

EditWindow::EditWindow()
{
 line = new String[1000];

 ed_x=ed_y=ed_x1=ed_y1=1;

 numlines=0;
}

EditWindow::~EditWindow()
{
 delete [] line;
}

void
EditWindow::feed(char *text)
{
 for(int i=0;*text;text++)
  {
   switch(*text)
     {
      case '\n': i++; break;
      case '\r': break;
      default  : line[i] << (*text);
     }
  }
 numlines=i+1;
}


void
EditWindow::retrieve(char *text,int /*maxlen*/)
{
 for(int i=0;i<numlines;i++)
   {
    for(int j=0;j<line[i].len();j++) *text++ = line[i][j];

    *text++ = '\r';
    *text++ = '\n';
   }

*text++='\0';
}

void
EditWindow::display(int l)
{
 if(ed_x < ed_x1) ed_x1 = ed_x;

 if(ed_x > ed_x1 + maxX - minX) ed_x1 = ed_x - maxX + minX;

 if(ed_y < ed_y1) ed_y1 = ed_y;

 if(ed_y > ed_y1 + maxY - minY) ed_y1 = ed_y - maxY + minY;

 if(l < ed_y1 || l > (ed_y1 + maxY - minY)) return;

 int len = line[l-1].len();

 for(int i=ed_x1;i<=(ed_x1+maxX-minX);i++)
   {
    if(i<=len && l<=numlines) direct(i-ed_x1+1,l-ed_y1+1,line[l-1][i-1]);
                         else direct(i-ed_x1+1,l-ed_y1+1,' ');
   }
}

void
EditWindow::display()
{
 for(int i=0;i<numlines;i++) display(i+1);

 for(;i<numlines+25;i++) display(i+1);

 display_cursor();
}

void
EditWindow::display_cursor()
{
   setPos(ed_x-ed_x1+1,ed_y-ed_y1+1);
}

void
EditWindow::key_up()
{
 if(ed_y<=1) return;

 if(ed_y==ed_y1)
   {
    scroll(DOWN);
    display(--ed_y1);
   }

 ed_y--;

 display_cursor();
}


void
EditWindow::key_left()
{
 if(ed_x<=1) return;

 if(ed_x==ed_x1)
   {
    ed_x1--;
    display();
   }

 ed_x--;

 display_cursor();
}

void
EditWindow::key_right()
{
 if(ed_x>=255) return;

 if(ed_x==ed_x1+maxX-minX)
   {
    ed_x1++;
    display();
   }

 ed_x++;

 display_cursor();
}

void
EditWindow::key_down()
{
 if(ed_y>=numlines) return;

 if(ed_y==ed_y1+maxY-minY)
   {
    scroll(UP);
    ed_y1++;
    display(ed_y1+maxY-minY);
   }

 ed_y++;

 display_cursor();
}

void
EditWindow::key_del()
{
 if(ed_x>line[ed_y-1].len())
    {
     if(ed_y>=numlines) return;

     key_char(' ');
     key_bs();
     line[ed_y-1] << line[ed_y];
     for(int i=ed_y+1;i<numlines;i++) line[i-1]=line[i];
     line[numlines--]="";
     display();
    }
   else
    {
     line[ed_y-1].del(ed_x-1,1);
    }

 display(ed_y);
}

void
EditWindow::key_enter()
{
 for(int i=numlines;i>ed_y;i--) line[i]=line[i-1];

 if(ed_x>line[ed_y-1].len())
   {
    line[ed_y]="";
   }
  else
   {
    line[ed_y]=&line[ed_y-1][ed_x-1];
    line[ed_y-1].del(ed_x-1,line[ed_y-1].len()-ed_x+1);
   }
 ed_y++;
 ed_x=1;
 numlines++;

 display();
}

void
EditWindow::key_end()
{
 ed_x=line[ed_y-1].len()+1;

 display();
}

void
EditWindow::key_home()
{
 ed_x=1;

 display();
}

void
EditWindow::key_char(char k)
{
 char s[2]; s[0]=k; s[1]=0;

 line[ed_y-1].insert(ed_x-1,s);
 ed_x++;

 display();
}

void
EditWindow::key_bs()
{
 if(ed_x==1)
   {
    key_up();
    key_end();
    key_del();
   }
  else
   {
    key_left();
    if(ed_x>line[ed_y-1].len()) return;
    key_del();
   }
}

void
EditWindow::key_pageup()
{
 int l=maxY-minY+1;

 ed_y1 = (ed_y1 <= l) ? 1 : ed_y1-l;
 ed_y  = (ed_y  <= l) ? 1 : ed_y-l;

 display();
}

void
EditWindow::key_pagedown()
{
 int l=maxY-minY+1;

 ed_y1 = (ed_y1 >= (numlines-l)) ? (numlines-l) : ed_y1+l;
 ed_y  = (ed_y  >= (numlines-l)) ? (numlines)   : ed_y +l;

 display();
}

KEY
EditWindow::edit(KEY *hotkeys)
{
display();

for(;;)
  {
   KEY k=KB.get();
   for(int i=0;hotkeys && hotkeys[i];i++) if(k==hotkeys[i]) return hotkeys[i];
   switch(k)
     {
      case KEY_UP  : key_up();       break;
      case KEY_DN  : key_down();     break;
      case KEY_LT  : key_left();     break;
      case KEY_RT  : key_right();    break;
      case KEY_DEL : key_del();      break;
      case KEY_ENT : key_enter();    break;
      case KEY_HOME: key_home();     break;
      case KEY_END : key_end();      break;
      case KEY_PGUP: key_pageup();   break;
      case KEY_PGDN: key_pagedown(); break;
      case 8       : key_bs();       break;
      case KEY_ESC : return 0;
      default      : if(k<256) key_char(k);
                     break;
     }
  }
}

