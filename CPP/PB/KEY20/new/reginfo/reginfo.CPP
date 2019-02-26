#include <stdio.h>
#include <string.h>
#include <tslib.hpp>
#include "..\pbkey.hpp"
    
typedef unsigned char byte;
typedef unsigned short word;


static 
word crc_table_16[ 256 ] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};



static
word makeCRC16( word  count,
                word  crc,
                byte *buffer )
{
     byte *p = buffer;


     while ( count--  !=  0 )
     {
          crc = word( ( crc << 8 )  ^  crc_table_16[ ( crc >> 8 )  ^  *p++ ] );
     }

     return crc;
}


short decompress_data( byte *input,
                     byte *output,
                     short   isize,
                     short   outsize );




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
                    short   cLength )
{
     short iFirst = 0;
     short iLast  = cLength - 1;


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
     short cLength;


     //------------------------------------------------  
     // A) Concatenate the Sysop name and the BBS name.
     //------------------------------------------------  

     strcpy( pFullText, pSysopName );
     strcat( pFullText, pBbsName   );

     cLength  =  strlen( pFullText )  +  1;



#if 1


     {
          int  i;
          long iValue = 0L;
          long iCRC;


          for ( i = 0;  i < strlen( pFullText );  i++ )
          {
               iValue  +=  pFullText[ i ];
          }



          iCRC  =  ( ( ( ( ( (~iValue) >> 24 ) & 0xFF )  ^  0x17 )  &  0xFF )  <<  24 )  |
                   ( ( ( ( (   iValue  >> 16 ) & 0xFF )  ^  0x62 )  &  0xFF )  <<  16 )  |
                   ( ( ( ( (   iValue  >>  8 ) & 0xFF )  ^  0x9C )  &  0xFF )  <<  8  )  |
                   ( ( ( ( ( (~iValue)       ) & 0xFF )  ^  0xCB )  &  0xFF )         );

          sprintf( pUpgradeID,
                   "%08lX", iCRC );
     }

#else


     //-----------------------------------------------------------------  
     // B) Encrypt the string generated in (A) using a unique key (KEY1)
     //-----------------------------------------------------------------  

     encryptData( pFullText,
                  cLength,
                  KEY_1 );


     //---------------------------------------  
     // C) Reverse all the bytes in the buffer
     //---------------------------------------  

     ReverseBuffer( pFullText, cLength );


     //----------------------------------------------------------  
     // D) Encrypt the string again, using a different key (KEY2)
     //----------------------------------------------------------  

     encryptData( pFullText,
                  cLength,
                  KEY_2 );


     //---------------------------------------------  
     // E) Reverse all the bytes in the buffer again
     //---------------------------------------------  

     ReverseBuffer( pFullText, cLength );

     printf( "%d %d %d %d", 
             pFullText[ 0 ], 
             pFullText[ 1 ], 
             pFullText[ 2 ],
             pFullText[ 3 ] );


     //---------------------------------------------------  
     // F) Calculate a 32-bit CRC based on the data in (E)
     //---------------------------------------------------  

     crc  =  crc32( pFullText, cLength );


     //---------------  
     // G) Encrypt the CRC using another key (KEY3).
     //---------------------------------------------  


     {
          char pOrigText[ 256 ];
          word crc1, crc2;

          strcpy( pOrigText, pSysopName );
          strcat( pOrigText, pBbsName   );

          crc1  =  (word) ( ~ makeCRC16( strlen( pFullText ),
                                         0L,
                                         (byte *) pFullText ) );

          crc2  =  (word) ( makeCRC16( strlen( pOrigText ),
                                       0L,
                                       (byte *) pOrigText )  ^  0x372C );

          sprintf( pUpgradeID, "%04X%04X", (int) crc1, (int) crc2 );

          //sprintf( pUpgradeID,
          //         "%02X%02X%02X%02X",
          //         (unsigned) ( ( crc1 >> 8 ) & 0xFF ),
          //         (unsigned) ( ( crc1      ) & 0xFF ),
          //         (unsigned) ( ( crc2 >> 8 ) & 0xFF ),
          //         (unsigned) ( ( crc2      ) & 0xFF ) );

          // encryptData( (byte *) & crc,
          //              sizeof( dword ),
          //              KEY_3 );


          //sprintf( pUpgradeID, 
          //         "%08lX",
          //         crc );
     }

#endif
}



int main( void )
{
     short      i;
     short      output_size;
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


     output_size  =  (short)( f.len() - 33 );


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
          char     pUpgradeID[ 128 ];
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



