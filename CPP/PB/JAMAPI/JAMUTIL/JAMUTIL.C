/*
**  JAM(mbp) - The Joaquim-Andrew-Mats Message Base Proposal
**
**  JAM Utility
**
**  Written by Mats Wallin & Mats Birch
**
**  ----------------------------------------------------------------------
**
**  jamutil.c (JAMmb)
**
**  JAM Utility, display contents of a JAM messagebase
**
**  Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and
**  Mats Wallin. ALL RIGHTS RESERVED.
**
**  93-06-28    MW
**  Initial coding
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__MSDOS__)
#include <fcntl.h>
#include <io.h>
#endif

#if defined(__sparc__)
#include <fcntl.h>
#include <unistd.h>
#endif

#include "jammb.h"

#include "jamutil.h"


/*
**  Prototypes
*/

int InitJAMAPIREC( JAMAPIREC * pJam, CHAR8 * pFile );
int DisplayMsgHdr( INT32 MsgNo );
int DisplayMsgSubFld( void );
int DisplayMsgTxt( void );
int DisplayHdrInfo( void );
CHAR8 * AttrToStr( UINT32 Attr );
CHAR8 * GetSubFldName( JAMSUBFIELD * pSubFld );
CHAR8 * GetSubFldStr( JAMSUBFIELD * pSubFld );
CHAR8 * DispTime( UINT32 * pt );

#if !defined(__MSDOS__)
UINT32 filelength( int fh );
#endif


/* ---------------------------------------------------------------------- *
 *
 *  main
 *
 * ---------------------------------------------------------------------- */

int main( int argc, CHAR8 * argv [] )
{
  CHAR8 JamFileName [128];
  INT32 MsgNo;
  int   ch,
        done;

  puts( "JAMutil\n"
        "Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and\n"
        "               Mats Wallin. ALL RIGHTS RESERVED\n"
        "Written by Mats Wallin & Mats Birch\n" );

  if( argc < 2 || argc > 3 )
    {
    puts( "Format: JAMUTIL <JAMfile> [<MsgNo>]\n" 
          "where: <JAMfile> is the path and base filename to the JAM messagebase\n"
          "       <MsgNo>   is the first message number to display\n" );
    return( 1 );
    }


  strcpy( JamFileName, argv [1] );
  if( argc == 3 )
    MsgNo = atol( argv [2] );
  else
    MsgNo = 0;


/*
**  Initiate the JAMAPIREC variable, and open the JAM messagebase
*/

  if( !JAMsysInitApiRec( &JamRec, JamFileName, WORKBUFSIZE ))
    {
    puts( "Out of memory" );
    return( 1 );
    }

  if( !JAMmbOpen( &JamRec ))
    {
    printf( "Unable to open messagebase: %s, code: %d, errno: %d\n", JamFileName, JamRec.APImsg, JamRec.Errno );
    return( 1 );
    }


/*
**  Display the specified (or first if none were specified) message in this
**  messagebase
*/

  if( MsgNo == 0L )
    MsgNo = JamRec.HdrInfo.BaseMsgNum;

  DisplayMsgHdr( MsgNo );



/*
**  Display a prompt, and let the user choose what to do next
*/

  while( 1 )
    {
    printf( "\nQ=Quit  S=Subfields  T=Text  N=Next  P=Previous  H=HdrInfo   :" );

    do
      {
      ch = getchar();
      ch = toupper( ch );
      done = 1;

      switch( ch )
        {
        case  'Q' :
        case  27 :
          puts( "Quit\n" );
          return( 0 );

        case 'H' :
          puts( "HdrInfo\n" );
          DisplayHdrInfo( );
          break;

        case 'P' :
          puts( "Prev\n" );
          DisplayMsgHdr( --MsgNo );
          break;

        case 'S' :
          puts( "SubFld\n" );
          DisplayMsgSubFld( );
          break;
      
        case 'T' :
          puts( "Text\n" );
          DisplayMsgTxt( );
          break;

        case 'N' :
        case 13 :
          puts( "Next\n" );
          DisplayMsgHdr( ++MsgNo );
          break;

        case '0' :
        case '1' :
        case '2' :
        case '3' :
        case '4' :
        case '5' :
        case '6' :
        case '7' :
        case '8' :
        case '9' :
          {
          CHAR8   Buf [16];

          putchar( ch );
          Buf [0] = ( CHAR8 ) ch;
          fgets( Buf + 1, sizeof( Buf ) - 1, stdin );
          putchar( '\n' );

          MsgNo = atol( Buf );
          DisplayMsgHdr( MsgNo );
          break;
          }

        default :
          done = 0;
          break;
        }
      }
    while( !done );
    }


  return( 0 );
}


/* ---------------------------------------------------------------------- *
 *
 *  DisplayMsgHdr
 *
 *    Displays all header information for the specified message.
 *    The message header is first read from the messagebase
 *
 *    Parameters:   INT32 MsgNo   - The message to display
 *
 *    Returns:      0             - Failure
 *                  1             - Success
 *
 * ---------------------------------------------------------------------- */

int DisplayMsgHdr( INT32 MsgNo )
{
  printf( "Message number: %lu\n\n", MsgNo );


/*
**  Check that the messagenumber where valid
*/

  if(( MsgNo - JamRec.HdrInfo.BaseMsgNum ) * sizeof( JAMIDXREC ) >= filelength( JamRec.IdxHandle ))
    {
    puts( "No more messages" );
    return( 0 );
    }

  if( MsgNo < JamRec.HdrInfo.BaseMsgNum )
    {
    printf( "Invalid message number: %ld\n", MsgNo );
    return( 0 );
    }


/*
**  Read the message header (but we don't want to read the subfields)
*/
                              
  if( !JAMmbFetchMsgHdr( &JamRec, MsgNo, 0 ))
    {
    printf( "Error reading message header, code: %d, errno: %d\n", JamRec.APImsg, JamRec.Errno );
    return( 0 );
    }


/*
**  Display the header contents
*/

  printf( "Signature    : <%02x><%02x><%02x><%02x>\n",
                                    JamRec.Hdr.Signature [0],
                                    JamRec.Hdr.Signature [1],
                                    JamRec.Hdr.Signature [2],
                                    JamRec.Hdr.Signature [3] );
  printf( "Revision     : %04xh\n", JamRec.Hdr.Revision );
  printf( "SubfieldLen  : %lu\n",   JamRec.Hdr.SubfieldLen );
  printf( "TimesRead    : %lu\n",   JamRec.Hdr.TimesRead );
  printf( "MsgIdCRC     : %08lx\n", JamRec.Hdr.MsgIdCRC );
  printf( "ReplyCRC     : %08lx\n", JamRec.Hdr.ReplyCRC );
  printf( "ReplyTo      : %lu\n",   JamRec.Hdr.ReplyTo );
  printf( "Reply1st     : %lu\n",   JamRec.Hdr.Reply1st );
  printf( "ReplyNext    : %lu\n",   JamRec.Hdr.ReplyNext );
  printf( "DateWritten  : %s",      JamRec.Hdr.DateWritten ? DispTime( &JamRec.Hdr.DateWritten ) : "<null>\n" );
  printf( "DateReceived : %s",      JamRec.Hdr.DateReceived ? DispTime( &JamRec.Hdr.DateReceived ) : "<null>\n" );
  printf( "DateProcessed: %s",      JamRec.Hdr.DateProcessed ? DispTime( &JamRec.Hdr.DateProcessed ) : "<null>\n" );
  printf( "MsgNum       : %lu\n",   JamRec.Hdr.MsgNum );
  printf( "Attribute    : %s\n",    AttrToStr( JamRec.Hdr.Attribute ));
  printf( "Attribute2   : %lu\n",   JamRec.Hdr.Attribute2 );
  printf( "TxtOffset    : %lu\n",   JamRec.Hdr.TxtOffset );
  printf( "TxtLen       : %lu\n",   JamRec.Hdr.TxtLen );
  printf( "PasswordCRC  : %08lx\n", JamRec.Hdr.PasswordCRC );
  printf( "Cost         : %u\n", JamRec.Hdr.Cost );

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  DisplayMsgSubFld
 *
 *    Displays all subfields in the message header of the current
 *    message. The message header is first read once more, to make
 *    sure that we have the subfields in the buffer. (The same buffer
 *    is used when reading the message text).
 *
 *    Parameters:   None
 *
 *    Returns:      0                   - Failure
 *                  1                   - Success
 *
 * ---------------------------------------------------------------------- */

int DisplayMsgSubFld( void )
{
  UINT32  SubFldLen = JamRec.Hdr.SubfieldLen,
          Len;

  printf( "Message number: %lu\n\n", JamRec.Hdr.MsgNum );


/*
**  Read the message header (and subfields)
*/

  if( !JAMmbFetchMsgHdr( &JamRec, JamRec.Hdr.MsgNum, 1 ))
    {
    printf( "Error reading message header, code: %d, errno: %d\n", JamRec.APImsg, JamRec.Errno );
    return( 0 );
    }


/*
**  If it wasn't possible to read all subfields into the buffer, only
**  process those that fit into the buffer
*/

  if( SubFldLen > JamRec.WorkLen )
    SubFldLen = JamRec.WorkLen;


/*
**  Display all the subfields
*/

  puts( " HiID LoID Name             Len Data\n"
        " ---- ---- ----             --- ----" );

  for(  JamRec.SubFieldPtr = ( JAMSUBFIELD * ) JamRec.WorkBuf;

        JamRec.SubFieldPtr->DatLen + sizeof( JAMBINSUBFIELD ) <= SubFldLen;

        Len = JAMsysAlign( JamRec.SubFieldPtr->DatLen + sizeof( JAMBINSUBFIELD )),
        SubFldLen -= Len,
        JamRec.SubFieldPtr = JAMsysAddPtr( JamRec.SubFieldPtr, Len ))
    {
    printf( "%5u%5u %-14.14s%6lu \"%s\"\n",
                        JamRec.SubFieldPtr->HiID, JamRec.SubFieldPtr->LoID,
                        GetSubFldName( JamRec.SubFieldPtr ),
                        JamRec.SubFieldPtr->DatLen,
                        GetSubFldStr( JamRec.SubFieldPtr ));
    }

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  DisplayMsgTxt
 *
 *    Displays the text of the current message. The message text is
 *    first read. For message texts that doesn't fit into the work-
 *    buffer, the text is read part by part
 *
 *    Parameters:   None
 *
 *    Returns:      0                   - Failure
 *                  1                   - Success
 *
 * ---------------------------------------------------------------------- */

int DisplayMsgTxt( void )
{
  int       First = 1,          /* Flag if this is the first read */
            Col = 0;
  UINT32    Pos = 0;
  UCHAR8  * p;

  printf( "Message number: %lu\n\n", JamRec.Hdr.MsgNum );

  while( 1 )
    {

/*
**  Get one part of the message
*/

    if( !JAMmbFetchMsgTxt( &JamRec, First ))
      {
      printf( "Error reading message text, code: %d, errno: %d\n", JamRec.APImsg, JamRec.Errno );
      return( 0 );
      }

    if( JamRec.APImsg == JAMAPIMSG_NOMORETEXT )
      break;

    First = 0;


/*
**  Display the message text
*/

    for( p = ( UCHAR8 * ) JamRec.WorkBuf; Pos < JamRec.WorkPos; Pos++, p++ )
      {
      if( *p == '\r' )
        {
        puts( "<0d>" );
        Col = 0;
        continue;
        }

      else if( *p < ' ' )
        {
        printf( "<%02x>", *p );
        Col += 4;
        }

      else
        {
        if( Col > 70 && *p == ' ' )
          {
          putchar( '\n' );
          Col = 0;
          }
        else
          {
          putchar( *p );
          Col++;
          }
        }
      }
    }

  puts( "" );

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  DisplayHdrInfo
 *
 *    Display the header information record, found first in the message
 *    header file. Also calculate the total number of messages, and the
 *    number of deleted messages
 *
 *    Parameters:   None
 *
 *    Returns:      0                   - Failure
 *                  1                   - Success
 *
 * ---------------------------------------------------------------------- */

int DisplayHdrInfo( void )
{
  INT32   TotalMsgs;

  printf( "Header info\n\n" );


/*
**  Read the header information record
*/

  if( !JAMmbUpdateHeaderInfo( &JamRec, 0 ))
    {
    printf( "Error reading header info, code: %d, errno: %d\n", JamRec.APImsg, JamRec.Errno );
    return( 0 );
    }


/*
**  Calculate the total (active + deleted) number of messages
*/

  TotalMsgs = filelength( JamRec.IdxHandle ) / sizeof( JAMIDXREC );


/*
**  Display the information
*/

  printf( "Signature    : <%02x><%02x><%02x><%02x>\n",
                                    JamRec.HdrInfo.Signature [0],
                                    JamRec.HdrInfo.Signature [1],
                                    JamRec.HdrInfo.Signature [2],
                                    JamRec.HdrInfo.Signature [3] );
  printf( "DateCreated  : %s",      JamRec.HdrInfo.DateCreated ? DispTime( &JamRec.HdrInfo.DateCreated ) : "<null>\n" );
  printf( "ModCounter   : %lu\n",   JamRec.HdrInfo.ModCounter );
  printf( "ActiveMsgs   : %lu\n",   JamRec.HdrInfo.ActiveMsgs );
  printf( "PasswordCRC  : %08lx\n", JamRec.HdrInfo.PasswordCRC );
  printf( "BaseMsgNum   : %lu\n",   JamRec.HdrInfo.BaseMsgNum );
  puts( "" );
  printf( "TotalMsgs    : %lu\n",   TotalMsgs );
  printf( "DeletedMsgs  : %lu\n",   TotalMsgs - JamRec.HdrInfo.ActiveMsgs );

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  AttrToStr
 *
 *    Convert the Attribute field to a ASCIIZ string containing all the
 *    attribute names
 *
 *    Parameters:   UINT32 Attr   - The attributes to convert
 *
 *    Returns:      CHAR8 *       - Ptr to buffer with attribute names
 *
 * ---------------------------------------------------------------------- */

CHAR8 * AttrToStr( UINT32 Attr )
{
  static CHAR8    Buf [256];
  CHAR8         * p = Buf;
  int             i;

  Buf [0] = '\0';

  for( i = 0; i < 32; i++ )
    {
    if( Attr & 0x00000001L )
      {
      if( p != Buf )
        {
        *p++ = ',';
        *p++ = ' ';
        }
      strcpy( p, AttrName [i] );
      p = strchr( p, '\0' );
      }

    Attr >>= 1;
    }

  return( Buf );
}


/* ---------------------------------------------------------------------- *
 *
 *  GetSubFldName
 *
 *    Get the name of the specified subfield
 *
 *    Parameters:   JAMSUBFIELD * pSubFld   - Ptr to subfield
 *
 *    Returns:      CHAR8 *                 - Name of subfield
 *
 * ---------------------------------------------------------------------- */

CHAR8 * GetSubFldName( JAMSUBFIELD * pSubFld )
{
  int     i;

  for( i = 0; i < sizeof( SubFieldInfo ) / sizeof( SubFieldInfo [0] ); i++ )
    {
    if( pSubFld->LoID == SubFieldInfo [i].LoID &&
        pSubFld->HiID == SubFieldInfo [i].HiID )
      return( SubFieldInfo [i].pName );
    }

  return( "Undefined" );
}


/* ---------------------------------------------------------------------- *
 *
 *  GetSubFldStr
 *
 *    Convert the subfield contents to a string that can be displayed
 *
 *    Parameters:   JAMSUBFIELD * pSubFld   - Ptr to subfield
 *
 *    Returns:      CHAR8 *                 - Ptr to buffer containing
 *                                            subfield contents
 *
 * ---------------------------------------------------------------------- */

CHAR8 * GetSubFldStr( JAMSUBFIELD * pSubFld )
{
  static CHAR8    Buffer [256],
                * pBuf;
  UINT32          i;
  int             BufPos;

  for(  pBuf = Buffer, i = BufPos = 0;
        i < pSubFld->DatLen && BufPos + 5 < sizeof( Buffer );
        pBuf++, i++, BufPos++ )
    {
    if( pSubFld->Buffer [( int ) i] < ' ' )
      {
      sprintf( pBuf, "<%02x>", pSubFld->Buffer [( int ) i] );
      BufPos += 3;
      pBuf += 3;
      }
    else
      *pBuf = pSubFld->Buffer [( int ) i];
    }

  *pBuf = '\0';

  return( Buffer );
}


/* ---------------------------------------------------------------------- *
 *
 *  DispTime
 *
 *    Converts the specified time to a ASCII string
 *
 *    Parameters:   UINT32 * pt             - # of seconds since 1970
 *
 *    Returns:      CHAR8 *                 - Ptr to buffer containing
 *                                            ASCII string
 *
 * ---------------------------------------------------------------------- */

CHAR8 * DispTime( UINT32 * pt )
{
  static CHAR8    MonthNames [12][4] = {"Jan","Feb","Mar","Apr","May","Jun",
                                        "Jul","Aug","Sep","Oct","Nov","Dec"};
  static CHAR8    Buffer [64];
  struct JAMtm  * ptm;

  ptm = JAMsysLocalTime( pt );
  sprintf( Buffer, "%s %d, %4d  %2d:%02d:%02d\n",
          MonthNames [ptm->tm_mon], ptm->tm_mday, ptm->tm_year + 1900,
          ptm->tm_hour, ptm->tm_min, ptm->tm_sec );

  return( Buffer );
}


#if !defined(__MSDOS__)

/* ---------------------------------------------------------------------- *
 *
 *  filelength
 *
 * ---------------------------------------------------------------------- */

UINT32 filelength( int fh )
{
  UINT32    Pos,
            Length;

  Pos = JAMsysSeek( NULL, fh, SEEK_CUR, 0L );
  Length = JAMsysSeek( NULL, fh, SEEK_END, 0L );
  JAMsysSeek( NULL, fh, SEEK_SET, Pos );

  return( Length );
}

#endif


/* end of file "jamutil.c" */
