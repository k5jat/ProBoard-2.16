#include <stdio.h>
#include <string.h>
#include <tslib.hpp>
#include "..\pbkey.hpp"



int decompress_data( byte *input,
                     byte *output,
                     int   isize,
                     int   outsize );




#define KEY_1  0x9C
#define KEY_2  0x5A
#define KEY_3  0xF3


byte *decryptData( byte  *buf, 
                   short  len,
                   byte   key )
{
     short i;
     byte  c;
     byte  shift = 0;
     byte  iPrime;
     byte  nKey;

     (void) shift;

     iPrime  =  0x17;
     nKey    =  ( byte ) ~key;


     for ( i = 0;  i < len;  i++, key++, iPrime++, nKey++ )
     {
          c   =  ( buf[ i ]  ^  nKey )  ^  iPrime;     // Get encrypted char  


          //--------------------------------------------------------------
          // Now let's do the bit shifting (rolling) to the left.  We used
          // to do this in a loop, but doing it this way avoids the loop
          // overhead and speeds things up at the cost of slightly larger
          // code size.
          //--------------------------------------------------------------
          
          shift  =  ( byte )  ( i & 0x07 );
          
          
          if ( shift != 0 )
          {
               c  =  ( byte )  ( ( c  <<  shift )   |   
                                 ( c  >>  ( 8 - shift ) ) );
          }
          
          
          //-----------------------------------
          // Now let's perform the bit swapping
          //-----------------------------------
          
          c  =  ( byte )  ~(   ( c             >> 7  )  | 
                               ( c             << 7  )  |
                             ( ( c  &  0x40 )  >>  5 )  | 
                             ( ( c  &  0x02 )  <<  5 )  |
                             ( ( c  &  0x20 )  >>  3 )  |
                             ( ( c  &  0x04 )  <<  3 )  |
                             ( ( c  &  0x10 )  >>  1 )  | 
                             ( ( c  &  0x08 )  <<  1 ) );


          buf[ i ]  =  ( byte )  ( ( c ^ key )  ^  0x49 );       // Store real char in buf 
     }


     return( buf );
}



static
byte *encryptData( byte  *buf, 
                   short  len,
                   byte   key )
{
     short i;
     byte  c;
     byte  shift = 0;
     byte  iPrime;
     byte  nKey;

     (void) shift;

     iPrime  =  0x17;
     nKey    =  ( byte ) ~key;


     for ( i = 0;   i  <  ( short ) len;   i++, key++, iPrime++, nKey++ )
     {
          //------------------
          // Get the character
          //------------------

          c   =  ( byte )  ( ( buf[ i ]  ^  key )  ^  0x49 );               


          //-----------------------------------
          // Now let's perform the bit swapping
          //-----------------------------------
          
          c  =  ( byte )  ~(   ( c           >>  7 )  |
                               ( c           <<  7 )  | 
                             ( ( c & 0x40 )  >>  5 )  | 
                             ( ( c & 0x02 )  <<  5 )  | 
                             ( ( c & 0x20 )  >>  3 )  | 
                             ( ( c & 0x04 )  <<  3 )  |
                             ( ( c & 0x10 )  >>  1 )  | 
                             ( ( c & 0x08 )  <<  1 ) );
          
          
          //----------------------------------------------------------------
          // Now let's do the bit shifting (rolling) to the right.  We used
          // to do this in a loop, but doing it this way eliminates the loop
          // code overhead and consequently speeds things up a bit.
          //----------------------------------------------------------------
          
          shift  =  ( byte ) ( i & 0x07 );
          
          
          if ( shift != 0 )
          {
               c  =  ( byte )  ( ( c  >>  shift )  |
                                 ( c  <<  ( 8 - shift ) ) );
          }

          buf[ i ]  =  ( c ^ iPrime )  ^  nKey;
     }


     return( buf );
}



void ReverseBuffer( byte *pData,
                    int   cLength )
{
     int iFirst = 0;
     int iLast  = cLength - 1;


     for ( ;  iFirst < cLength / 2;  iFirst++, iLast-- )
     {
          byte iChar;


          iChar            =  pData[ iLast ];
          pData[ iLast  ]  =  pData[ iFirst ];
          pData[ iFirst ]  =  iChar;
     }
}


//**************************************************************************
//
// Generate an Upgrade-ID code given the Sysop and BBS names
//
//     Prototype:  void GenerateUpgradeID( const char *pSysopName, const
//                                         char *pBbsName, char 
//                                         *pUpgradeID );
//
//    Parameters:  pSysopName ... Sysop name to build Upgrade-ID from
//                 pBbsName   ... BBS name to build Upgrade-ID from
//                 pUpgradeID ... Filled-in with the 8-byte Upgrade-ID
//
//       Returns:  Nothing
//
//       Remarks:
//
// This routine uses a semi-complex algorithm to generate an Upgrade-ID that
// should be relatively successful at making the Sysop Name and BBS name
// sufficiently hard to crack in the upgrade process.  
//
// Here's how the algorithm works:
//
//   A) Concatenate the Sysop name and the BBS name.
//
//   B) Encrypt the string generated in (A) using a unique key (KEY1)
//   
//   C) Reverse all the bytes in the buffer
//   
//   D) Encrypt the string again, using a different key (KEY2)
//
//   E) Reverse all the bytes in the buffer again
//
//   F) Calculate a 32-bit CRC based on the data in (E)
//
//   G) Encrypt the CRC using another key (KEY3).
//
// -------------------------------------------------------------------------
//
//    Created on:  08/09/99 (Jeff Reeder)
// Last modified:  08/09/99 (Jeff Reeder)   Created
//
//**************************************************************************

void GenerateUpgradeID( const char *pSysopName,
                        const char *pBbsName,
                              char *pUpgradeID )
{
     char  pFullText[ 200 ];
     int   cLength;
     dword crc;


     //------------------------------------------------  
     // A) Concatenate the Sysop name and the BBS name.
     //------------------------------------------------  

     strcpy( pFullText, pSysopName );
     strcat( pFullText, pBbsName   );

     cLength  =  strlen( pFullText )  +  1;



     {
          int  i;
          long iValue = 0L;
          long iCRC;


          for ( i = 0;  i < strlen( pFullText );  i++ )
          {
               iValue  +=  pFullText[ i ];
          }



          iCRC  =  ( ( ( ( ( (~iValue) >> 24 ) & 0xFF )  ^  0x17 )  &  0xFF
)  <<  24 )  |
                   ( ( ( ( (   iValue  >> 16 ) & 0xFF )  ^  0x62 )  &  0xFF
)  <<  16 )  |
                   ( ( ( ( (   iValue  >>  8 ) & 0xFF )  ^  0x9C )  &  0xFF
)  <<  8  )  |
                   ( ( ( ( ( (~iValue)       ) & 0xFF )  ^  0xCB )  &  0xFF
)         );

          sprintf( pUpgradeID,
                   "%04lX%04lX", 
		   ( ( iCRC >> 16 )  & 0xFFFF ),
		   ( ( iCRC       )  & 0xFFFF ) );
     }

//     //-----------------------------------------------------------------  
//     // B) Encrypt the string generated in (A) using a unique key (KEY1)
//     //-----------------------------------------------------------------  
//
//     encryptData( pFullText,
//                  cLength,
//                  KEY_1 );
//
//
//     //---------------------------------------  
//     // C) Reverse all the bytes in the buffer
//     //---------------------------------------  
//
//     ReverseBuffer( pFullText, cLength );
//
//
//     //----------------------------------------------------------  
//     // D) Encrypt the string again, using a different key (KEY2)
//     //----------------------------------------------------------  
//
//     encryptData( pFullText,
//                  cLength,
//                  KEY_2 );
//
//
//     //---------------------------------------------  
//     // E) Reverse all the bytes in the buffer again
//     //---------------------------------------------  
//
//     ReverseBuffer( pFullText, cLength );
//
//
//     //---------------------------------------------------  
//     // F) Calculate a 32-bit CRC based on the data in (E)
//     //---------------------------------------------------  
//
//     crc  =  crc32( pFullText, cLength );
//
//
//     //---------------------------------------------  
//     // G) Encrypt the CRC using another key (KEY3).
//     //---------------------------------------------  
//
//     encryptData( (byte *) & crc,
//                  sizeof( dword ),
//                  KEY_3 );
//
//
//     sprintf( pUpgradeID, 
//              "%08lX",
//              crc );
}



int main( void )
{
     int      i;
     int      output_size;
     File     f;
     KeyFile *kf            =  new KeyFile;


     if ( ! f.open( "REGKEY.PB" ) )
     {
          printf( "Key file \"REGKEY.PB\" cannot found.  Make sure you run this program\n" );
          printf( "from within the ProBoard directory where the key file is stored.\n" );
          printf( "\n" );
          printf( "NOTE: This program is only to be used for ProBoard customers using version\n" );
          printf( "      2.16 or earlier.  As of ProBoard v2.2, key files are no longer used.\n" );

          return 1;
     }


     output_size  =  int( f.len() - 33 );


     byte *output_data  =  new byte[ output_size ];


     f.seek( 33 );

     f.read( output_data, output_size );

     f.close();


     for ( i = 0;  i < output_size;  i++ )
     {
          output_data[ i ]  ^=  fixed_xor_data[ i % 100 ];
     }


     decompress_data( output_data, 
                      (byte *) kf, 
                      output_size,
                      sizeof( *kf ) );


     encrypt( (char *) kf->keyData,
              sizeof( kf->keyData ),
              kf->xorData );


     bool valid = TRUE;


     if( kf->keyCrc  !=  crc32( kf->keyData,  sizeof( kf->keyData ) )  || 
         kf->xorCrc  !=  crc32( kf->xorData,  sizeof( kf->xorData ) ) )
     {
          valid = FALSE;
     }


     if ( kf->securityCheck[ regSelectedTable ]  !=  calc_security( & kf->keyData[ 0 ],
                                                                    regTable1, 
                                                                    regTable2 ) )
     {
          valid = FALSE;
     }


     printf( "\n" );


     if ( valid )
     {
          char     pUpgradeID[ 10 ];
          KeyData *key = kf->keyData;


          GenerateUpgradeID( key->sysopName,
                             key->bbsName,
                             pUpgradeID );


          printf( "To get your new copy of ProBoard unlocked, you'll have to contact TeleGrafix\n" );
          printf( "and purchase an upgrade to ProBoard v2.2.  You'll need to provide the following\n" );
          printf( "information while you are filling out the upgrade form:\n" );
          printf( "\n" );
          printf( "              SysOp name: \"%s\"\n", key->sysopName );
          printf( "                BBS name: \"%s\"\n", key->bbsName   );
          printf( "         Upgrade-ID Code: \"%-4.4s-%-4.4s\"\n", 
                  pUpgradeID,
                  pUpgradeID + 4 );

          printf( "\n" );
          printf( "This Upgrade-ID Code is VERY important.  It tells our upgrade center that you\n" );
          printf( "are a legitimate ProBoard customer, and that you are entitled to your upgrade\n" );
          printf( "to the latest version without having to buy a whole new copy of ProBoard.\n" );
          printf( "Do not lose this code, and when you tell our upgrade center, you'll have to\n" );
          printf( "type in all the above information EXACTLY as you see it above.  Failing to\n" );
          printf( "do so will cause our upgrade center to reject your upgrade request.\n" );
          printf( "\n" );
          printf( "You may upgrade ProBoard by purchasing an upgrade from the following:\n" );
          printf( "\n" );
          printf( "     TeleGrafix Communications, Inc.         Voice: (540) 678-4050\n" );
          printf( "     2180 S. Loudoun, PMB #308               Sales: (540) 678-4051\n" );
          printf( "     Winchester, VA 22601                      Fax: (540) 678-4052\n" );
          printf( "                                             Email: sales@telegrafix.com\n" );
          printf( "                                               Web: www.telegrafix.com\n" );
     }
     else
     {
          printf( "\n\nThis is an illegal key file.  This copy of ProBoard does not appear!\n" );
          printf( "to be a legitimate license, and cannot be upgraded.  You'll have to\n" );
          printf( "purchase a full copy of the software.\n" );
     }                                         

     return 0;
}



