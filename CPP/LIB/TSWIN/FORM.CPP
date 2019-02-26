#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <string.h>
  #include <ctype.h>
  #include <stdlib.h>
#endif

#include <tswin.hpp>



//----------------------------------------------------------------
//
// This table identifies where the slashes should be in the date
// strings.  This supports each of the international date formats:
//
//        MM/DD/YYYY
//        YYYY/MM/DD
//        DD/MM/YYYY
//        MM/YYYY/DD
//   
//----------------------------------------------------------------
static
const char piSlashColumns[ 4 ] [ 2 ]  =
{
     { 2, 5 },
     { 4, 7 },
     { 2, 5 },
     { 2, 7 },
};



extern char *form(const char * ...);

FormWindow::FormWindow()
{
   numFields = cur_field = 0;
}

FormWindow::FormWindow(int x1,int y1,int x2,int y2,ATTR attr,int type,const char *border,ATTR b_attr,char *title,ATTR t_attr) : Window(x1,y1,x2,y2,attr,type,border,b_attr,title,t_attr,0)
{
   numFields = cur_field = 0;
}

FormWindow::FormWindow(Field *fields,int num,ATTR dispAttr,ATTR selAttr,KEY *hotkeys,void(*func)(int))
{
   define(fields,num,dispAttr,selAttr,hotkeys,func);
}

void
FormWindow::define(Field *fields,int num,ATTR attr,ATTR s_attr,KEY *hot,void(*func)(int))
{
   f          = fields;
   numFields  = num;
   dispAttr   = attr;
   selAttr    = s_attr;
   hotkeys    = hot;
   helpFunc   = func;
   cur_field  = 0;
}

static char *yesno[] = { "No ","Yes" };

void
FormWindow::showField(int i)
{
     String buffer;


     switch ( f[ i ].type )
     {
          case FRM_FILENAME:
          case FRM_PATH:
          case FRM_STRING:
          
               buffer = form("%-*s",f[i].width,(char *)f[i].value);
               break;


          case FRM_DATE:
          
               buffer  =  ( (Date *) f[ i ].value )->format( (char *) f[ i ].data );
               break;


          case FRM_TIME:
          
               buffer = ((Time *)f[i].value)->format((char *)f[i].data);
               break;


          case FRM_INT:
          
               buffer = form("%-*d",f[i].width,*((int *)(f[i].value)));
               break;


          case FRM_LONG:
          
               buffer = form("%-*ld",f[i].width,*((long *)(f[i].value)));
               break;


          case FRM_UNSIGNED: 
          
               buffer = form("%-*u",f[i].width,*((unsigned *)(f[i].value)));
               break;


          case FRM_ULONG:
          
               buffer = form("%-*lu",f[i].width,*((unsigned long *)(f[i].value)));
               break;


          case FRM_CHAR:
          
               buffer = form("%-*c",f[i].width,*((char *)(f[i].value)));
               break;


          case FRM_SHORT:
          
               buffer = form("%-*d",f[i].width,*((char *)(f[i].value)));
               break;


          case FRM_YESNO:
          
               buffer = yesno[(*((char *)f[i].value))?1:0];
               break;


          case FRM_CHOICE:
          {
               char **choices = (char **) f[i].data;

               buffer = form("%-*s",f[i].width,choices[*((char *)f[i].value)]);
          }
               break;


          case FRM_FUNCTION: 
          
               (*(f[i].func))(0,*this,f[i].x,f[i].y,dispAttr,f[i].value);
               break;
     }


     if(f[i].type != FRM_FUNCTION)
     {
          direct(f[i].x,f[i].y,dispAttr,form("%-*.*s",f[i].width,f[i].width,(char *)buffer));
     }
}



//**************************************************************************
//
// Edit a date field
//
//     Prototype:  int edit_date( Window &w, int x, int y, ATTR attr, KEY    
//                                *hotkeys, Date &date, char *fmt );
//
//    Parameters:  w       ... Window containing field
//                 x       ... X position in window where field starts
//                 y       ... Y position in window where field starts
//                 attr    ... Video attributes for field
//                 hotkeys ... List of keystroke hotkeys to look for
//                 date    ... Date variable to edit
//                 fmt     ... Date format to display
//
//       Returns:  A field editor return code indicating how the field was
//                 completed.
//
//       Remarks:
//
// This routine parses a date field, using a picture template based on the
// format parameter <fmt>.  It had to be enhanced by Jeff Reeder to support
// four-byte year data entry.
//
// -------------------------------------------------------------------------
//
//    Created on:  ??/??/1999 (Philippe Leybaert)
// Last modified:  08/05/1999 (Jeff Reeder)   Modified for Y2K compliance
//
//**************************************************************************

#define MAX_DATE 10

static 
int edit_date( Window &w, 
               int     x, 
               int     y, 
               ATTR    attr, 
               KEY    *hotkeys, 
               Date   &date, 
               char   *fmt )
{
     char s     [ MAX_DATE + 1 ];
     int  fmt_ar[ 3  ];
     int  iEntry;


     //-------------------------------------------  
     // Figure out which format we're dealing with
     //-------------------------------------------  

     if ( strcmp( fmt, "MM/DD/YYYY" )  ==  0 )
     {
          fmt_ar[ 0 ]  =  DATE_MONTH;
          fmt_ar[ 1 ]  =  DATE_DAY;
          fmt_ar[ 2 ]  =  DATE_YEAR;

          iEntry = 0;
     }
     else if ( strcmp( fmt, "YYYY/MM/DD" )  ==  0 )
     {
          fmt_ar[ 0 ]  =  DATE_YEAR;
          fmt_ar[ 1 ]  =  DATE_MONTH;
          fmt_ar[ 2 ]  =  DATE_DAY;

          iEntry = 1;
     }
     else      // if ( strcmp( fmt, "DD/MM/YYYY" )  ==  0 )
     {
          fmt_ar[ 0 ]  =  DATE_DAY;
          fmt_ar[ 1 ]  =  DATE_MONTH;
          fmt_ar[ 2 ]  =  DATE_YEAR;

          iEntry = 2;
     }


     //-----------------------------------------  
     // Get the data as it will appear initially
     //-----------------------------------------  

     strncpy( s,
              date.format( fmt ),
              MAX_DATE );

     s[ MAX_DATE ]  =  '\0';


     //--------------------------------------------------  
     // Display the initial date value in the right color
     //--------------------------------------------------  

     w.attrib( attr );
     w.setPos( x, y );
     w  <<  (char *) s;
     w.setPos( x, y );


     int curpos = 0;
     KEY k;


     //------------------------------  
     // Keep looping until we're done
     //------------------------------  

     for ( ; ; )
     {
          //-------------------------------------------
          // Display the current date, and position the
          // cursor at the current cursor position.
          //-------------------------------------------

          w.setPos( x, y );
          w << s;
          w.setPos( x + curpos,  y );


          //----------------  
          // Get a keystroke
          //----------------  

          k = KB.get();


          if ( hotkeys )
          {
               //-------------------------------------------------  
               // We're looking for hotkeys.  Is this one of them?
               //-------------------------------------------------  

               for ( int j = 0;  hotkeys[ j ];  j++ )
               {
                    if ( k  ==  hotkeys[ j ] )
                    {
                         //---------------------------------  
                         // We found a hotkey - stop looking
                         //---------------------------------  

                         break;
                    }
               }


               if ( k  ==  hotkeys[ j ] )
               {
                    //-------------------------------------------  
                    // We found a hotkey - break out of edit loop
                    //-------------------------------------------  

                    break;
               }
          }


          if ( curpos < MAX_DATE  && 
               isdigit( (char) k ) )
          {
               //-----------------------------------------------
               // Our date field isn't full yet, and this is a
               // numeric digit.  Record it in the string buffer
               //-----------------------------------------------

               s[ curpos++ ]  =  (char) k;
          }


          if ( curpos >= MAX_DATE )
          {
               //-------------------------------------------------
               // The cursor position has moved off the end of the
               // field.  Back it down by one byte so it's inside.
               //-------------------------------------------------

               curpos = MAX_DATE - 1;
          }


          if ( k == KEY_RET   || 
               k == KEY_DN    || 
               k == KEY_UP    || 
               k == KEY_TAB   || 
               k == KEY_STAB  || 
               k == KEY_ESC   ) 
          {
               //---------------------------------------------------------  
               // The keystroke will take us out of this field - break out
               //---------------------------------------------------------  

               break;
          }


          if ( k == KEY_RT  && 
               curpos  <  MAX_DATE - 1 )
          {
               //-----------------------------------------------------
               // The right-arrow was hit, and the cursor's not at the
               // end of the field.  Move to the right one character.
               //-----------------------------------------------------

               curpos++;
          }


          if ( 
               ( 
                 k == KEY_LT  ||  
                 k == 8 
               )
               &&  curpos > 0 )
          {
               //-----------------------------------------------------------
               // The left-arrow or backspace key was hit.  Since the cursor
               // isn't at home position, it's safe to move backward.
               //-----------------------------------------------------------

               curpos--;
          
               
               if ( curpos  ==  piSlashColumns[ iEntry ][ 0 ]  ||
                    curpos  ==  piSlashColumns[ iEntry ][ 1 ] )
               {
                    //---------------------------------------------------  
                    // We just backed on top of a slash.  Move back again
                    //---------------------------------------------------  

                    curpos--;
               }
          }


          if ( curpos  ==  piSlashColumns[ iEntry ][ 0 ]  || 
               curpos  ==  piSlashColumns[ iEntry ][ 1 ] )
          {
               //-------------------------------------------------
               // The cursor is currently on top of a slash.  Move
               // forward to the next valid numeric digit value.
               //-------------------------------------------------

               curpos++;
          }
     }


     //-----------------------------  
     // Extract the numerical values
     //-----------------------------  

     int val1  =  atoi( strtok( s,    "-/" ) );
     int val2  =  atoi( strtok( NULL, "-/" ) );
     int val3  =  atoi( strtok( NULL, "-/" ) );


     if ( val1 != 0  ||
          val2 != 0  ||
          val3 != 0 )
     {
          //-------------------------------------------------------------  
          // Figure out which one's the year, and handle it appropriately
          //-------------------------------------------------------------  
     
          if ( fmt_ar[ 0 ]  ==  DATE_YEAR )
          {
               date[ fmt_ar[ 0 ] ]  =  byte( NormalizeYear( val1 ) );
          }
          else
          {
               date[ fmt_ar[ 0 ] ]  =  byte( val1 );
          }
     
     
          if ( fmt_ar[ 1 ]  ==  DATE_YEAR )
          {
               date[ fmt_ar[ 1 ] ]  =  byte( NormalizeYear( val2 ) );
          }
          else
          {
               date[ fmt_ar[ 1 ] ]  =  byte( val2 );
          }
     
     
          if ( fmt_ar[ 2 ]  ==  DATE_YEAR )
          {
               date[ fmt_ar[ 2 ] ]  =  byte( NormalizeYear( val3 ) );
          }
          else
          {
               date[ fmt_ar[ 2 ] ]  =  byte( val3 );
          }
     }
     else
     {
          //------------------------------------------
          // User entered something like "00/00/0000".
          // Make the date all zeros (i.e., unset).
          //------------------------------------------

          date[ 0 ]  =  0;
          date[ 1 ]  =  0;
          date[ 2 ]  =  0;
     }


     if ( hotkeys )
     {
          for ( int j = 0;  hotkeys[ j ];  j++ )
          {
               if ( k  ==  hotkeys[ j ] )
               {
                    w.scanHotKey = k;

                    return SF_HOT;
               }
          }
     }


     //-----------------------------------------------------  
     // Find out what keystroke terminated our field editing
     //-----------------------------------------------------  

     switch ( k )
     {
          case KEY_STAB:
          case KEY_UP: 
          
               //-----------------------  
               // Move to previous field
               //-----------------------  

               return SF_UP;


          case KEY_TAB:
          case KEY_RET:
          case KEY_DN:
          
               //-------------------  
               // Move to next field
               //-------------------  

               return SF_DOWN;


          case KEY_ESC:
          
               //-----  
               // Quit
               //-----  

               return SF_ESC;
     }


     return SF_ESC;
}



int
FormWindow::editField(int i)
{
 switch(f[i].type)
   {
    case FRM_STRING  : if(f[i].flags & FRM_UPPER) return scan((char *)f[i].value,f[i].len,f[i].width,byte(SCAN_UPPER|SCAN_FIELD));
                                            else  return scan((char *)f[i].value,f[i].len,f[i].width,byte(SCAN_FIELD));

    case FRM_DATE    : 
    
       return  edit_date( *this, 
                          f[ i ].x,
                          f[ i ].y,
                          selAttr, 
                          hotkeys,
                          *((Date *)f[i].value),
                          (char *)f[i].data );

    case FRM_LONG    : return scan(*((long *)f[i].value),f[i].len,f[i].width,byte(SCAN_FIELD));

    case FRM_INT     : return scan(*((int *)f[i].value),unsigned(f[i].len),unsigned(f[i].width),byte(SCAN_FIELD));

    case FRM_SHORT   : {
                        int x = *((char *)f[i].value);
                        int ret=scan(x,unsigned(f[i].len),unsigned(f[i].width),byte(SCAN_FIELD));
                        *((char *)f[i].value) = char(x);
                        return ret;
                       }

    case FRM_UNSIGNED: return scan(*((unsigned *)f[i].value),unsigned(f[i].len),unsigned(f[i].width),byte(SCAN_FIELD));

    case FRM_CHAR    : {
                        for(;;)
                          {
                           tsw_cursoroff();
                           KEY k = KB.get();
                           tsw_cursoron();

                           if(hotkeys)
                             for(int j=0;hotkeys[j];j++)
                               if(k==hotkeys[j])
                                 {
                                  scanHotKey = k;
                                  return SF_HOT;
                                 }

                           switch(k)
                             {
                              case KEY_STAB:
                              case KEY_UP  : return SF_UP;
                              case KEY_TAB :
                              case KEY_RET :
                              case KEY_DN  : return SF_DOWN;
                              case KEY_ESC : return SF_ESC;
                              default      : if(f[i].flags & FRM_UPPER) k = KEY(toupper(k));
                                             if(k < 256) *((char *)f[i].value) = k;
                                             direct(f[i].x,f[i].y,selAttr,form("%-*c",f[i].len,*((char *)f[i].value)));
                                             break;
                             }
                          }
                       }
    case FRM_YESNO   : {
                        tsw_selbar(f[i].y+minY-1,f[i].x+minX-1,minX+f[i].x+f[i].len-2,selAttr);

                        for(;;)
                          {
                           tsw_cursoroff();
                           KEY k = KB.get();
                           tsw_cursoron();

                           if(hotkeys)
                             for(int j=0 ; hotkeys[j] ; j++)
                               if(k == hotkeys[j])
                                 {
                                  scanHotKey = k;
                                  return SF_HOT;
                                 }

                           switch(k)
                             {
                              case KEY_RET: return SF_RETURN;
                              case KEY_LT :
                              case KEY_RT :
                              case ' '    : {
                                             char *x = (char *)f[i].value;

                                             *x = char((*x) ? 0 : 1);

                                             direct(f[i].x,f[i].y,selAttr,yesno[*x]);
                                            }
                                            break;
                              case 'Y'    :
                              case 'y'    : {
                                             char *x = (char *)f[i].value;
                                             *x = 1;
                                             direct(f[i].x,f[i].y,selAttr,yesno[*x]);
                                             return SF_RETURN;
                                            }
                              case 'n'    :
                              case 'N'    : {
                                             char *x = (char *)f[i].value;
                                             *x = 0;
                                             direct(f[i].x,f[i].y,selAttr,yesno[*x]);
                                             return SF_RETURN;
                                            }
                              case KEY_STAB:
                              case KEY_UP : return SF_UP;

                              case KEY_TAB:
                              case KEY_DN : return SF_DOWN;

                              case KEY_ESC: return SF_ESC;

                              default     : tsw_beep(2000,50);
                                            break;
                             }
                          }
                       }
    case FRM_CHOICE  : {
                        char **choices = (char **) f[i].data;

                        tsw_selbar( f[ i ].y  +  minY - 1,
                                    f[ i ].x  +  minX - 1,
                                    minX  +  f[ i ].x  +  f[ i ].len  -  2,
                                    selAttr );

                        for(;;)
                          {
                           tsw_cursoroff();
                           KEY k = KB.get();
                           tsw_cursoron();

                           if(hotkeys)
                             for(int j=0;hotkeys[j];j++)
                               if(k==hotkeys[j])
                                 {
                                  scanHotKey=k;
                                  return SF_HOT;
                                 }

                           switch(k)
                             {
                              case KEY_RET:
                              {
                                 char *x = (char *)f[i].value;

                                 int c = tsw_selbox(f[i].x+minX-1,f[i].y+minY-1,tsw_vsize-(f[i].y+minY-1)+1,0xF,choices,SHADOW,*x,
                                    CHISEL_BORDER,
                                    0x07,
                                    NULL,
                                    0x08 );

                                 if(c>=0) *x = char(c);

                                 return SF_RETURN;
                              }

                              case KEY_RT :
                              case ' '    : {
                                             char *x = (char *) f[i].value;
                                             (*x)++;
                                             if(choices[*x] == NULL) *x = 0;
                                             direct(f[i].x,f[i].y,selAttr,form("%-*s",f[i].len,choices[*x]));
                                            }
                                            break;
                              case KEY_LT:  {
                                             char *x = (char *)f[i].value;
                                             (*x)--;
                                             if(*x < 0)
                                               {
                                                for(int j=0;choices[j];j++) {}
                                                *x = char(j-1);
                                               }
                                             direct(f[i].x,f[i].y,selAttr,form("%-*s",f[i].len,choices[*x]));
                                            }
                                            break;

                              case KEY_STAB:
                              case KEY_UP:  return SF_UP;

                              case KEY_TAB:
                              case KEY_DN:  return SF_DOWN;

                              case KEY_ESC: return SF_ESC;

                              default     : tsw_beep(2000,50);
                                            break;
                             }
                          }
                       }
    case FRM_FUNCTION: {
                        KEY k = 0;

                        tsw_selbar(f[i].y+minY-1,f[i].x+minX-1,minX+f[i].x+f[i].len-2,selAttr);

                        for(;;)
                          {
                           tsw_cursoroff();

                           if(helpFunc!=NULLFUNC) (*helpFunc)(i);

                           (*(f[i].func))(0,*this,f[i].x,f[i].y,selAttr,f[i].value);

                           if(f[i].flags & FRM_NOWAIT && !k)
                              k = KEY_RET;
                           else
                              k = KB.get();

                           tsw_cursoron();

                           if(hotkeys)
                             for(int j=0 ; hotkeys[j] ; j++)
                               if(k == hotkeys[j])
                                 {
                                  scanHotKey = k;
                                  return SF_HOT;
                                 }

                           switch(k)
                             {
                              case KEY_STAB:
                              case KEY_UP  : return SF_UP;

                              case KEY_TAB :
                              case KEY_DN  : return SF_DOWN;

                              case KEY_ESC : return SF_ESC;

                              case KEY_RET :
                              default      : if(f[i].flags & FRM_ANYKEY || k==KEY_RET)
                                             {
                                                k = (*(f[i].func))(1,*this,f[i].x,f[i].y,selAttr,f[i].value);
                                                setPos(f[i].x,f[i].y);
                                                if(f[i].flags & FRM_NOWAIT)
                                                {
                                                   KB.push(k);
                                                   continue;
                                                }
                                             }
                                             else
                                             {
                                                tsw_beep(2000,50);
                                                break;
                                             }
                                             return SF_RETURN;
                             }
                          }
                       }
   }

 return SF_ESC;
}


int
FormWindow::process()
{
 ATTR old_fieldattr;
 int& current = cur_field;
 int previous;
 int i,retval = 0;
 KEY *oldhotkeys;

 oldhotkeys = scanHotKeys;

 previous = current;

 for(i=0 ; i<numFields ; i++) showField(i);

 old_fieldattr = fieldAttr;
 fieldAttr     = selAttr;
 scanHotKeys   = hotkeys;

 for(;;)
   {
    if(f[previous].type != FRM_FUNCTION)
      tsw_selbar(f[previous].y+minY-1,f[previous].x+minX-1,minX+f[previous].x+f[previous].width-2,dispAttr);
    tsw_selbar(f[current].y+minY-1,f[current].x+minX-1,minX+f[current].x+f[current].width-2,selAttr);

    if(helpFunc!=NULLFUNC) (*helpFunc)(current);

    setPos(f[current].x,f[current].y);

    int i = editField(current);

    showField(current);

    switch(i)
      {
       case SF_UP:  {
                     previous=current;
                     if(!current) current=numFields-1;
                            else  current--;
                    }
                    break;

       case SF_RETURN:
       case SF_DOWN:{
                     previous = current;

                     if(current == numFields-1) current = 0;
                                            else current++;
                    }
                    break;

       case SF_HOT: retval = ED_HOT;
                    break;

       case SF_ESC: retval = ED_ESC;
                    break;
      }

    if(retval) break;
   }

 fieldAttr   = old_fieldattr;
 scanHotKeys = oldhotkeys;

 return retval;
}

