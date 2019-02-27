/*
**  JAM(mbp) - The Joaquim-Andrew-Mats Message Base Proposal
**
**  HMB to JAM converter
**
**  Written by Mats Wallin
**
**  ----------------------------------------------------------------------
**
**  hmbsub.c (JAMmb)
**
**  Common HMB routines used for the HMB2JAM program
**
**  Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and
**  Mats Wallin. ALL RIGHTS RESERVED.
**
**  93-06-28    MW
**  Initial coding.
*/

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __TURBOC__
#include <dos.h>
#else
#include <time.h>
#endif

#include "jammb.h"
#include "hmbsub.h"


/* ---------------------------------------------------------------------- *
 *
 *  HmbClose
 *
 * ---------------------------------------------------------------------- */

int HmbClose( HMBREC * pHmb )
{
  if( pHmb->InfoHandle  != -1 ) JAMsysClose( NULL, pHmb->InfoHandle );
  if( pHmb->IdxHandle   != -1 ) JAMsysClose( NULL, pHmb->IdxHandle );
  if( pHmb->ToidxHandle != -1 ) JAMsysClose( NULL, pHmb->ToidxHandle );
  if( pHmb->HdrHandle   != -1 ) JAMsysClose( NULL, pHmb->HdrHandle );
  if( pHmb->TxtHandle   != -1 ) JAMsysClose( NULL, pHmb->TxtHandle );

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  HmbOpen
 *
 * ---------------------------------------------------------------------- */

int HmbOpen( HMBREC * pHmb, int OpenMode, int ShareMode )
{
  if(( pHmb->InfoHandle  = JAMsysSopen( NULL, "MSGINFO.BBS",  OpenMode, ShareMode )) == -1 ||
     ( pHmb->IdxHandle   = JAMsysSopen( NULL, "MSGIDX.BBS",   OpenMode, ShareMode )) == -1 ||
     ( pHmb->ToidxHandle = JAMsysSopen( NULL, "MSGTOIDX.BBS", OpenMode, ShareMode )) == -1 ||
     ( pHmb->HdrHandle   = JAMsysSopen( NULL, "MSGHDR.BBS",   OpenMode, ShareMode )) == -1 ||
     ( pHmb->TxtHandle   = JAMsysSopen( NULL, "MSGTXT.BBS",   OpenMode, ShareMode )) == -1 )
    {
    perror( "Unable to open one or more of the HMB files" );

    HmbClose( pHmb );
    return( 0 );
    }

  JAMsysRead( NULL, pHmb->InfoHandle, &pHmb->Info, sizeof( HMBINFO ));

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  HmbReadFirstHdr
 *
 * ---------------------------------------------------------------------- */

int HmbReadFirstHdr( HMBREC * pHmb )
{
  JAMsysSeek( NULL, pHmb->HdrHandle, JAMSEEK_SET, 0L );

  return( HmbReadNextHdr( pHmb ));
}


/* ---------------------------------------------------------------------- *
 *
 *  HmbReadNextHdr
 *
 * ---------------------------------------------------------------------- */

int HmbReadNextHdr( HMBREC * pHmb )
{
  INT32   Len;

  if(( Len = JAMsysRead( NULL, pHmb->HdrHandle, &pHmb->Hdr, sizeof( HMBHDR ))) != sizeof( HMBHDR ))
    {
    if( Len != 0L )
      puts( "Error reading the HMB header file" );
    return( 1 );
    }

  return( 0 );
}


/* ---------------------------------------------------------------------- *
 *
 *  InitHMBREC
 *
 * ---------------------------------------------------------------------- */

int InitHMBREC( HMBREC * pHmbRec )
{
  memset( pHmbRec, '\0', sizeof( HMBREC ));

  pHmbRec->InfoHandle = -1,
    pHmbRec->IdxHandle = -1,
      pHmbRec->ToidxHandle = -1,
        pHmbRec->HdrHandle = -1,
          pHmbRec->TxtHandle = -1;

  return( 1 );
}


/* ---------------------------------------------------------------------- *
 *
 *  Pascal2C
 *
 * ---------------------------------------------------------------------- */

CHAR8 * Pascal2C( CHAR8 * PascalStr, CHAR8 * CStr )
{
  memcpy( CStr, PascalStr, PascalStr [-1] );
  CStr [PascalStr [-1]] = '\0';

  return( CStr );
}


/* ---------------------------------------------------------------------- *
 *
 *  HmbDateTimeToUnix
 *
 * ---------------------------------------------------------------------- */

UINT32 HmbDateTimeToUnix( CHAR8 * pDate, CHAR8 * pTime )
{
  struct JAMtm    TM;

  memset( &TM, '\0', sizeof( TM ));

  TM.tm_year = atoi( pDate + 6 );
  TM.tm_mon  = atoi( pDate     ) - 1;
  TM.tm_mday = atoi( pDate + 3 );
  TM.tm_hour = atoi( pTime     );
  TM.tm_min  = atoi( pTime + 3 );

  return( JAMsysMkTime( &TM ));
}


/* end of file "hmbsub.c" */
