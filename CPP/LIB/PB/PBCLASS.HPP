
/* ------------------------------------------------------------
 * Filename ............... PbClass.Hpp
 *
 * General Purpose ........ ProBoard classes
 * ------------------------------------------------------------
 * First date ............. 14 sep 1993
 *
 * First in version ....... 1.40
 *
 * Written by ............. Alain Schellinck
 * ------------------------------------------------------------
 * Revisions:
 * ----------
 *
 *   Date   | By |                  Purpose                   |
 * ---------+----+--------------------------------------------+
 *          |    |                                            |
 *          |    |                                            |
 *          |    |                                            |
 */

/* +---------------------------------------------------------------------+
   |                      Multi Record classes                           |
   +---------------------------------------------------------------------+ */

/*--] Classes [----------------------------------] PbBinLog [--------------*/

#ifdef Uses_PbBinLog

   struct binlog_data
   {
      Date     date;
      Time     timeIn;
      Time     timeOut;
      ChrT     name[36];
      ChrT     city[26];
      ChrT     country[26];
      LngT     baud;
      UShrtT   node;
      LngT     kbDown;
      LngT     kbUp;
      UShrtT   yells;
      UShrtT   level;
      DwdT     uflags;
      ChrT     extra[81];
   };

   _ClassType(PbBinLog)
   class PbBinLog : public binlog_data, public MultiRecIO
   {
   private:

   public:
            PbBinLog       (  ChrP pbPath, ChrP fn = fnPbBinLog         )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(binlog_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbBinLogP orig                            )
            {
               memcpy(this, orig, recLen());
            }
   };

#endif

/*--] Classes [----------------------------------] PbEvent [---------------*/

#ifdef Uses_PbEvent

   _StructType(event_data)
   struct event_data
   {
      UShrtT   cmdFlag;
      UShrtT   errorLevel;
      ChrT     dosCmd[80];
      Time     eventTime;
      UShrtT   duration;
      UShrtT   eventFlag;
      BytT     nodeNum;
      FlgT     enabled;
   };

   _ClassType(PbEvent)
   class PbEvent : public event_data, public MultiRecIO
   {
   private:

   public:
            PbEvent        (  ChrP pbPath, ChrP fn = fnPbEvent          )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(event_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbEventP orig                             )
            {
               memcpy(this, orig, recLen());
            }
   };

#endif

/*--] Classes [----------------------------------] PbFile [----------------*/

#ifdef Uses_PbFile

#  define   PB_CDROM_AREA           1
#  define   PB_HDISK_AREA           0

   _StructType(file_data)
   struct file_data
   {
      ChrT     name[80];                     // Name of file area
      ChrT     listpbPath[80];                 // Path for file-list
      ChrT     filepbPath[80];                 // Path for files
      UShrtT   level;                        // Level needed for access
      PbFlags  flags;                        // Flags needed for access
      ChrT     type;                         // 1 = CDROM File listing type
      ShrtT    maxfiles;                     // Max files per day in this area downloadable
      ShrtT    maxkb;                        // Max Kbytes per day in this area
      FlgT     notops;                       // Set to 1 -> should not be listed in TOPFILES.A??
      FlgT     free;                         // Set to 1 -> free area
      ChrT     extra[13];                    // Reserved
   };

   _ClassType(PbFile)
   class PbFile : public file_data, public MultiRecIO
   {
   private:

   public:
            PbFile         (  ChrP pbPath, ChrP fn = fnPbFile           )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(file_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbFileP orig                              )
            {
               memcpy(this, orig, recLen());
            }

      FlgT  inUse          (                                            )
            {
               return(name[0] == 0 ? FALSE : TRUE);
            }
   };

#endif

/*--] Classes [----------------------------------] PbFileIdx [-------------*/

#ifdef Uses_PbFileIdx

   _StructType(fileidx_data)
   struct fileidx_data
   {
      ChrT     filename[13];                 // Name of the file, with extension
      UShrtT   area;                         // File area number where file is located
   };

   _ClassType(PbFileIdx)
   class PbFileIdx : public fileidx_data, public MultiRecIO
   {
   private:

   public:
            PbFileIdx      (  ChrP pbPath, ChrP fn = fnPbFileIdx        )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(fileidx_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbFileIdxP orig                           )
            {
               memcpy(this, orig, recLen());
            }

      FlgT  inUse          (                                            )
            {
               return(filename[0] == 0 ? FALSE : TRUE);
            }
   };

#endif


/*--] Classes [----------------------------------] PbLimits [--------------*/

#ifdef Uses_PbLimits

   _StructType(limits_data)
   struct limits_data
   {
      UShrtT   level;                        // Level
      ShrtT    timelimit;                    // # minutes per day
      ShrtT    daily_klimit;                 // Kbytes per day allowed
      ShrtT    pre_download;                 // # minutes online before download
      ChrT     id[6];                        // Usergroup ID
      UShrtT   free;                         // Free upload in Kb.
      BytT     factor;                       // Percentage upload required
      UShrtT   max_download;                 // Max download for this level
      ShrtT    fallto;                       // Fall to level x when max. reached
      ShrtT    msgfactor;                    // # Kbytes granted per message written
      ChrT     extra[5];
   };

   _ClassType(PbLimits)
   class PbLimits : public limits_data, public MultiRecIO
   {
   private:

   public:
            PbLimits       (  ChrP pbPath, ChrP fn = fnPbLimits         )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(limits_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbLimitsP orig                            )
            {
               memcpy(this, orig, recLen());
            }
   };

#endif

/*--] Classes [----------------------------------] PbMessage [-------------*/

#ifdef Uses_PbMessage

/* ---------------------------------------------------------------
#  define   MSGTYPE_BOTH            0     // Private/Public
#  define   MSGTYPE_PVT             1     // Private Only
#  define   MSGTYPE_PUBLIC          2     // Public Only
#  define   MSGTYPE_TOALL           3     // To All

#  define   MSGKIND_LOCAL           0     // Local
#  define   MSGKIND_NET             1     // NetMail
#  define   MSGKIND_ECHO            2     // EchoMail
#  define   MSGKIND_PVTECHO         3     // Pvt EchoMail

#  define   MSGBASE_HUDSON          0     // hudson message base
#  define   MSGBASE_SQUISH          1     // squish message base
#  define   MSGBASE_SDM             2     // *.msg message base
#  define   MSGBASE_JAM             3     // jam message base
------------------------------------------------------------------ */

   _StructType(message_data)
   struct message_data
   {
      ShrtT    areanum;                      // # of message area (1-10000)
      ChrT     name[31];                     // Name of message areas
      BytT     msgtype;                      // Type of messages
      BytT     msgkind;                      // Kind of message area
      BytT     msgbasetype;                  // 0 = hudson , 1 = squish , 2 = *.MSG , 3 = JAM
      ChrT     pbPath[80];                     // Path/Directory for Squish               //.MSG
      BytT     flags;                        // Alias allowed/forced/prohibited
      UShrtT   readlevel;                    // Minimum level needed to read msgs
      PbFlags  readflags;                    // flags needed to read msgs
      UShrtT   writelevel;                   // Minimum level needed to write msgs
      PbFlags  writeflags;                   // flags needed to write msgs
      UShrtT   sysoplevel;                   // Minimum level needed to change msgs
      PbFlags  sysopflags;                   // flags needed to change msgs
      ChrT     origin[62];                   // Origin line
      ShrtT    aka;                          // AKA
      ShrtT    rcv_kill_days;                // Kill received after xx days
      ShrtT    msg_kill_days;                // Kill after xx days
      ShrtT    max_msgs;                     // Max # msgs
      ChrT     sysop[36];                    // Area Sysop
      ShrtT    replyboard;                   // Reply area # (0=here)
      ChrT     echotag[21];                  // Echomail Tag Name
      ChrT     qwktag[13];                   // QWK Area Name
      ChrT     extra[23];
   };

   _ClassType(PbMessage)
   class PbMessage : public message_data, public MultiRecIO
   {
   private:

   public:
            PbMessage      (  ChrP pbPath, ChrP fname = fnPbMessage     )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(message_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbMessageP orig                           )
            {
               memcpy(this, orig, recLen());
            }

      FlgT  inUse          (                                            )
            {
               return(name[0] == 0 ? FALSE : TRUE);
            }
   };

#endif

/*--] Classes [----------------------------------] PbOnLine [--------------*/

#ifdef Uses_PbOnLine

   _StructType(online_data)
   struct online_data
   {
      ChrT     name[36];                     // Name of user online
      ChrT     city[16];                     // City of user online
      ShrtT    baud;                         // Baud rate of user
      ShrtT    status;                       // 0 -> online , <0 -> not online or unavailable
      UShrtT   userRecNo;                    // not officialy!!!
      ChrT     extra[6];
   };

   _ClassType(PbOnLine)
   class PbOnLine : public online_data, public MultiRecIO
   {
   private:

   public:
            PbOnLine       (  ChrP pbPath, ChrP fname = fnPbOnLine      )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(online_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbOnlineP orig                            )
            {
               memcpy(this, orig, recLen());
            }

      FlgT  inUse          (                                            )
            {
               return(name[0] == 0 ? FALSE : TRUE);
            }
   };

#endif

/*--] Classes [----------------------------------] PbProtocol [------------*/

#ifdef Uses_PbProtocol

#  define   PROT_BATCH              1       // Batch protocol
#  define   PROT_ENABLED            2       // Enabled
#  define   PROT_BOTH               4       // Full-duplex protocol
#  define   PROT_BIM                8       // Bimodem-type ctl-file

   _StructType(protocol_data)
   struct protocol_data
   {
      ChrT     name[50];                     // Name of protocol
      ChrT     key;                          // Hotkey for Protocol
      ChrT     flags;                        // Protocol behavior
      ChrT     logfile[60];                  // Logfile from protocol
      ChrT     ctlfile[60];                  // Control file (redirection file)
      ChrT     dlcom[128];                   // Download command line
      ChrT     ulcom[128];                   // Upload command line
      ChrT     dlstring[80];                 // String to place in download control file
      ChrT     ulstring[80];                 // String to place in upload control file
      ChrT     dlkeyword[40];                // Download keyword to search for in logfile
      ChrT     ulkeyword[40];                // Upload keyword to search for in logfile
      ShrtT    wordnr;                       // File name word nr after keyword (1-..)
   };

   _ClassType(PbProtocol)
   class PbProtocol : public protocol_data, public MultiRecIO
   {
   private:

   public:
            PbProtocol     (  ChrP pbPath, ChrP fn = fnPbProtocol       )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(protocol_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbProtocolP orig                          )
            {
               memcpy(this, orig, recLen());
            }

      FlgT  inUse          (                                            )
            {
               return(name[0] == 0 ? FALSE : TRUE);
            }
   };

#endif

/*--] Classes [----------------------------------] PbPvtFile [-------------*/

#ifdef Uses_PbPvtFile

   _StructType(pvtfile_data)
   struct pvtfile_data
   {
      Date date;
      ChrT fname[13];
      ChrT username[36];
      ChrT from[36];
   };

   _ClassType(PbPvtFile)
   class PbPvtFile : public PvtFile_data, public MultiRecIO
   {
   private:

   public:
            PbPvtFile     (  ChrP pbPath, ChrP fn = fnPbPvtFile         )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(pvtfile_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbPvtFileP orig                           )
            {
               memcpy(this, orig, recLen());
            }

      FlgT  inUse          (                                            )
            {
               return(name[0] == 0 ? FALSE : TRUE);
            }
   };

#endif

/*--] Classes [----------------------------------] PbUsersPb [-------------*/

#ifdef Uses_PbUsersPb

#  define   RA_UFLAG_NOIBM          1
#  define   RA_UFLAG_NOTOPS         2
#  define   RA_UFLAG_AVTPLUS        4
#  define   RA_UFLAG_ATTEN          8
#  define   RA_UFLAG_NORIP          16
#  define   RA_UFLAG_MULTILOGIN     32
#  define   RA_UFLAG_FREECHAT       64
#  define   RA_UFLAG_LOCALONLY      128

   _StructType(userspb_data)
   struct userspb_data
   {
      ChrT     name[36];                     // user's name
      ChrT     country[26];                  // his country
      ChrT     state[26];                    // and state
      ChrT     faxPhone[16];                 // his fax phone number
      ChrT     password[16];                 // password
      ChrT     language[9];                  // default language
      Date     lastPwdChange;                // date lasy pwd changed
      Date     lastNewFilesCheck;            // date last scanned for new files
      ShrtT    loglevel;                     // Loglevel (0-3)
      ShrtT    tbTimeBalance;                // Timebank time balance
      ShrtT    tbKbBalance;                  // TimeBank Kb balance
      ShrtT    tbTimeWithdrawn;              // TimeBank time withdrawn today
      ShrtT    tbKbWithdrawn;                // TimeBank Kb withdrawn today
      UShrtT   tbTimeDeposited;              // Time deposited today   (Time Bank)
      UShrtT   tbKbDeposited;                // Kbytes deposited today (Time Bank)
      UShrtT   tbTimeLoaned;                 // Time lended (total)    (Time Bank)
      UShrtT   tbKbLoaned;                   // Kbytes lended (total)  (Time Bank)
      Date     tbTimePayback;                // Payback date for time  (Time Bank)
      Date     tbKbPayback;                  // Payback date for Kbyte (Time Bank)
      Date     tbLastUsed;                   // Date of last access to TimeBank
      UShrtT   explevel;                     // Expiration level
      PbFlags  expFlagsOn;                   // flags to switch on when expired
      PbFlags  expFlagsOff;                  // flags to switch off when expired
      DwdT     uflags;                       // Extra user flags  (see UFLAG_XXXX)
      PbBoards mailcheckboards;              // Mail areas where to check for mail
      DwdT     totalTimeUsed;                // Total time used ever (minutes)
      UShrtT   qwkMaxMsgPerArea;             // max. message / area for QWK's
      UShrtT   qwkMaxMsg;                    // total number of max messages
      BytT     qwkArchiver;                  // selected QWK archiver
      BytT     ripFont;                      // selected RIP font
      BytT     checkMail;                    // check for mail ?
      BytT     checkNewFiles;                // check for new files ?
      BytT     extra[398];                   // reserved f/t future
   };

   _ClassType(PbUsersPb)
   class PbUsersPb : public userspb_data, public MultiRecIO
   {
   private:

   public:
            PbUsersPb      (  ChrP pbPath, ChrP fn = fnPbUsersPb        )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(userspb_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbUsersPbP orig                           )
            {
               memcpy(this, orig, recLen());
            }

      FlgT  inUse          (                                            )
            {
               return(name[0] == 0 ? FALSE : TRUE);
            }
   };

#endif

/*--] Classes [----------------------------------] PbUsersIdx [------------*/

#ifdef Uses_PbUsersIdx

   _StructType(usersidx_data)
   struct usersidx_data
   {
      DwdT     nameCrc;                      // 32-bit CRC code for the name
      DwdT     handleCrc;                    // 32-bit CRC code for the handle
   };

   _ClassType(PbUsersIdx)
   class PbUsersIdx : public usersidx_data, public MultiRecIO
   {
   private:

   public:
            PbUsersIdx     (  ChrP pbPath, ChrP fn = fnPbUsersIdx       )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(usersidx_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbUsersIdxP orig                          )
            {
               memcpy(this, orig, recLen());
            }
   };

#endif

/*--] Classes [----------------------------------] PbUsersBbs [------------*/

#ifdef Uses_PbUsersBbs

   _StructType(usersbbs_data)
   struct usersbbs_data
   {
      ChrT     Name[36];
      ChrT     Location[26];
      ChrT     Company[51];
      ChrT     Adress1[51];
      ChrT     Adress2[51];
      ChrT     Adress3[51];
      ChrT     Handle[36];
      ChrT     Comment[81];
      LngT     PasswordCRC;
      ChrT     DataPhone[16];
      ChrT     VoicePhone[16];
      ChrT     LastTime[6];
      ChrT     LastDate[9];
      UShrtT   uFlags;
      LngT     Flags;
      LngT     Credit;
      LngT     Pending;
      WrdT     MsgsPosted;
      WrdT     Security;
      LngT     LastRead;
      LngT     NoCalls;
      LngT     Uploads;
      LngT     Downloads;
      LngT     UploadsK;
      LngT     DownloadsK;
      LngT     TodayK;
      ShrtT    Elapsed;
      UShrtT   ScreenLength;
      BytT     LastPwdChange;
      UShrtT   Group;
      UShrtT   combined[200];
      ChrT     FirstDate[9];
      ChrT     BirthDate[9];
      ChrT     SubDate[9];
      BytT     ScreenWidth;
      BytT     Language;
      BytT     DateFormat;
      ChrT     ForwardTo[36];
      UShrtT   MsgArea;
      UShrtT   FileArea;
      ChrT     DefaultProtocol;
      UShrtT   FileGroup;
      BytT     LastDOBCheck;
      BytT     Sex;
      LngT     XURecord;
      UShrtT   MsgGroup;
      ChrT     extra[48];
   };

   _ClassType(PbUsersBbs)
   class PbUsersBbs : public usersbbs_data, public MultiRecIO
   {
   private:

   public:
            PbUsersBbs     (  ChrP pbPath, ChrP fn = fnPbUsersBbs    )
            {
               setFname(pbPath, fn);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(usersbbs_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbUsersBbsP orig                          )
            {
               memcpy(this, orig, recLen());
            }

      FlgT  inUse          (                                            )
            {
               return(Name[0] == 0 ? FALSE : TRUE);
            }
   };

#endif

/* +---------------------------------------------------------------------+
   |                      Single Record classes                          |
   +---------------------------------------------------------------------+ */

/*--] Classes [----------------------------------] PbConfig [--------------*/

#ifdef Uses_PbConfig

   _StructType(config_data)
   struct config_data
   {
      ChrT     shellmsg[81];                 // Message to show when shelling
      ChrT     sysopname[36];                // Name of sysop
      ChrT     txtpbPath[61];                  // Path for textfiles
      ChrT     mnupbPath[61];                  // Path for menu-files
      ChrT     msgpbPath[61];                  // Path for message base
      ChrT     uploadpbPath[61];               // UploadpbPath
      ChrT     editorname[61];               // Name of external editor
      UShrtT   newuserlevel;                 // Level for new user
      ShrtT    newuserloglevel;              // Loglevel for new user
      PbFlags  newuserflags;                 // Newuser flags
      ShrtT    max_passinput;                // Maximum attempts for password entry
      ShrtT    min_passlength;               // Minimum password length
      ShrtT    inactivity_time;              // Inactivity time-out limit
      ShrtT    max_sysop_pages;              // Maximum times sysop can be paged
      ShrtT    pagebell_length;              // Length of page-bell (secs)
      ShrtT    mailcheck;                    // Check for mail at logon?
      ShrtT    europe;                       // European date format?
      ShrtT    numnodes;                     // # nodes
      FlgT     allowansi;                    // Allow ANSI?
      FlgT     allowavatar;                  // Allow AVATAR?
      ShrtT    discrete;                     // Hide sysop activity?
      ShrtT    askphone;                     // Ask for phone number?
      ShrtT    allowoneword;                 // Allow one-word names
      UShrtT   crashlevel;                   // Level needed for crashmail
      PbFlags  crashflags;                   // Flags needed for crashmail
      UShrtT   attachlevel;                  // Level needed for file attach
      PbFlags  attachflags;                  // Flags needed for file attach
      ShrtT    allowmsgupload;               // Allow message uploads
      ShrtT    allowstacking;                // Allow command stacking
      Time     page_start;                   // Paging hours start
      Time     page_end;                     // Paging hours end
      ShrtT    handshaking;                  // I/O Handshaking
      ShrtT    allowalias;                   // Allow alias for login
      ShrtT    loglocal;                     // Log local calls
      ShrtT    doswap;                       // Allow swapping
      ChrT     originline[61];               // Origin line
      ChrT     nodelistdir[61];              // Nodelist directory
      ChrT     sysopkeys[10][60];            // Sysop hotkeys
      Time     dl_start;                     // Download hours start
      Time     dl_end;                       // Download hours end
      ShrtT    uploadspace;                  // Space needed for uploads
      ChrT     pvtuploadpbPath[61];            // Directory for files uploads
      ChrT     quotestring[6];               // String used for quoting
      FlgT     fastmode;                     // Use fast mode
      FlgT     extra_bool_1;
      FlgT     killsent;                     // Kill netmail after sent
      FlgT     multiline;                    // Use message base sharing?
      FlgT     egamode;                      // Use 43/50 line mode
      FlgT     showuserinfo;                 // Show user info while in EGA mode?
      ChrT     pexpbPath[61];                  // Directory for PEX files
      FlgT     allowquicklogin;              // Allow quick sysop login?
      FlgT     suspendmsgtime;               // Suspend time when writing msgs
      ShrtT    securityboard;                // MsgBoard for security messages
      FlgT     pwdmessages;                  // Write security-messages?
      FlgT     extra_bool_2;
      ChrT     bbsname[36];                  // Name of the BBS
      ChrT     pwdchar;                      // Password character
      ShrtT    tb_maxtimedeposit;            // Max time deposit per day (TimeBank)
      ShrtT    tb_maxkbdeposit;              // Max Kbytes deposit per day (TimeBank)
      ShrtT    tb_maxtimewithdrawal;         // Max time withdrawal per day (TimeBank)
      ShrtT    tb_maxkbwithdrawal;           // Max Kbytes withdrawal per day (TimeBank)
      ShrtT    usage_days;                   // Days to keep usage graphs
      ChrT     systempwd[16];                // System password
      FlgT     usesystempwd;                 // Use system password?
      FlgT     askbirthdate;                 // Ask Birth Date?
      ShrtT    binlogdays;                   // # days to log in BINLOG.PB
      FlgT     binloglocal;                  // Log local calls to BINLOG.PB yes/no
      ShrtT    pageArea;                     // Area number for page messages
      FlgT     indexfiles;                   // Use file indexing
      FlgT     checkdupes;                   // Check for dupes
      FlgT     killdupes;                    // Kill duplocate files
      FlgT     ignore_ext;                   // Ignore file extensions for dupe checking
      ChrT     RIPpbPath[61];                  // Path for RIP scripts
      ChrT     iconspbPath[61];
      ChrT     location[36];
      ChrT     phone[26];
      ChrT     QWKid[9];
      UShrtT   IObuffersize;
      PbTime   pagingHours;
      ChrT     defaultLanguage[9];
      FlgT     addUploaderName;
      PbTime   downloadHours;
      FlgT     askdataphone;
      FlgT     askfaxphone;
      FlgT     askaddress;
      FlgT     asksex;
      FlgT     askdateformat;
      ChrT     extra[278];
   };

   _ClassType(PbConfig)
   class PbConfig : public config_data , public MultiRecIO
   {
   private:

   public:
            PbConfig       (  ChrP pbPath, ChrP fn = fnPbConfig         )
            {
               setFname(pbPath, fn);
            }

     FlgT   read           (                                            )
            {
               FlgT ret_val = FALSE;

               if(open(fmode_read, defaultBufferSize) == TRUE)
                  if(MultiRecIO::read(0) == TRUE)
                  {
                     close();
                     ret_val = TRUE;
                  }

               return(ret_val);
            }

     FlgT   write          (                                            )
            {
               FlgT ret_val = FALSE;

               if(open(fmode_create, defaultBufferSize) == TRUE)
                  if(MultiRecIO::write(0) == TRUE)
                  {
                     close();
                     ret_val = TRUE;
                  }

               return(ret_val);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(config_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbConfigP orig                            )
            {
               memcpy(this, orig, recLen());
            }
   };

#endif

/*--] Classes [----------------------------------] PbModem [---------------*/

#ifdef Uses_PbModem

   _StructType(modem_data)
   struct modem_data
   {
      UShrtT   baud;
      ShrtT    port;
      ShrtT    quiet;
      ShrtT    blanktime;
      ChrT     initstring[70];
      ChrT     busystring[70];
      ChrT     initok[40];
      ChrT     busyok[40];
      ChrT     connect300[40];
      ChrT     connect1200[40];
      ChrT     connect2400[40];
      ChrT     connect4800[40];
      ChrT     connect9600[40];
      ChrT     connect19200[40];
      ChrT     connect38400[40];
      ChrT     ring[40];
      ChrT     answer[40];
      ChrT     connect7200[40];
      ChrT     connect12000[40];
      bool     forceanswer;
      byte     extra1[5];
      ShrtT    delay;
      ChrT     connect14400[40];
      ChrT     initstring2[70];
      ChrT     connect16800[40];
      ChrT     connect28800[40];
      ChrT     ext_connect1[40];
      ChrT     ext_connect2[40];
      ShrtT    ext_errlevel1;
      ShrtT    ext_errlevel2;
      ChrT     extra[36];
   };

   _ClassType(PbModem)
   class PbModem : public modem_data , public MultiRecIO
   {
   private:

   public:
            PbModem        (  ChrP pbPath, ChrP fn = fnPbModem          )
            {
               setFname(pbPath, fn);
            }


     FlgT   read           (                                            )
            {
               FlgT ret_val = FALSE;

               if(open(fmode_read, defaultBufferSize) == TRUE)
                  if(MultiRecIO::read(0) == TRUE)
                  {
                     close();
                     ret_val = TRUE;
                  }

               return(ret_val);
            }

     FlgT   write          (                                            )
            {
               FlgT ret_val = FALSE;

               if(open(fmode_create, defaultBufferSize) == TRUE)
                  if(MultiRecIO::write(0) == TRUE)
                  {
                     close();
                     ret_val = TRUE;
                  }

               return(ret_val);
            }

     FlgT   readRec        (                                            )
            {
               return(File::read(this, recLen()) == recLen());
            }

     FlgT   writeRec       (                                            )
            {
               return(File::write(this, recLen()) == recLen());
            }

     ShrtT  recLen         (                                            )
            {
               return(SIZ(modem_data));
            }

     void   clear          (                                            )
            {
               memset(this, 0, recLen());
            }

     void   copy           (  PbModemP orig                             )
            {
               memcpy(this, orig, recLen());
            }
   };

#endif

/*---------------------------] END OF THE CODE [---------------------------*/
