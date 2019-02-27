/*
**  JAM(mbp) - The Joaquim-Andrew-Mats Message Base Proposal
**
**  HMB to JAM converter
**
**  Written by Mats Wallin
**
**  ----------------------------------------------------------------------
**
**  jamsub.c (JAMmb)
**
**  Common JAM routines used for the HMB2JAM program
**
**  Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and
**  Mats Wallin. ALL RIGHTS RESERVED.
**
**  93-06-28    MW
**  Initial coding.
*/

#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "jammb.h"

#include "jamsub.h"


/* ---------------------------------------------------------------------- *
 *
 *  JamMsgInit
 *
 *    Initiates the structures used for a message
 *
 * ---------------------------------------------------------------------- */

int JamMsgInit( JAMAPIREC * pJam )
{
  memset( &pJam->Idx, '\0', sizeof( JAMIDXREC ));
  pJam->Idx.UserCRC = ( UINT32 ) -1L;

  memset( &pJam->Hdr, '\0', sizeof( JAMHDR ));

  pJam->Hdr.MsgIdCRC    = ( UINT32 ) -1L;
  pJam->Hdr.ReplyCRC    = ( UINT32 ) -1L;
  pJam->Hdr.PasswordCRC = ( UINT32 ) -1L;

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  JamMsgDeinit
 *
 * ---------------------------------------------------------------------- */

int JamMsgDeinit( JAMAPIREC * pJam )
{
  memset( &pJam->Hdr, '\0', sizeof( JAMHDR ));

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  JamMsgAddSFldStr
 *
 * ---------------------------------------------------------------------- */

int JamMsgAddSFldStr( JAMAPIREC * pJam, UINT16 SubFld, CHAR8 * Str, UINT32 * pSubFldPos )
{
  int           Len         = strlen( Str );

  if( !JAMmbAddField( pJam, SubFld, 1, strlen( Str ), pSubFldPos, Str ))
    puts( "WARNING: Work buffer for subfields to small" );

  switch( SubFld )
    {
    case JAMSFLD_RECVRNAME :
      strlwr( Str );
      pJam->Idx.UserCRC = JAMsysCrc32( Str, Len, ( UINT32 ) -1L );
      break;

    case JAMSFLD_MSGID :
      strlwr( Str );
      pJam->Hdr.MsgIdCRC = JAMsysCrc32( Str, Len, ( UINT32 ) -1L );
      break;

    case JAMSFLD_REPLYID :
      strlwr( Str );
      pJam->Hdr.ReplyCRC = JAMsysCrc32( Str, Len, ( UINT32 ) -1L );
      break;
    }

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  JamMsgWrite
 *
 * ---------------------------------------------------------------------- */

int JamMsgWrite( JAMAPIREC * pJam, CHAR8 * pMsgTxt )
{
  int     LockTryCnt = 0;
  UINT32  MsgNo;

/*
**  Lock the messagebase
*/

  while( !JAMmbLockMsgBase( pJam, 1 ))
    {
    if( ++LockTryCnt >= 15 )
      {
      puts( "Unable to get lock on messagebase" );
      exit( 3 );
      }

#if defined(_MSC_VER) || defined(_QC)
#else
    sleep( 1 );      /* Wait one second */
#endif
    }


/*
**  Get the message number for the new message
*/

  MsgNo = ( filelength( pJam->IdxHandle ) / sizeof( JAMIDXREC )) + pJam->HdrInfo.BaseMsgNum;
  pJam->Hdr.MsgNum = MsgNo;


/*
**  Get the offset in the header file for the next message header
*/

  pJam->Idx.HdrOffset = filelength( pJam->HdrHandle );


/*
**  And get the offset in the text file for the next text
*/

  pJam->Hdr.TxtOffset = filelength( pJam->TxtHandle );


/*
**  Store the index record
*/

  if( !JAMmbStoreMsgIdx( pJam, MsgNo ))
    {
    printf( "Error writing JAMIDXREC: %d\n", pJam->APImsg );
    exit( 4 );
    }


/*
**  And the header record
*/

  if( !JAMmbStoreMsgHdr( pJam, MsgNo ))
    {
    printf( "Error writing JAMHDR: %d\n", pJam->APImsg );
    exit( 4 );
    }


/*
**  Write all the subfields
*/

  if( JAMsysWrite( NULL, pJam->HdrHandle, pJam->WorkBuf, ( int ) pJam->Hdr.SubfieldLen ) != ( int ) pJam->Hdr.SubfieldLen )
    {
    printf( "Error writing SubFields\n" );
    exit( 4 );
    }


/*
**  Write the message text
*/

  if( !JAMmbStoreMsgTxtBuf( pJam, pMsgTxt, pJam->Hdr.TxtLen, 1 ))
    {
    printf( "Error writing message text: %d\n", pJam->APImsg );
    exit( 4 );
    }


/*
**  Unlock the messagebase
*/

  pJam->HdrInfo.ActiveMsgs++;
  JAMmbUnLockMsgBase( pJam, 1 );

  return( 1 );
}


/* end of file "jamsub.c" */
