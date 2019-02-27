/*
**  JAM(mbp) - The Joaquim-Andrew-Mats Message Base Proposal
**
**  HMB to JAM converter
**
**  Written by Mats Wallin
**
**  ----------------------------------------------------------------------
**
**  hmb2jam.c (JAMmb)
**
**  Program that moves messages from a HMB message base to a
**  JAM message base
**
**  Requires that the HMB messagebase is located in the current directory
**
**  Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and
**  Mats Wallin. ALL RIGHTS RESERVED.
**
**  93-06-28    MW
**  Initial coding.
*/

#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <share.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "jammb.h"

#include "hmbsub.h"
#include "jamsub.h"


/*
**  The largest supported size of a HMB message text, in records (256 bytes)
*/

#define MAXHMBRECS    200


/*
**  Global variables
*/

JAMAPIREC       JamRec;
HMBREC          HmbRec;

AKA             DestAddr,
                OrigAddr;

UCHAR8          MsgTxt [MAXHMBRECS * sizeof( HMBTXT )];

UINT32          SubFldPos;


/*
**  Prototypes
*/

int ScanMsgs( CHAR8 * pJamFileName, int Board );
int MoveThisMsg( HMBREC * pHmb, JAMAPIREC * pJam );
int HandleFtscKludge( JAMAPIREC * pJam, CHAR8 * pStr );
CHAR8 * GetKludgeContents( CHAR8 * pStr );


/* ---------------------------------------------------------------------- *
 *
 *  main
 *
 * ---------------------------------------------------------------------- */

int main( int argc, CHAR8 * argv [] )
{
  int   Board;
  CHAR8 JamFileName [128];

  puts( "HMB to JAM converter\n"
        "Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and\n"
        "               Mats Wallin. ALL RIGHTS RESERVED.\n"
        "Written by Mats Wallin\n" );

  if( argc != 3 )
    {
    puts( "Format: HMB2JAM <board> <JAMfile>\n"
          "where: <board>   is the HMB board number\n"
          "       <JAMfile> is the path and base filename to the JAM messagebase\n" );
    return( 1 );
    }

  if(( Board = atoi( argv [1] )) < 1 || Board > 200 )
    {
    printf( "Invalid HMB board number specified: %s\n", argv [1] );
    return( 1 );
    }

  strcpy( JamFileName, argv [2] );


/*
**  Open the HMB messagebase
*/

  if( !InitHMBREC( &HmbRec ))
    return( 0 );

  if( !HmbOpen( &HmbRec, JAMO_RDONLY, JAMSH_DENYNO ))
    return( 0 );


  if( !ScanMsgs( JamFileName, Board ))
    return( 1 );


/*
**  Close the HMB messagebase
*/

  HmbClose( &HmbRec );
  return( 0 );
}


/* ---------------------------------------------------------------------- *
 *
 *  ScanMsgs
 *
 * ---------------------------------------------------------------------- */

int ScanMsgs( CHAR8 * pJamFileName, int Board )
{
  int   done;

  if( !JAMsysInitApiRec( &JamRec, pJamFileName, WORKBUFSIZE ))
    {
    puts( "Not enough memory" );
    return( 0 );
    }


/*
**  Open the JAM messagebase
*/

  if( !JAMmbOpen( &JamRec ))
    {
    if( JamRec.Errno != ENOENT )
      {
      perror( "Unable to open the JAM messagebase" );
      JAMsysDeinitApiRec( &JamRec );
      return( 0 );
      }

    if( !JAMmbCreate( &JamRec ))
      {
      perror( "Unable to create the JAM messagebase" );
      JAMsysDeinitApiRec( &JamRec );
      return( 0 );
      }
    }


/*
**  Read all messages in the HMB messagebase, and search for those in the
**  correct board
*/

  done = HmbReadFirstHdr( &HmbRec );
  while( !done )
    {
    if( HmbRec.Hdr.Board == ( UCHAR8 ) Board )
      {
      printf( "Message %u\n", HmbRec.Hdr.MsgNo );
      MoveThisMsg( &HmbRec, &JamRec );
      }

    done = HmbReadNextHdr( &HmbRec );
    }


/*
**  Close the JAM messagebase
*/

  if( !JAMmbClose( &JamRec ))
    {
    perror( "Unable to close the JAM messagebase" );
    return( 0 );
    }

  JAMsysDeinitApiRec( &JamRec );

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  MoveThisMsg
 *
 * ---------------------------------------------------------------------- */

int MoveThisMsg( HMBREC * pHmb, JAMAPIREC * pJam )
{
  CHAR8         String [256],
                Str2 [128];
  UCHAR8      * pSrc,
              * pDst;
  int           NumRecs,
                i;
  unsigned int  MsgTxtSize;


/*
**  If the message is deleted, skip it
*/

  if( pHmb->Hdr.MsgAttr & HMB_DELETED )
    return( 0 );


/*
**  First process the header
*/

  JamMsgInit( pJam );
  SubFldPos = 0;

  if( pHmb->Hdr.MsgAttr & HMB_NETMAIL )
    {
    OrigAddr.Zone  = pHmb->Hdr.OrigZone;
    OrigAddr.Net   = pHmb->Hdr.OrigNet;
    OrigAddr.Node  = pHmb->Hdr.OrigNode;
    OrigAddr.Point = 0;

    DestAddr.Zone  = pHmb->Hdr.DestZone;
    DestAddr.Net   = pHmb->Hdr.DestNet;
    DestAddr.Node  = pHmb->Hdr.DestNode;
    DestAddr.Point = 0;
    }

  Pascal2C( pHmb->Hdr.WhoFrom, String );
  JamMsgAddSFldStr( pJam, JAMSFLD_SENDERNAME, String, &SubFldPos );

  Pascal2C( pHmb->Hdr.WhoTo, String );
  JamMsgAddSFldStr( pJam, JAMSFLD_RECVRNAME, String, &SubFldPos );

  Pascal2C( pHmb->Hdr.Subject, String );
  if( pHmb->Hdr.NetAttr & HMB_FILE )
    JamMsgAddSFldStr( pJam, JAMSFLD_ENCLFILE, String, &SubFldPos );
  else
    JamMsgAddSFldStr( pJam, JAMSFLD_SUBJECT, String, &SubFldPos );

  pJam->Hdr.TimesRead = pHmb->Hdr.TimesRead;
  pJam->Hdr.Cost      = pHmb->Hdr.Cost;

  Pascal2C( pHmb->Hdr.PostDate, String );
  Pascal2C( pHmb->Hdr.PostTime, Str2 );
  pJam->Hdr.DateWritten = HmbDateTimeToUnix( String, Str2 );
  pJam->Hdr.DateProcessed = JAMsysTime( NULL );

  if( pHmb->Hdr.MsgAttr & HMB_NETMAIL )
    pJam->Hdr.Attribute |= MSG_TYPENET;
  else
    pJam->Hdr.Attribute |= MSG_TYPEECHO;

  if( pHmb->Hdr.MsgAttr & HMB_PVT     ) pJam->Hdr.Attribute |= MSG_PRIVATE;
  if( pHmb->Hdr.MsgAttr & HMB_RCVD    ) pJam->Hdr.Attribute |= MSG_READ;
  if( pHmb->Hdr.MsgAttr & HMB_LOCAL   ) pJam->Hdr.Attribute |= MSG_LOCAL;

  if( pHmb->Hdr.NetAttr & HMB_KILL    ) pJam->Hdr.Attribute |= MSG_KILLSENT;
  if( pHmb->Hdr.NetAttr & HMB_SENT    ) pJam->Hdr.Attribute |= MSG_SENT;
  if( pHmb->Hdr.NetAttr & HMB_FILE    ) pJam->Hdr.Attribute |= MSG_FILEATTACH;
  if( pHmb->Hdr.NetAttr & HMB_CRASH   ) pJam->Hdr.Attribute |= MSG_CRASH;
  if( pHmb->Hdr.NetAttr & HMB_RR_REQ  ) pJam->Hdr.Attribute |= MSG_RECEIPTREQ;


/*
**  And now it's time for the actual messagetext
*/

  JAMsysSeek( NULL, pHmb->TxtHandle, JAMSEEK_SET, ( INT32 ) pHmb->Hdr.StartRec * ( INT32 ) sizeof( HMBTXT ) );

  NumRecs = min( pHmb->Hdr.NumRecs, MAXHMBRECS );
  JAMsysRead( NULL, pHmb->TxtHandle, MsgTxt, NumRecs * sizeof( HMBTXT ));


/*
**  Copy the messagetext in the buffer, so we can get rid of the Length byte
**  in each record
*/

  pSrc        = MsgTxt + 1;
  pDst        = MsgTxt;
  MsgTxtSize  = 0;

  for( i = 0; i < NumRecs; i++, pSrc += sizeof( HMBTXT ))
    {
    int   RecSize = pSrc [-1];

    memmove( pDst, pSrc, RecSize );

    pDst        += RecSize;
    MsgTxtSize  += RecSize;
    }

  *pDst = '\0';
  MsgTxtSize++;


/*
**  Locate all FTSC kludge lines
**  They should be stored as SubFields in the header
*/

  pDst = MsgTxt;
  do
    {
    while( *pDst == '\r' || *pDst == '\n' || *pDst == 0x8D )
      pDst++;

    while( *pDst == '\x01' )
      {
      if(( pSrc = ( UCHAR8 * ) strchr(( CHAR8 * ) pDst, '\r' )) != NULL )
        *pSrc = '\0';

      strcpy( String, ( CHAR8 * ) pDst + 1 );
      HandleFtscKludge( pJam, String );

      if( pSrc )
        {
        pSrc++;
        while( *pSrc == '\n' || *pDst == 0x8D )
          pSrc++;

        MsgTxtSize -= ( unsigned int ) ( pSrc - pDst );
        memmove( pDst, pSrc, ( int ) ( MsgTxt + MsgTxtSize - pDst ));
        }
      }
    }
  while(( pDst = ( UCHAR8 * ) strchr(( CHAR8 * ) pDst, '\r' )) != NULL );


/*
**  If this is a netmail, add the origin and destination address
*/

  if( pHmb->Hdr.MsgAttr & HMB_NETMAIL )
    {
    sprintf( String, OrigAddr.Point != 0 ? "%u:%u/%u.%u" : "%u:%u/%u",
                OrigAddr.Zone, OrigAddr.Net, OrigAddr.Node, OrigAddr.Point );
    JamMsgAddSFldStr( pJam, JAMSFLD_OADDRESS, String, &SubFldPos );

    sprintf( String, DestAddr.Point != 0 ? "%u:%u/%u.%u" : "%u:%u/%u",
                DestAddr.Zone, DestAddr.Net, DestAddr.Node, DestAddr.Point );
    JamMsgAddSFldStr( pJam, JAMSFLD_DADDRESS, String, &SubFldPos );
    }


/*
**  Now, start to write this message
*/

  pJam->Hdr.SubfieldLen = SubFldPos;
  pJam->Hdr.TxtLen = MsgTxtSize;

  JamMsgWrite( pJam, ( CHAR8 * ) MsgTxt );
  JamMsgDeinit( pJam );

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  HandleFtscKludge
 *
 * ---------------------------------------------------------------------- */

int HandleFtscKludge( JAMAPIREC * pJam, CHAR8 * pStr )
{
  CHAR8      Kludge [32];

  strncpy( Kludge, pStr, sizeof( Kludge ));
  Kludge [sizeof( Kludge ) - 1] = '\0';
  strtok( Kludge, ": " );

  if( !strcmp( Kludge, "MSGID" ))
    JamMsgAddSFldStr( pJam, JAMSFLD_MSGID, GetKludgeContents( pStr ), &SubFldPos );

  else if( !strcmp( Kludge, "REPLY" ))
    JamMsgAddSFldStr( pJam, JAMSFLD_REPLYID, GetKludgeContents( pStr ), &SubFldPos );

  else if( !strcmp( Kludge, "PID" ))
    JamMsgAddSFldStr( pJam, JAMSFLD_PID, GetKludgeContents( pStr ), &SubFldPos );

  else if( !strcmp( Kludge, "SEEN-BY" ))
    JamMsgAddSFldStr( pJam, JAMSFLD_SEENBY2D, GetKludgeContents( pStr ), &SubFldPos );

  else if( !strcmp( Kludge, "PATH" ))
    JamMsgAddSFldStr( pJam, JAMSFLD_PATH2D, GetKludgeContents( pStr ), &SubFldPos );

  else if( !strcmp( Kludge, "FLAGS" ))
    JamMsgAddSFldStr( pJam, JAMSFLD_FLAGS, GetKludgeContents( pStr ), &SubFldPos );

  else if( !strcmp( Kludge, "TZUTC" ))
    JamMsgAddSFldStr( pJam, JAMSFLD_TZUTCINFO, GetKludgeContents( pStr ), &SubFldPos );

  else if( !strcmp( Kludge, "INTL" ))
    {
    /* Do nothing, INTL kludge shouldn't exist in HMB messagebase */
    }

  else if( !strcmp( Kludge, "FMPT" ))
    OrigAddr.Point = atoi( GetKludgeContents( pStr ));

  else if( !strcmp( Kludge, "TOPT" ))
    DestAddr.Point = atoi( GetKludgeContents( pStr ));

  else
    JamMsgAddSFldStr( pJam, JAMSFLD_FTSKLUDGE, pStr, &SubFldPos );

  return( 0 );
}


/* ---------------------------------------------------------------------- *
 *
 *  GetKludgeContents
 *
 * ---------------------------------------------------------------------- */

CHAR8 * GetKludgeContents( CHAR8 * pStr )
{
  CHAR8   * p;

  for( p = pStr; *p != ':' && *p != ' '; p++ )
    ;
  for( ; *p == ':' || *p == ' '; p++ )
    ;

  return( p );
}


/* end of file "hmb2jam.c" */
