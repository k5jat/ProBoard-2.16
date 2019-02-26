#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#endif

#include <tswin.hpp>
#include <string.h>



void tsw_clearrect(int x1,int y1,int x2,int y2,ATTR attr)
{
     tsw_fillrect(x1,y1,x2,y2,32,attr);
}



void tsw_selbar(int line,int x1,int x2,ATTR attr,int lines)
{
     tsw_changeatt(attr,x1,line,x2,line+lines-1);
}



void tsw_fillscreen(char c,ATTR attr)
{
     tsw_fillrect(1,1,tsw_hsize,tsw_vsize,c,attr);
}



void tsw_drawbox( int         x1,
                  int         y1,
                  int         x2,
                  int         y2,
                  const char *c,
                  ATTR        attr )
{
     int i;


     for ( i = x1 + 1;  i < x2;  i++ )
     {
          tsw_maputc( i, y1, attr, c[ 1 ] );
          tsw_maputc( i, y2, attr, c[ 5 ] );
     }


     for ( i = y1 + 1;  i < y2; i++ )
     {
          tsw_maputc( x1, i, attr, c[ 7 ] );
          tsw_maputc( x2, i, attr, c[ 3 ] );
     }


     tsw_maputc( x1, y1, attr, c[ 0 ] );
     tsw_maputc( x2, y1, attr, c[ 2 ] );
     tsw_maputc( x1, y2, attr, c[ 6 ] );
     tsw_maputc( x2, y2, attr, c[ 4 ] );
}



void tsw_chiselbox( int         x1,
                    int         y1,
                    int         x2,
                    int         y2,
                    const char *c,
                    ATTR        bright,
                    ATTR        dark )
{
     int i;


     for ( i = x1 + 1;  i < x2;  i++ )
     {
          tsw_maputc( i, y1, bright, c[ 1 ] );
          tsw_maputc( i, y2, dark,   c[ 5 ] );
     }


     for ( i = y1 + 1;  i < y2; i++ )
     {
          tsw_maputc( x1, i, bright, c[ 7 ] );
          tsw_maputc( x2, i, dark,   c[ 3 ] );
     }


     tsw_maputc( x1, y1, bright, c[ 0 ] );
     tsw_maputc( x2, y1, dark,   c[ 2 ] );
     tsw_maputc( x1, y2, bright, c[ 6 ] );
     tsw_maputc( x2, y2, dark,   c[ 4 ] );
}



void tsw_recessbox( int         x1,
                    int         y1,
                    int         x2,
                    int         y2,
                    const char *c,
                    ATTR        bright,
                    ATTR        dark,
                    ATTR        inner,
                    ATTR        outer )
{
     int i;


     for ( i = x1 + 1;  i < x2;  i++ )
     {
          tsw_maputc( i, 
                      y1, 
                      ( outer & 0xF0 )  |  ( bright & 0x0F ),
                      c[ 1 ] );

          tsw_maputc( i, 
                      y2, 
                      ( outer & 0xF0 )  |  ( dark & 0x0F ),
                      c[ 5 ] );
     }


     for ( i = y1 + 1;  i < y2; i++ )
     {
          //tsw_maputc( x1 - 1, 
          //            i, 
          //            outer,
          //            c[ 7 ] );

          tsw_maputc( x1, 
                      i, 
                      ( inner & 0xF0 )  |  ( bright & 0x0F ),
                      c[ 7 ] );

          tsw_maputc( x2, 
                      i, 
                      ( inner & 0xF0 )  |  ( dark & 0x0F ),
                      c[ 3 ] );

          //tsw_maputc( x2 + 1, 
          //            i, 
          //            outer,
          //            c[ 3 ] );
     }


     tsw_maputc( x1, 
                 y1, 
                 ( outer & 0xF0 )  |  ( bright & 0x0F ),
                 c[ 0 ] );

     tsw_maputc( x2, 
                 y1, 
                 ( outer & 0xF0 )  |  ( bright & 0x0F ),
                 c[ 2 ] );

     tsw_maputc( x1, 
                 y2, 
                 ( outer & 0xF0 )  |  ( dark & 0x0F ),
                 c[ 6 ] );

     tsw_maputc( x2, 
                 y2, 
                 ( outer & 0xF0 )  |  ( dark & 0x0F ),
                 c[ 4 ] );
}



//**************************************************************************
//
// Show a footer message, with optional highlighting
//
//     Prototype:  void tsw_showfooter( char *str, ATTR attr, ATTR high );
//
//    Parameters:  str ... Message to display
//
//       Returns:  Nothing
//
//       Remarks:
//
// This routine has been updated to support the optional highlighting of
// key phrases in the text.  Highlighting is toggled by placing the
// character 0x01 before a sequence.  Colors start with <attr> until toggled
// with 0x01.  A highlight sequence will continue until another toggle
// character is found.
//
// The message is centered, ignoring any highlight marker characters.
//
// -------------------------------------------------------------------------
//
//    Created on:  ??/??/???? (Philippe Leybaert)
// Last modified:  08/06/1999 (Jeff Reeder)   Modified to support
//                            highlighting of hotkey characters, and the
//                            like.
//
//**************************************************************************

void tsw_showfooter( char *str,
                     ATTR  attr,
                     ATTR  high )
{
     char buf[ 128 ];
     char *ptr = buf;
     char *pos = str;
     int   cLength;
     ATTR  iCurAttr = attr;
     int   cur = 0;


     if ( high == 0 )
     {
          //---------------------------------------------------------  
          // We have no highlight attribute.  Use the basic attribute
          //---------------------------------------------------------  

          high = attr;
     }

     
     //-------------------------------------  
     // Copy over all non-control characters
     //-------------------------------------  

     while (  pos  &&
             *pos )
     {
          if ( *pos >= 32 )
          {
               *ptr++ = *pos++;
               *ptr   = '\0';
          }
          else
          {
               pos++;
          }
     }


     //-----------------------------  
     // How long is the raw message?
     //-----------------------------  

     cLength  =  strlen( buf );


     //-----------------------------------------------------  
     // Erase the footer area to the default color attribute
     //-----------------------------------------------------  

     tsw_clearrect( 1,
                    tsw_vsize,
                    tsw_hsize,
                    tsw_vsize,
                    attr );


     //----------------------------  
     // Here's where the fun begins
     //----------------------------  

     int x  =  tsw_hsize / 2  +  1  -  cLength / 2;



     ptr = str;


     while ( ptr  &&
             *ptr )
     {
          if ( *ptr == 0x01 )
          {
               if ( cur == 0 )
               {
                    iCurAttr = high;
               }
               else
               {
                    iCurAttr = attr;
               }


               cur  =  ! cur;
          }
          else
          {
               tsw_maputc( x++,
                           tsw_vsize,
                           iCurAttr,
                           *ptr );
          }


          ptr++;
     }
}



void tsw_clearfooter( ATTR attr )
{
     tsw_clearrect( 1,
                    tsw_vsize,
                    tsw_hsize,
                    tsw_vsize,
                    attr );
}
