#include "pb_sdk.h"

/************************************************************************/
/*   This is only an example. This program is completely useless :-(    */
/************************************************************************/


char *expr[] = { "GREAT","GOOD","DON'T CARE","DEPRESSING","HATE IT" };

main()
{
     int   age;
     char  c;
     FILE *fp;
     char  buf[ 256 ];
     int   disk;
     unsigned d;
     unsigned t;
     int      handle;
     

     strcpy( buf, "<BLANK>" );

     disk  =  getdisk();
     
     
     printf( "Current disk: %d\n", disk );


     if ( getcurdir( 0, buf )  !=  0 )
     {
          printf( "Can't get current directory\n" );
     }
     else
     {
          printf( "Current  dir: %s\n", buf );
     }


     printf( "Current dir.: %s\n", buf  );

     age  =  LoginDate[ 2 ]  -  CurUser->birthDate[ 2 ];


     fp  =  fopen( "BIRTHDAY.LOG", "at" );

     handle  =  fileno( fp );

     _dos_getftime( handle, & d, & t );

     printf( "File date: %d\n", ( (d >> 9 ) & 0x7F ) + 1980 );


     printf( "\n\7 ** \3CONGRATULATIONS\7 **  How does it feel to be %d years old?\n\n", 
             age );

     printf( "       \0031 \7- \6Great!\n"
             "       \0032 \7- \6Good I guess...\n"
             "       \0033 \7- \6I don't care\n"
             "       \0034 \7- \6Pretty depressing\n"
             "       \0035 \7- \6I hate it!!\n\n"
             "  \7Please select: \3" );


     c  =  WaitKeys( "12345" );


     printf( "\n\n\6Thank you!   Press [Enter] to continue.\t\n", c );


     fprintf( fp,
              "%-35s : Age %-2d -> %s\n",
              CurUser->name,
              age,
              expr[ c - '1' ] );

     fclose( fp );


     return 0;
}
