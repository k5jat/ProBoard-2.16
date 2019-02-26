/*
**  JAM(mbp) - The Joaquim-Andrew-Mats Message Base Proposal
**
**  HMB to JAM converter
**
**  Written by Mats Wallin
**
**  ----------------------------------------------------------------------
**
**  hmb.h (JAMmb)
**
**  Definitions of the HMB messagebase files
**
**  Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and
**  Mats Wallin. ALL RIGHTS RESERVED.
**
**  93-06-28    MW
**  Initial coding.
*/

#ifndef __HMB_H__
#define __HMB_H__

/*
**  PASCAL_STR is used when defining a pascal string. The result will be   
**  a char-declared variable containing the length, and a char array       
**  containing the string itself. Ex:                                      
**  PASCAL_STR( Str, 5 );    = UCHAR8 lStr; CHAR8 Str [5];                   
*/

#define PASCAL_STR(Str,Len) UCHAR8 l##Str; CHAR8 Str [Len]


/*
** MSGIDX.BBS
*/

typedef struct
  {
  UINT16  MsgNo;
  UCHAR8  Board;
  } HMBIDX;


/*
** MSGHDR.BBS
*/

typedef struct 
  {
  UINT16      MsgNo,
              ReplyTo,
              SeeAlso,
              TimesRead,
              StartRec;
  INT16       NumRecs;
  UINT16      DestNet,
              DestNode,
              OrigNet,
              OrigNode;
  UCHAR8      DestZone,
              OrigZone;
  UINT16      Cost;
  UCHAR8      MsgAttr,
              NetAttr,
              Board;
  PASCAL_STR( PostTime,    5 );
  PASCAL_STR( PostDate,    8 );
  PASCAL_STR( WhoTo,      35 );
  PASCAL_STR( WhoFrom,    35 );
  PASCAL_STR( Subject,    72 );
  } HMBHDR;


/*
** Message attributes (HMB.MsgAttr) 
*/

#define HMB_DELETED     0x01            /* Message is deleted               */
#define HMB_UNMOV_NETM  0x02            /* Unsent netmail message           */
#define HMB_NETMAIL     0x04            /* Message is a netmail message     */
#define HMB_PVT         0x08            /* Private                          */
#define HMB_RCVD        0x10            /* Message read by addressee        */
#define HMB_UNMOV_ECHO  0x20            /* Unexported echomail message      */
#define HMB_LOCAL       0x40            /* Message is written locally       */

/*
**  Net attributes (HMB.NetAttr) 
*/

#define HMB_KILL        0x01            /* Delete message when sent         */
#define HMB_SENT        0x02            /* Message is sent                  */
#define HMB_FILE        0x04            /* File attach                      */
#define HMB_CRASH       0x08            /* High priority                    */
#define HMB_RR_REQ      0x10            /* Return Receipt Requested         */
#define HMB_AUDIT_REQ   0x20            /* Audit request                    */
#define HMB_IS_RR       0x40            /* Is a return receipt              */


/*
**  MSGTXT.BBS
*/

typedef struct
  {
  PASCAL_STR(     Txt,       255 );
  } HMBTXT;


/*
**  MSGINFO.BBS
*/

typedef struct
  {
  UCHAR8  LowMsgNo,
          HighMsgNo,
          TotalActive,
          ActiveMsgs [200];
  } HMBINFO;


/*
**  MSGTOIDX.BBS
*/

typedef struct 
  {
  PASCAL_STR( WhoTo,      35 );
  } HMBTOIDX;

#endif

/* end of file "hmb.h" */
