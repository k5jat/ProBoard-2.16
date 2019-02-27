#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#endif

#include <tswin.hpp>



PopMenu::PopMenu()
{
     items     = NULL;
     hotkeys   = NULL;
     bar_attr  = 0;
     high_attr = 0;
     current   = 0;
     action    = NULLFUNC;
}



PopMenu::PopMenu( menu_item *i,
                  ATTR       b_attr,
                  ATTR       h_attr,
                  KEY       *hot,
                  void       ( *func ) ( int ) )
{
     items     = i;
     hotkeys   = hot;
     bar_attr  = b_attr;
     high_attr = h_attr;
     current   = 0;
     action    = func;
}



void PopMenu::define( menu_item *i,
                      ATTR       b_attr,
                      ATTR       h_attr,
                      KEY       *hot,
                      void       ( *func ) ( int ) )
{
     items     = i;
     hotkeys   = hot;
     bar_attr  = b_attr;
     high_attr = h_attr;
     current   = 0;
     action    = func;
}



int PopMenu::process()
{
     int previous = current;
     int i;

     int x1 = minX;
     int y1 = minY;
     int x2 = maxX;


     for ( num_entries = 0;  items[ num_entries ].s;  num_entries++ )
     {
          // DO NOTHING
     }


     clear();


     for ( i = 0;  i < num_entries;  i++ )
     {
          tsw_mputs( x1,
                     y1 + i,
                     items[ i ].s );
     

          if ( items[ i ].highlight1 )
          {
               tsw_selbar( y1  +  i,
                           x1  +  items[ i ].highlight1  -  1,
                           x1  +  items[ i ].highlight2  -  1,
                           high_attr );
          }
     }


     for ( ; ; )
     {
          KEY c;


          setPos( 1,  current + 1 );

          tsw_selbar( previous + y1,
                      x1,
                      x2, 
                      attr );


          if ( items[ previous ].highlight1 )
          {
               tsw_selbar( y1  +  previous,
                           x1  +  items[ previous ].highlight1  -  1,
                           x1  +  items[ previous ].highlight2  -  1,
                           high_attr );
          }


          tsw_selbar( current + y1,
                      x1,
                      x2,
                      bar_attr );

          previous = current;

          
          if ( action != NULLFUNC ) 
          {
               ( *action ) ( current );
          }


          c = KB.uget();


          if ( c == KEY_ESC )
          {
               return 0;
          }


          if ( c == KEY_RET )
          {
               return( current + 1 );
          }


          if ( hotkeys )
          {
               for ( i = 0;  hotkeys[ i ];  i++ )
               {
                    if ( c  ==  hotkeys[ i ] )
                    {
                         hotkey = c;

                         return SL_HOTKEY;
                    }
               }
          }


          for ( i = 0;  i < num_entries;  i++ )
          {
               if ( items[ i ].hotkey  && 
                    items[ i ].hotkey  ==  c )
               {
                    current = i;

                    tsw_selbar( previous + y1,
                                x1,
                                x2,
                                attr );

                    if ( items[ previous ].highlight1 )
                    {
                         tsw_selbar( y1 + previous,
                                     x1 + items[ previous ].highlight1  -  1,
                                     x1 + items[ previous ].highlight2  -  1,
                                     high_attr );
                    }


                    tsw_selbar( current + y1,
                                x1,
                                x2,
                                bar_attr );

                    return  i + 1;
               }
          }


          switch ( c )
          {
               case KEY_UP:
               
                    current  =  current 
                                   ?  current - 1 
                                   :  num_entries - 1;
                    break;


               case KEY_DN:
               
                    current  =  ( current  ==  num_entries - 1 )
                                   ?  0 
                                   :  current + 1;
                    break;


               case KEY_HOME:
               case KEY_PGUP: 
               
                    current = 0;
                    break;


               case KEY_END:
               case KEY_PGDN:
               
                    current  =  num_entries-1;
                    break;
          }
     }
}



void PopMenu::refresh( int n )
{
     current  =  n - 1;
}

