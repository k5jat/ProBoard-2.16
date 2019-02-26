#pragma warn -sig

#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <stdlib.h>
  #include <ctype.h>
  #include <string.h>
#endif

#ifdef __OS2__
   #define INCL_VIO
   #include <os2.h>

   #define sleep_clock()
#endif

#include <tswin.hpp>

void
Window::_init()
{
   CLEAR_OBJECT(*this);
}

Window::Window( int         x1,
                int         y1,
                int         x2,
                int         y2,
                ATTR        attr,
                int         mode,
                const char *border,
                ATTR        b_attr,
                char       *title,
                ATTR        t_attr,
                ATTR        o_attr,
                ATTR        tt_attr )
{
   _init();

   Window::x1          = x1;
   Window::y1          = y1;
   Window::x2          = x2;
   Window::y2          = y2;
   Window::attr        = attr;
   Window::mode        = mode;
   Window::borderType  = border;
   Window::b_attr      = (b_attr)? b_attr : attr;
   Window::t_attr      = (t_attr)? t_attr : attr;
   Window::iOuterColor = (o_attr)? o_attr : attr;

   Window::tt_attr     = (tt_attr)? tt_attr : Window::t_attr;

   if(title) strcpy(titleStr,title);
}


Window::Window()
{
   _init();
}


void
Window::open( int         x1,
              int         y1,
              int         x2,
              int         y2,
              ATTR        attr,
              int         mode,
              const char *border,
              ATTR        b_attr,
              char       *title,
              ATTR        t_attr,
              ATTR        tt_attr )
{
   Window::x1         = x1;
   Window::y1         = y1;
   Window::x2         = x2;
   Window::y2         = y2;
   Window::attr       = attr;
   Window::mode       = mode;
   Window::borderType = border;
   Window::b_attr     = (b_attr) ? b_attr : attr;
   Window::t_attr     = (t_attr) ? t_attr : attr;

   Window::tt_attr    = (tt_attr)? tt_attr : Window::t_attr;

   if(title)
      strcpy(titleStr,title);

   open();
}

void
Window::open()
{
   if(mode & CENTERED_X)
   {
      int w = x2-x1;

      x1 = tsw_hsize/2+1 - (w/2) - (w%2);
      x2 = tsw_hsize/2+1 + (w/2);
   }

   if(mode & CENTERED_Y)
   {
      int w = y2-y1;

      y1 = tsw_vsize/2+1 - (w/2) - (w%2);
      y2 = tsw_vsize/2+1 + (w/2);
   }

   mode &= ~(CENTERED_X|CENTERED_Y);

   curX = curY = 1;

   if(mode & NOBORDER)
   {
      minX = x1;
      minY = y1;
      maxX = x2;
      maxY = y2;
   }
   else
   {
      minX = x1+2;
      minY = y1+1;
      maxX = x2-2;
      maxY = y2-1;
   }

   if(mode & NOSAVE)
   {
      buf.clear();
   }
   else
   {
      if(mode & SHADOW)
      {
         buf.copy(x1 , y1 , x2-x1+3 , y2-y1+3);
      }
      else
      {
         buf.copy(x1 , y1 , x2-x1+1 , y2-y1+1);
      }
   }

   if(mode & EXPLODE)
      explode();

   if(mode & SHADOW)
      putShadow(x1,y1,x2,y2);

   if ( ! ( mode & NOCLEAR ) )
   {
      if ( borderType == RECESS_BORDER )
      {
         tsw_clearrect( x1,
                        y1,
                        x1,
                        y2,
                        iOuterColor );

         tsw_clearrect( x1 + 1,
                        y1,
                        x2 - 1,
                        y2,
                        attr );

         tsw_clearrect( x2,
                        y1,
                        x2,
                        y2,
                        iOuterColor );
      }
      else
      {
         tsw_clearrect( x1,
                        y1,
                        x2,
                        y2,
                        attr );
      }
   }

   frame();

   if(initFunc)
      (*initFunc)();

   updateCursor();
}

void
Window::close()
{
   if(curX <= 0) return;

   if(!(mode & NOSAVE))
   {
      if(mode & EXPLODE)
         implode();

      buf.paste(x1,y1);
   }

   buf.clear();

   curX = 0;
}

void
Window::clear()
{
   scroll(UP,0);

   setPos(1,1);
}

void
Window::border(const char *type,ATTR attr)
{
   borderType = type;

   if(attr)
      b_attr = attr;

   frame();
}

void
Window::border(ATTR attr)
{
   b_attr = attr;

   frame();
}

void
Window::putShadow(int x1,int y1,int x2,int y2)
{
   ATTR shadow_attr;
   int x,y;


   if ( iShadowColor == 0 )
   {
     shadow_attr = 0x08;
   }
   else
   {
     shadow_attr = iShadowColor;
   }


   for(x=x2,y=y1 ; y<=y2 ; y++)
   {
#ifdef __OS2__
      if(x<tsw_hsize)   VioWrtNAttr(&shadow_attr,1,y,x  ,0);
      if(x<tsw_hsize-1) VioWrtNAttr(&shadow_attr,1,y,x+1,0);
#else
      if(x<tsw_hsize)   *(tsw_videobase+y*tsw_hsize*2+x*2+1) = shadow_attr;
      if(x<tsw_hsize-1) *(tsw_videobase+y*tsw_hsize*2+x*2+3) = shadow_attr;
#endif
   }

   for(y = y2 , x = x1+1 ; x <= (x2>(tsw_hsize-2) ? (tsw_hsize-2) : x2)+1 ; x++)
   {
#ifdef __OS2__
      VioWrtNAttr(&shadow_attr,1,y,x,0);
#else
      *(tsw_videobase+y*tsw_hsize*2+x*2+1) = shadow_attr;
#endif
   }
}

#define EXPL_STEP 25;

void
Window::explode()
{
   int x,y;
   int xs,ys,x_step,y_step;

   int x1 = Window::x1 * 100;
   int x2 = Window::x2 * 100;
   int y1 = Window::y1 * 100;
   int y2 = Window::y2 * 100;

   xs     = x2 - x1;
   ys     = y2 - y1;

   x_step = xs / EXPL_STEP;
   y_step = ys / EXPL_STEP;

   for(x=(xs/2+x1) , y=(ys/2+y1) ; x>=x1 ; x-=x_step,y-=y_step)
   {
      int xt1 = x / 100;
      int yt1 = y / 100;

      int xt2 = (x2+x1-x) / 100;
      int yt2 = (y2+y1-y) / 100;

      tsw_wait20ms();

      if(mode & SHADOW) putShadow(xt1,yt1,xt2,yt2);

      if(xt2-xt1>1 && yt2-yt1>1) tsw_clearrect(xt1+1,yt1+1,xt2-1,yt2-1,attr);

      if(!(mode&NOBORDER)) tsw_drawbox(xt1,yt1,xt2,yt2,borderType,b_attr);

      tsw_changeatt(attr,xt1,yt1,xt2,yt2);
   }
}

void
Window::title(char *str,ATTR att)
{
   tt_attr = (att) ? att:b_attr;

   titleStr[0] = '\0';

   if(str)
      strcpy(titleStr,str);

   frame();
}

void
Window::implode()
{
   int x,y;
   int xs,ys,x_step,y_step;

   int x1 = Window::x1 * 100;
   int x2 = Window::x2 * 100;
   int y1 = Window::y1 * 100;
   int y2 = Window::y2 * 100;

   xs = x2 - x1;
   ys = y2 - y1;

   x_step = xs / EXPL_STEP;
   y_step = ys / EXPL_STEP;

   int prev_xt1 = Window::x1;
   int prev_yt1 = Window::y1;
   int prev_xt2 = Window::x2;
   int prev_yt2 = Window::y2;

   for(x=x1 , y=y1 ; x<(xs/2+x1) ; x+=x_step,y+=y_step)
   {
      bool flag = FALSE;
      int xt1 = x / 100;
      int yt1 = y / 100;

      int xt2 = (x2 + x1 - x + 50) / 100;
      int yt2 = (y2 + y1 - y + 50) / 100;

      if(mode & SHADOW)
      {
         buf.paste(prev_xt1-Window::x1+3,prev_yt2-Window::y1+2,prev_xt1+2,prev_yt2+1,prev_xt2-prev_xt1+1,1);
         buf.paste(prev_xt2-Window::x1+2,prev_yt1-Window::y1+2,prev_xt2+1,prev_yt1+1,2,prev_yt2-prev_yt1+1);
      }

      if(yt1!=prev_yt1)
      {
         buf.paste(prev_xt1-Window::x1+1 , prev_yt1-Window::y1+1 , prev_xt1 , prev_yt1 , prev_xt2 - prev_xt1 + 1 , yt1-prev_yt1);
         prev_yt1 = yt1;
         flag     = TRUE;
      }

      if(yt2 != prev_yt2)
      {
         buf.paste(prev_xt1-Window::x1+1 , yt2-Window::y1+2 , prev_xt1 , yt2+1 , prev_xt2 - prev_xt1 + 1 , prev_yt2-yt2);
         prev_yt2 = yt2;
         flag     = TRUE;
      }

      if(xt1!=prev_xt1)
      {
         buf.paste(prev_xt1-Window::x1+1 , prev_yt1-Window::y1+1 , prev_xt1 , prev_yt1 , xt1-prev_xt1 , prev_yt2-prev_yt1+1);
         prev_xt1 = xt1;
         flag = TRUE;
      }

      if(xt2!=prev_xt2)
      {
         buf.paste(xt2-Window::x1+2 , prev_yt1-Window::y1+1 , xt2+1 , prev_yt1 , prev_xt2-xt2   , prev_yt2-prev_yt1+1);
         prev_xt2 = xt2;
         flag = TRUE;
      }

      if(flag)
      {
         if(mode & SHADOW) putShadow(xt1,yt1,xt2,yt2);
         if(!(mode & NOBORDER)) tsw_drawbox(xt1,yt1,xt2,yt2,borderType,b_attr);
         tsw_wait20ms();
      }
   }
}


void
Window::title(ATTR att)
{
   t_attr = att;

   frame();
}

void Window::frame()
{
     if ( ! ( mode & NOBORDER ) )
     {
          if ( borderType == CHISEL_BORDER )
          {
               tsw_chiselbox( x1 + 1,
                              y1,
                              x2 - 1,
                              y2,
                              borderType,
                              b_attr,
                              t_attr );
          }
          else if ( borderType == RECESS_BORDER )
          {
               tsw_recessbox( x1 + 1,
                              y1,
                              x2 - 1,
                              y2,
                              borderType,
                              b_attr,
                              t_attr,
                              attr,
                              iOuterColor );
          }
          else
          {
               tsw_drawbox( x1 + 1,
                            y1,
                            x2 - 1,
                            y2,
                            borderType,
                            b_attr );
          }


          if ( titleStr[ 0 ] )
          {
               int x  =  x1  +  ( x2 - x1 + 1 )  /  2  -
                             strlen( titleStr )  /  2;
               int y  =  y1;


               tsw_maputs( x,
                           y,
                           tt_attr,
                           titleStr );


               if ( mode & BRACKETS )
               {
                    tsw_maputs( x - 2, 
                                y,
                                b_attr,
                                "´ " );

                    tsw_maputs( x + strlen( titleStr ), 
                                y,
                                b_attr,
                                " Ã" );
               }
          }
     }
}



#define xSize (maxX-minX+1)
#define ySize (maxY-minY+1)

static
 struct clr
   {
   int val;
   byte andval;
   byte orval;
   } colors[]=
      {
       {  0 , 0x00 , 0x07 },
       {  1 , 0xFF , 0x08 },
       {  5 , 0xFF , 0x80 },
       {  6 , 0xFF , 0x80 },
       { 30 , 0xF8 , 0x00 },
       { 31 , 0xF8 , 0x04 },
       { 32 , 0xF8 , 0x02 },
       { 33 , 0xF8 , 0x06 },
       { 34 , 0xF8 , 0x01 },
       { 35 , 0xF8 , 0x05 },
       { 36 , 0xF8 , 0x03 },
       { 37 , 0xF8 , 0x07 },
       { 40 , 0x8F , 0x00 },
       { 41 , 0x8F , 0x40 },
       { 42 , 0x8F , 0x20 },
       { 43 , 0x8F , 0x60 },
       { 44 , 0x8F , 0x10 },
       { 45 , 0x8F , 0x50 },
       { 46 , 0x8F , 0x30 },
       { 47 , 0x8F , 0x70 }
      };

Window&
Window::operator<<(char *str)
{
   while(*str)
      (*this) << (*str++);

   return *this;
}

Window&
Window::operator<<(char c)
{
   int i;

   switch(terminal.status)
   {
      case TERMINAL_NORMAL:
         {
            switch(c)
               {
                  case  0 : break;
         /*^Y*/  case 25 : terminal.status = TERMINAL_AVT_REPCHAR;
                           terminal.needed = 2;
                           break;
         /*LF*/  case 10 : curX = 1;
                           curY++;
                           break;
         /*CR*/  case 13 : curX = 1;
                           break;
         /*FF*/  case 12 : attrib(7);
                           clear();
                           break;
         /*BEL*/ case  7 : if(tsw_shutup) break;
                           sound(2000);
                           sleep_clock();
                           sleep_clock();
                           sleep_clock();
                           stopsound();
                           break;
         /*BS*/  case  8 : if(curX>1) curX--;
                           tsw_maputc(minX+curX-1,minY+curY-1,attr,' ');
                           break;
         /*ESC*/ case 27 : terminal.status = TERMINAL_ANSI;
                           terminal.par  = 0;
                           terminal.p[0] = terminal.p[1]
                                          = terminal.p[2]
                                          = terminal.p[3] = 0;
                           break;
         /*^V*/  case 22 : terminal.status = TERMINAL_AVT;
                           break;
                  default : tsw_maputc(minX+(curX++)-1,minY+curY-1,attr,c);
                           break;
               }
         }
         break; // case TERMINAL_NORMAL

      case TERMINAL_ANSI:
         {
            if(isalpha(c))
            {
               terminal.status = TERMINAL_NORMAL;
               terminal.par++;

               switch(c)
               {
                  case 'f': // Goto row,col
                  case 'H':
                     {
                        if(!terminal.p[0]) terminal.p[0]++;
                        if(!terminal.p[1]) terminal.p[1]++;

                        setPos( terminal.p[1] , terminal.p[0] );
                     }
                     break;

                  case 'A': // Cursor Up
                     {
                        if(!terminal.p[0])
                           terminal.p[0]++;

                        cursorUp(terminal.p[0]);
                     }
                     break;

                  case 'B': // Cursor Down
                     {
                        if(!terminal.p[0])
                           terminal.p[0]++;

                        cursorDown(terminal.p[0]);
                     }
                     break;

                  case 'C': // Cursor Right
                     {
                        if(!terminal.p[0])
                           terminal.p[0]++;

                        cursorRight(terminal.p[0]);
                     }
                     break;

                  case 'D': // Cursor Left
                     {
                        if(!terminal.p[0])
                           terminal.p[0]++;

                        cursorLeft(terminal.p[0]);
                     }
                     break;

                  case 's': // Save Cursor Position
                     {
                        saveCursor();
                     }
                     break;

                  case 'u': // Restore Cursor Position
                     {
                        restoreCursor();
                     }
                     break;

                  case 'J': // Clear Screen
                     {
                        if(terminal.p[0] == 2)
                           clear();
                     }
                     break;

                  case 'K': // Clear EOL
                     {
                        for(i=curX ; i<=xSize ;i++)
                           direct(i,curY,attr,' ');
                     }
                     break;

                  case 'm': // Set Color
                     {
                        for(i=0 ; i<terminal.par ; i++)
                        {
                           for(int j=0;j<20;j++)
                              if(colors[j].val==terminal.p[i])
                              {
                                 attr &= colors[j].andval;
                                 attr |= colors[j].orval;
                                 break;
                              }
                        }
                     }
                     break;
               }
            }
            else
            {
               if(isdigit(c))
               {
                  terminal.p[terminal.par] *= 10;
                  terminal.p[terminal.par] += c-'0';
               }
               else
               {
                  if(c==';')
                  {
                     terminal.p[++terminal.par] = 0;
                  }
                  else
                  {
                     if(c!='[')
                        terminal.status = TERMINAL_NORMAL;
                  }
               }
            }
         }
         break; // case TERMINAL_ANSI

      case TERMINAL_AVT_REPCHAR:
         {
            if(terminal.needed == 1)
            {
               terminal.status = TERMINAL_NORMAL;

               for(i=0;i<int(byte(c));i++)
                  (*this) << byte(terminal.p[0]);
            }
            else
            {
               terminal.p[0] = c;
               terminal.needed--;
            }
         }
         break;

      case TERMINAL_AVT:
         {
            switch(c)
               {
                  case 1: // Go in Attribute Set Mode
                     {
                        terminal.status = TERMINAL_AVT_ATTR;
                     }
                     break;

                  case 2: // Set Blink On
                     {
                        attr |= 0x80;

                        terminal.status = TERMINAL_NORMAL;
                     }
                     break;

                  case 3: // Cursor Up
                     {
                        cursorUp();

                        terminal.status = TERMINAL_NORMAL;
                     }
                     break;

                  case 4: // Cursor Down
                     {
                        cursorDown();

                        terminal.status = TERMINAL_NORMAL;
                     }
                     break;

                  case 5: // Cursor Left
                     {
                        cursorLeft();

                        terminal.status = TERMINAL_NORMAL;
                     }
                     break;

                  case 6: // Cursor Right
                     {
                        cursorRight();

                        terminal.status = TERMINAL_NORMAL;
                     }
                     break;

                  case 7: // Clear EOL
                     {
                        for(i=curX ; i<=xSize ; i++)
                           direct(i,curY,attr,' ');

                        terminal.status = TERMINAL_NORMAL;
                     }
                     break;

                  case 8: // Start Move Cursor Command
                     {
                        terminal.status = TERMINAL_AVT_MOVE;

                        terminal.needed = 2;
                     }
                     break;

                  case 25: // Start RLE command
                     {
                        terminal.status = TERMINAL_AVT_RLE;
                        terminal.needed = 0;
                     }
                     break;

                  default: // Unknown command
                     {
                        terminal.status = TERMINAL_NORMAL;
                        (*this) << c;
                     }
                     break;
               }
         }
         break;

      case TERMINAL_AVT_ATTR:
         {
            attr = byte(c);

            terminal.status = TERMINAL_NORMAL;
         }
         break;

      case TERMINAL_AVT_MOVE:
         {
            if(terminal.needed==1)
            {
               if(c<=xSize && c>=1 && terminal.p[0]>=1 && terminal.p[0]<=ySize)
               {
                  curX = c;
                  curY = terminal.p[0];
               }
               else
               {
                  terminal.status = TERMINAL_NORMAL;

                  (*this) << char(terminal.p[0]) << c;
               }

               terminal.status = TERMINAL_NORMAL;
            }
            else
            {
               terminal.p[0] = c;
               terminal.needed--;
            }
         }
         break;

      case TERMINAL_AVT_RLE:
         {
            if(!terminal.needed)
            {
               terminal.p[0]     = byte(c);
               terminal.rle_buf  = new byte[c];
               terminal.par      = 0;
               terminal.needed   = byte(c+1);
            }
            else
            {
               if(terminal.needed>1)
               {
                  terminal.rle_buf[terminal.par++] = c;
                  terminal.needed--;
               }
               else
               {
                  terminal.status = TERMINAL_NORMAL;

                  int repcount = terminal.p[0];
                  byte *buf=new byte[repcount];

                  for(i=0;i<repcount;i++) buf[i]=terminal.rle_buf[i];

                  delete [] terminal.rle_buf;

                  for(i=0;i<c;i++)
                     for(int j=0;j<repcount;j++)
                        (*this) << buf[j];
               }
            }
         }
         break;
   }


   if(curX > xSize)
   {
      curX = 1;
      curY++;
   }

   if(curY > ySize)
   {
      scroll();
      curY = ySize;
   }

   updateCursor();

   return *this;
}

void
Window::updateCursor()
{
   if(curX < 1)      curX = 1;
   if(curY < 1)      curY = 1;
   if(curX > xSize)  curX = xSize;
   if(curY > ySize)  curY = ySize;

   if(!noCursorUpdate)
   {
      tsw_gotoxy(curX+minX-2 , curY+minY-2);
   }
}

void
Window::setPos(int x,int y)
{
   curX = x;
   curY = y;

   updateCursor();
}

void
Window::moveCursor(int dx,int dy)
{
   curX += dx;
   curY += dy;

   updateCursor();
}

void
Window::saveCursor()
{
   if(sp < 5)
   {
      oldX[sp] = curX;
      oldY[sp] = curY;

      sp++;
   }
}

void
Window::restoreCursor()
{
   if(sp > 0)
   {
      sp--;

      setPos(oldX[sp],oldY[sp]);
   }
}

void
Window::zoom()
{
   close();

   x1 = 1;
   y1 = 1;
   x2 = tsw_hsize;
   y2 = tsw_vsize;

   open();
}

void
Window::centerLine(int y,char *s,ATTR a)
{
   int wy = (y == 0) ? ((maxY-minY)/2+1): y;

   direct((maxX-minX+1)/2 - strlen(s)/2 + 1,wy,a ? a:attr,s);
}

void
Window::direct(int x,int y,ATTR a,char *str)
{
   tsw_maputs(minX+x-1,minY+y-1,a,str);
}




static
void rec_box( int x1,
              int y1,
              int x2,
              int y2,
              const char *border,
              ATTR top,
              ATTR bottom,
              ATTR left,
              ATTR right )
{
     int i;


     for ( i = x1 + 1;  i < x2;  i++ )
     {
          tsw_maputc( i, 
                      y1, 
                      top,
                      border[ 1 ] );

          tsw_maputc( i, 
                      y2, 
                      bottom,
                      border[ 5 ] );
     }


     for ( i = y1 + 1;  i < y2; i++ )
     {
          tsw_maputc( x1, 
                      i, 
                      left,
                      border[ 7 ] );

          tsw_maputc( x2, 
                      i, 
                      right,
                      border[ 3 ] );
     }


     //tsw_maputc( x1, 
     //            y1, 
     //            top,
     //            border[ 0 ] );
     //
     //tsw_maputc( x2, 
     //            y1, 
     //            right,
     //            border[ 2 ] );
     //
     //tsw_maputc( x1, 
     //            y2, 
     //            left,
     //            border[ 6 ] );
     //
     //tsw_maputc( x2, 
     //            y2, 
     //            right,
     //            border[ 4 ] );
}


void Window::RecessBox( int x1,
                        int y1,
                        int x2,
                        int y2,
                        const char *border,
                        ATTR top,
                        ATTR bottom,
                        ATTR left,
                        ATTR right )
{
     tsw_recessbox( minX + x1 - 1,
                    minY + y1 - 1,
                    minX + x2 - 1,
                    minY + y2 - 1,
                    border,
                    top,
                    bottom,
                    left,
                    right );
}


void
Window::direct(int x,int y,char *str)
{
   direct(x,y,attr,str);
}

void
Window::direct(int x,int y,char c)
{
   direct(x,y,attr,c);
}

void
Window::direct(int x,int y,ATTR a,char c)
{
   tsw_maputc(minX+x-1,minY+y-1,a,c);
}

void
Window::change(int x1,int y1,int x2,int y2)
{
   Window::x1 = minX = x1;
   Window::y1 = minY = y1;
   Window::x2 = maxX = x2;
   Window::y2 = maxY = y2;

   clear();
}

void
Window::scroll(byte direction,word numlines)
{
   tsw_scroll(direction,minX,minY,maxX,maxY,numlines,attr);
}

char
Window::whaton(int x,int y)
{
   return tsw_whaton(minX+x-1,minY+y-1);
}

void
Window::cursorUp(int n)
{
   curY -= n;

   updateCursor();
}

void
Window::cursorDown(int n)
{
   curY += n;

   updateCursor();
}

void
Window::cursorLeft(int n)
{
   curX -= n;

   updateCursor();
}

void
Window::cursorRight(int n)
{
   curX += n;

   updateCursor();
}

void
Window::placeCursor()
{
   bool old = noCursorUpdate;

   noCursorUpdate = FALSE;

   updateCursor();

   noCursorUpdate = old;
}

