#include "pb_sdk.h"



int handler( KEY k )
{
     if ( k == KEY_ALTG )
     {
          printf( "\n\7Hang on \3%s\7, I'm shelling to DOS...",
                  UserFirstName );

          MenuFunction( MENU_SHELL, "*!*N*Q*C*Y" );

          puts( "\6I'm back!\7" );

          return HANDLED;
     }


     return NOT_HANDLED;
}



void main()
{
     InstallHandler( HANDLER_SYSOPKEY, handler );

     ExitTSR();
}
