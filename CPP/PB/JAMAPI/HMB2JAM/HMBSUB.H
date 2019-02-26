/*
**  JAM(mbp) - The Joaquim-Andrew-Mats Message Base Proposal
**
**  HMB to JAM converter
**
**  Written by Mats Wallin
**
**  ----------------------------------------------------------------------
**
**  hmbsub.h (JAMmb)
**
**  Definitions and prototypes for HMB subroutines
**
**  Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and
**  Mats Wallin. ALL RIGHTS RESERVED.
**
**  93-06-28    MW
**  Initial coding.
*/

#include "hmb.h"

/*
**  Structures
*/

typedef struct
  {
  unsigned int  Zone,
                Net,
                Node,
                Point;
  } AKA;


typedef struct
  {
  int     InfoHandle,
          IdxHandle,
          ToidxHandle,
          HdrHandle,
          TxtHandle;
  HMBINFO Info;
  HMBIDX  Idx;
  HMBHDR  Hdr;
  HMBTXT  Txt;
  } HMBREC;


/*
**  Prototypes
*/

int HmbClose( HMBREC * pHmb );
int HmbOpen( HMBREC * pHmb, int OpenMode, int ShareMode );
int HmbReadFirstHdr( HMBREC * pHmb );
int HmbReadNextHdr( HMBREC * pHmb );
int InitHMBREC( HMBREC * pHmbRec );
CHAR8 * Pascal2C( CHAR8 * PascalStr, CHAR8 * CStr );
UINT32 HmbDateTimeToUnix( CHAR8 * pDate, CHAR8 * pTime );


/* end of file "hmbsub.h" */
