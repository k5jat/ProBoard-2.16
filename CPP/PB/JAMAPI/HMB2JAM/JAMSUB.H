/*
**  JAM(mbp) - The Joaquim-Andrew-Mats Message Base Proposal
**
**  HMB to JAM converter
**
**  Written by Mats Wallin
**
**  ----------------------------------------------------------------------
**
**  jamsub.h (JAMmb)
**
**  Definitions and prototypes for JAM subroutines
**
**  Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and
**  Mats Wallin. ALL RIGHTS RESERVED.
**
**  93-06-28    MW
**  Initial coding.
*/

/*
**  Constants
*/

#define WORKBUFSIZE     0x4000


/*
**  Prototypes
*/

int JamMsgInit( JAMAPIREC * pJam );
int JamMsgDeinit( JAMAPIREC * pJam );
int JamMsgAddSFldStr( JAMAPIREC * pJam, UINT16 SubFld, CHAR8 * Str, UINT32 * pSubFldPos );
int JamMsgWrite( JAMAPIREC * pJam, CHAR8 * pMsgTxt );


/* end of file "jamsub.h" */
