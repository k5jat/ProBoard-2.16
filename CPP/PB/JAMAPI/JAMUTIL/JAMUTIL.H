/*
**  JAM(mbp) - The Joaquim-Andrew-Mats Message Base Proposal
**
**  JAM Utility
**
**  Written by Mats Wallin & Mats Birch
**
**  ----------------------------------------------------------------------
**
**  jamutil.h (JAMmb)
**
**  Definitions for JAMUTIL.C
**
**  Copyright 1993 Joaquim Homrighausen, Andrew Milner, Mats Birch, and
**  Mats Wallin. ALL RIGHTS RESERVED.
**
**  93-06-28    MW
**  Initial coding
*/

/*
**  Buffer size allocated for the JAM API functions.     
**  This buffer is used when reading message text and    
**  message header subfields                             
*/

#define WORKBUFSIZE       0x8000


/*
**  Structure where the names of all known SUBFIELDS are stored 
*/

typedef struct
  {
  unsigned    LoID,               /* Subfield id, low word */
              HiID;               /* "-, reserved */
  CHAR8     * pName;              /* Name of subfield */
  } SUBFLDINFO;


/*
**  Define the JAMAPIREC variable, used in almost all calls to the 
**  JAM API, and contains information about the currently opened   
**  messagebase, and the last message read                         
*/

JAMAPIREC JamRec;


/*
**  Define name of all attributes. The first name is the attribute 
**  with the value 0x00000001, and the last name is the attribute  
**  with the value 0x80000000                                      
*/

CHAR8     AttrName [32][16] =
              {
              "Local",
              "Transit",
              "Pvt",
              "Rcvd",
              "Sent",
              "Kill",
              "Arch",
              "Hold",
              "Crash",
              "Imm",
              "Dir",
              "Gate",
              "Req",
              "File",
              "Trunc/Sent",
              "Kill/Sent",
              "Rcpt",
              "Conf",
              "Orphan",
              "Encrypt",
              "Comp",
              "Esc",
              "Fpu",
              "TypeLocal",
              "TypeEcho",
              "TypeNet",
              "n/a1",
              "n/a2",
              "NoDisp",
              "Lock",
              "Del"
              };


/*
**  Define names for all subfields 
*/

SUBFLDINFO    SubFieldInfo [] =
  {
    { JAMSFLD_OADDRESS,     0, "OriginAddress"    },
    { JAMSFLD_DADDRESS,     0, "DestAddress"      },
    { JAMSFLD_SENDERNAME,   0, "SenderName"       },
    { JAMSFLD_RECVRNAME,    0, "ReceiverName"     },
    { JAMSFLD_MSGID,        0, "MsgId"            },
    { JAMSFLD_REPLYID,      0, "ReplyId"          },
    { JAMSFLD_SUBJECT,      0, "Subject"          },
    { JAMSFLD_PID,          0, "PID"              },
    { JAMSFLD_TRACE,        0, "Trace"            },
    { JAMSFLD_ENCLFILE,     0, "EnclFile"         },
    { JAMSFLD_ENCLFWALIAS,  0, "EnclFileWAlias"   },
    { JAMSFLD_ENCLFREQ,     0, "EnclFreq"         },
    { JAMSFLD_ENCLFILEWC,   0, "EnclFileWildCard" },
    { JAMSFLD_ENCLINDFILE,  0, "EnclIndirectFile" },
    { JAMSFLD_EMBINDAT,     0, "Embindat"         },
    { JAMSFLD_FTSKLUDGE,    0, "FTS kludge"       },
    { JAMSFLD_SEENBY2D,     0, "Seen-By 2d"       },
    { JAMSFLD_PATH2D,       0, "Path 2d"          },
    { JAMSFLD_FLAGS,        0, "Flags"            },
    { JAMSFLD_TZUTCINFO,    0, "TZUTC info"       }
  };


/* end of file "jamutil.h" */
