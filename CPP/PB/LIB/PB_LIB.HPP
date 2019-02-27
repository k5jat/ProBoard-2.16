#undef enable
#undef disable

typedef byte AskType;


//-------------------  
// Basic ASCII values
//-------------------  

#ifndef   CR
  #define CR 13
#endif

#ifndef   BS
  #define BS 8
#endif


//-------------------------------------------------------------------  
// Nice readability constants for the source code, introduced in v2.2
//-------------------------------------------------------------------  

#ifndef   and
  #define and   &&
#endif

#ifndef   or
  #define or    ||
#endif

#ifndef   not
  #define not   !
#endif

#ifndef   loop
  #define loop  for ( ; ; )
#endif


const int MAX_FILE_AREAS = 10000;
const int MAX_MSG_AREAS  = 10000;

class AccessFlags
  {
   dword v;
 public:
   AccessFlags() { clearall(); }
   void clearall() { v=0L; }
   bool getflag(int flagnr);
   void setflag(int flagnr);
   void clearflag(int flagnr);
   void flagstostr(char *str);

        operator dword (              )  {  return(v);  }
   void operator =     (  dword flags )  {  v = flags;  }
  };

class CombinedType
  {
   byte bits[125];
 public:
   CombinedType();
   bool connected(int brdnr);
   void connect(int brdnr);
   void disconnect(int brdnr);
   void toggle(int brdnr);
  };

struct TimeFrame
  {
    byte periods[7][6];                 // 7x48 periods of 1/2 hour = one week

    TimeFrame();

    void clear();
    void setAll();

    bool enabled()                   const;
    bool enabled(Time&)              const;
    bool enabled(Date&,Time&)        const;
    bool enabled(int dow,Time&)      const;
    bool enabled(int dow,int period) const;

    void enable(int dow,Time&);
    void enable(int dow,int period);
    void disable(int dow,Time&);
    void disable(int dow,int period);
  };

struct AccessRecord
  {
//    AccessRecord();

    word         minLevel;              // 0 =           don't care
    word         maxLevel;              // 0 =           don't care
    AccessFlags  flags;                 // All off =     don't care
    AccessFlags  flagsNot;              // All off =     don't care
    byte         minAge;                // 0 =           don't care
    byte         maxAge;                // 0 =           don't care
    byte         sex;                   // 0 =           don't care
    word         minTimeLeft;           // 0 =           don't care
    word         minTimeOnline;         // 0 =           don't care
    TimeFrame    timeFrame;             // All on =      don't care
    byte         terminals;             // 0xFF =        don't care
    dword        minSpeed;              // 0 =           don't care
    dword        maxSpeed;              // 0 =           don't care

    byte         nodes[32];             // All 0 or all 0xFF = all nodes

    byte         extra[18];             // Extra space
  };

#ifndef NO_RA1
struct RA1_UsersXiBBS
  {
   char Handle[36];
   char Comment[81];
   char FirstDate[9];
   byte CombinedInfo[25];
   char BirthDate[9];
   char SubDate[9];
   byte ScreenWidth;
   byte MsgArea;
   byte FileArea;
   byte Language;
   byte DateFormat;
   char ForwardFor[36];
   byte extra[43];
  };

struct RA1_UsersBBS
  {
   char Name[36];
   char Location[26];
   char Password[16];
   char DataPhone[13];
   char VoicePhone[13];
   char LastTime[6];
   char LastDate[9];
   byte Attribute;
   long Flags;
   int  Credit;
   int  Pending;
   unsigned
        MsgsPosted,
        LastRead,
        Security,
        NoCalls,
        Uploads,
        Downloads,
        UploadsK,
        DownloadsK,
        TodayK,
        Elapsed,
        ScreenLength;
   byte LastPwdChange;
   byte Attribute2;

   byte Group;
   unsigned XIrecord;
   char extra[3];
  };

#define RA1_UFLAG_DELETED  1
#define RA1_UFLAG_CLRSCR   2
#define RA1_UFLAG_MORE     4
#define RA1_UFLAG_ANSI     8
#define RA1_UFLAG_NOKILL  16
#define RA1_UFLAG_IGNORE  32
#define RA1_UFLAG_FSE     64
#define RA1_UFLAG_QUIET  128

#define RA1_UFLAG2_HOTKEYS 1
#define RA1_UFLAG2_AVATAR  2
#define RA1_UFLAG2_FSMSG   4
#define RA1_UFLAG2_HIDDEN  8

#define RA1_UFLAG3_NOIBM    1
#define RA1_UFLAG3_NOTOPS   2
#define RA1_UFLAG3_AVTPLUS  4
#define RA1_UFLAG3_ATTEN    8
#define RA1_UFLAG3_NORIP   16

struct RA1_UsersPbBBS
  {
   char            username[36];
   int             loglevel;
   char            country[26];
   int             tb_timebalance;
   int             tb_kbytebalance;
   int             tb_timewithdrawn;
   int             tb_kbytewithdrawn;
   word            expirlevel;
   word            uflags;
   Date            tb_lastused;
   long            totaltimeused;
   CombinedType mailcheckboards;
   CombinedType boards;
   word            tb_timedeposited;
   word            tb_kbytedeposited;
   word            tb_timelended;
   word            tb_kbytelended;
   Date            tb_timepayback;
   Date            tb_kbytepayback;
   byte            hibyte_filearea;
   byte            hibyte_msgarea;
   byte            rip_font;
   byte            packer;
   byte            protocol;
   dword           passwordCrc;
   char            language[9];
   byte            extra[368];
  };

struct RA1_UsersIdxBBS
  {
   dword NameCRC;
   dword HandleCRC;
  };

#endif

struct RA2_UsersIdxBBS
  {
      dword NameCRC;
      dword HandleCRC;
  };

struct RA2_UsersBBS
    {
      char  Name[36];               //û    Name           : MSGTOIDXrecord;
      char  Location[26];           //û    Location       : String[25];
      char  Organisation[51];       //û    Organisation,
      char  Address1[51];           //û    Address1,
      char  Address2[51];           //û    Address2,
      char  Address3[51];           //û    Address3       : String[50];
      char  Handle[36];             //û    Handle         : String[35];
      char  Comment[81];            //û    Comment        : String[80];
      dword PasswordCRC;            //û    PasswordCRC    : LongInt;
      char  DataPhone[16];          //û    DataPhone,
      char  VoicePhone[16];         //û    VoicePhone     : String[15];
      char  LastTime[6];            //û    LastTime       : Time;
      char  LastDate[9];            //û    LastDate       : Date;
      byte  Attribute;              //û    Attribute,
      byte  Attribute2;             //û    Attribute2     : Byte;
      dword Flags;                  //û    Flags          : FlagType;
      long  Credit;                 //û    Credit,
      long  Pending;                //û    Pending        : LongInt;
      word  MsgsPosted;             //û    MsgsPosted,
      word  Security;               //û    Security       : Word;
      long  LastRead;               //û    LastRead,
      long  NoCalls;                //û    NoCalls,
      long  Uploads;                //û    Uploads,
      long  Downloads;              //û    Downloads,
      long  UploadsK;               //û    UploadsK,
      long  DownloadsK;             //û    DownloadsK,
      long  TodayK;                 //û    TodayK         : LongInt;
      sword Elapsed;                //û    Elapsed        : Integer;
      word  ScreenLength;           //û    ScreenLength   : Word;
      byte  LastPwdChange;          //û*   LastPwdChange  : Byte;
      word  Group;                  //û*   Group          : Word;
      byte  CombinedInfo[400];      //     CombinedInfo   : COMBINEDrecord;
      char  FirstDate[9];           //û    FirstDate,
      char  BirthDate[9];           //û    BirthDate,
      char  SubDate[9];             //û    SubDate        : Date;
      byte  ScreenWidth;            //û    ScreenWidth,
      byte  Language;               //û*   Language,
      byte  DateFormat;             //û    DateFormat     : Byte;
      char  ForwardTo[36];          //û    ForwardTo      : String[35];
      word  MsgArea;                //û    MsgArea,
      word  FileArea;               //û    FileArea       : Word;
      byte  DefaultProtocol;        //û    DefaultProtocol: Char;
      word  FileGroup;              //û*   FileGroup      : Word;
      byte  LastDOBCheck;           //û*   LastDOBCheck   : Byte;
      byte  Sex;                    //     Sex            : Byte;
      long  XIrecord;               //     XIrecord       : LongInt;
      word  MsgGroup;               //û*   MsgGroup       : Word;
      byte  FreeSpace[48];          //     FreeSpace      : Array[1..48] of Byte;
    };

struct RA2_UsersXiBBS
    {
      byte extra[200];
    };

struct RA2_UsersPbBBS
  {
   char         name[36];
   char         country[26];
   char         state[26];
   char         faxPhone[16];
   char         passWord[16];
   char         language[9];
   Date         lastPwdChange;
   Date         lastNewFilesCheck;
   int          logLevel;
   sword        tbTimeBalance;
   sword        tbKbBalance;
   sword        tbTimeWithdrawn;
   sword        tbKbWithdrawn;
   word         tbTimeDeposited;
   word         tbKbDeposited;
   word         tbTimeLoaned;
   word         tbKbLoaned;
   Date         tbTimePayback;
   Date         tbKbPayback;
   Date         tbLastUsed;

   word         expLevel;
   AccessFlags  expFlagsOn;
   AccessFlags  expFlagsOff;
   dword        uFlags;
   CombinedType mailCheckBoards;
   dword        totalTimeUsed;
   word         qwkMaxMsgsPerArea;
   word         qwkMaxMsgs;
   byte         qwkArchiver;
   byte         ripFont;
   AskType      checkMail;
   AskType      checkNewFiles;

   byte         extra[398];
  };


#define RA_UFLAG_DELETED  1
#define RA_UFLAG_CLRSCR   2
#define RA_UFLAG_MORE     4
#define RA_UFLAG_ANSI     8
#define RA_UFLAG_NOKILL  16
#define RA_UFLAG_IGNORE  32
#define RA_UFLAG_FSE     64
#define RA_UFLAG_QUIET  128

#define RA_UFLAG2_HOTKEYS     1
#define RA_UFLAG2_AVATAR      2
#define RA_UFLAG2_FSMSG       4
#define RA_UFLAG2_HIDDEN      8
#define RA_UFLAG2_PAGEPRI    16
#define RA_UFLAG2_NOECHOSCAN 32
#define RA_UFLAG2_GUEST      64
#define RA_UFLAG2_POSTBILL  128

#define RA_UFLAG3_NOIBM        1
#define RA_UFLAG3_NOTOPS       2
#define RA_UFLAG3_AVTPLUS      4
#define RA_UFLAG3_ATTEN        8
#define RA_UFLAG3_NORIP       16
#define RA_UFLAG3_MULTILOGIN  32
#define RA_UFLAG3_FREECHAT    64
#define RA_UFLAG3_LOCALONLY  128


const dword UFLAG_DELETED   = 0x00000001L; //  0 // User deleted
const dword UFLAG_ANSI      = 0x00000002L; //  1 // ANSI mode ON
const dword UFLAG_PAUSE     = 0x00000004L; //  2 // Pause mode ON
const dword UFLAG_CLEAR     = 0x00000008L; //  3 // Screenclear mode ON
const dword UFLAG_HOTKEYS   = 0x00000010L; //  4 // Hotkeys?
const dword UFLAG_NOIBM     = 0x00000020L; //  5 // User has no IBM-graphics
const dword UFLAG_FSED      = 0x00000040L; //  6 // Fullscreen editor used
const dword UFLAG_NOKILL    = 0x00000080L; //  7 // Do not kill user
const dword UFLAG_IGNORE    = 0x00000100L; //  8 // Ignore DL-hours
const dword UFLAG_ATTEN     = 0x00000200L; //  9 // Attention flag
const dword UFLAG_NOTOPS    = 0x00000400L; // 10 // Don't appear in tops
const dword UFLAG_HIDDEN    = 0x00000800L; // 11 // Hide from lists
const dword UFLAG_QUIET     = 0x00001000L; // 12 // Quiet??
const dword UFLAG_AVATAR    = 0x00002000L; // 13 // AVT/0 codes
const dword UFLAG_AVTPLUS   = 0x00004000L; // 14 // AVT/0+ codes
const dword UFLAG_GUEST     = 0x00008000L; // 15 // Guest account
const dword UFLAG_PAGEPRI   = 0x00010000L; // 16 // Page priority
const dword UFLAG_LOCALONLY = 0x00020000L; // 17 // Local login only
const dword UFLAG_MULTILOGIN= 0x00040000L; // 18 // Allow multiple login
const dword UFLAG_FREECHAT  = 0x00080000L; // 19 // Freeze timer in chat
const dword UFLAG_NORIP     = 0x00100000L; // 20 // Disable RIP

const byte  SEX_UNKNOWN     = 0;
const byte  SEX_MALE        = 1;
const byte  SEX_FEMALE      = 2;

/*
#define UFLAG_FUTURE      0x00200000L  // 21
#define UFLAG_FUTURE      0x00400000L  // 22
#define UFLAG_FUTURE      0x00800000L  // 23
#define UFLAG_FUTURE      0x01000000L  // 24
#define UFLAG_FUTURE      0x02000000L  // 25
#define UFLAG_FUTURE      0x04000000L  // 26
#define UFLAG_FUTURE      0x08000000L  // 27
#define UFLAG_FUTURE      0x10000000L  // 28
#define UFLAG_FUTURE      0x20000000L  // 29
#define UFLAG_FUTURE      0x40000000L  // 30
#define UFLAG_FUTURE      0x80000000L  // 31
*/

struct _User
   {
   private:
      static      String     sysDir;
      static      String     lastSearchSpec;
      static      bool       lastSearchAlias;
      static      bool       lastSearchExact;
      static      bool       lastSearchFuzzy;
      static      long       lastSearchRecord;

      bool        search     ();

   public:
                  _User      ();

      static void setDir     ( char *dir )
         {
            sysDir = dir;
         }

      static long   numUsers();
      static String findAlias(char *name);

      void fromRa( RA2_UsersBBS & , RA2_UsersXiBBS & , RA2_UsersPbBBS &);
      void   toRa( RA2_UsersBBS & , RA2_UsersXiBBS & , RA2_UsersPbBBS &);

      bool         read       ( long recno );
      bool         write      ( bool append );
      bool         search     ( char *name , bool exact = TRUE , bool fuzzy = FALSE , bool alias = FALSE);
      bool         searchNext ();

      void         newUser    ();

      String       firstName  ();

    //----------------------

    long         record;                // Record number in USERS.BBS

    char         name[36];              // User name
    char         alias[36];             // Alias/Handle
    char         passWord[16];          // Password
    dword        passWordCRC;
    word         level;                 // Security level
    char         country[26];           // Country
    char         state[26];             // State
    char         city[26];              // City
    char         company[51];           // Company

    char         address1[51];          // Address line 1
    char         address2[51];          // Address line 2
    char         address3[51];          // Address line 3

    char         comment[81];           // Comment
    char         forwardTo[36];         // Forward mail to

    Date         birthDate;             // Birthday

    char         voicePhone[16];        // Voice phone #
    char         dataPhone [16];        // Data phone #
    char         faxPhone  [16];        // Fax phone #

    byte         sex;                   // 0 = Unknown, 1 = Male, 2 = Female
    byte         dateFormat;            // Date Format
    byte         defaultProtocol;       // Default protocol hotkey
    char         language[9];           // Language
    word         screenWidth;           // Screen Width
    word         screenLength;          // # of lines

    sdword       timeUsed;              // time used today

    dword        timesCalled;           // # times called
    dword        numDownloads;          // # downloads
    dword        kbDownloaded;          // K downloaded
    dword        numUploads;            // # downloads
    dword        kbUploaded;            // K downloaded
    dword        msgsPosted;            // # messages posted

    Date         lastDate;              // Date last called
    Time         lastTime;              // Time last called
    Date         lastPwdChange;         // Date of last password change
    Date         lastNewFilesCheck;     // Date of last check for new files

    AccessFlags  aFlags;                // Access flags       (A-Z,1-6)
    dword        uFlags;                // Flags (DELETED/ANSI/PAUSE/...)
    word         logLevel;              // Log level of user

    CombinedType mailCheckBoards;       // Msg areas to check for mail
    CombinedType combinedBoards;        // Areas to be used in combined mode

    dword        totalTimeUsed;         // Total time used (minutes)

    Date         expDate;               // Expiration date
    word         expLevel;              // Fallback level after expiration
    AccessFlags  expFlagsOn;            // Flags to be enabled after expiration
    AccessFlags  expFlagsOff;           // Flags to be disabled after expiration

    Date         firstDate;             // Date of first login
    long         kbToday;               // K downloaded today
    long         credit;                // Netmail credit
    long         pending;               // Netmail credit pending
    word         fileArea;              // Last file area accessed
    word         msgArea;               // Last msg area accessed

    sword        tbTimeBalance;         // Time balance           (Time Bank)
    sword        tbKbBalance;           // Kbyte balance          (Time Bank)
    sword        tbTimeWithdrawn;       // Time withdrawn today   (Time Bank)
    sword        tbKbWithdrawn;         // Kbytes withdrawn today (Time Bank)
    word         tbTimeDeposited;
    word         tbKbDeposited;
    word         tbTimeLoaned;
    word         tbKbLoaned;
    Date         tbLastUsed;            // Date last used         (Time Bank)

    AskType      checkMail;             // Check for mail at logon
    AskType      checkNewFiles;         // Check for new files at logon

    dword        highMsgRead;

    word         qwkMaxMsgsPerArea;
    word         qwkMaxMsgs;
    byte         qwkArchiver;
    byte         ripFont;

    Date         tbTimePayback;
    Date         tbKbPayback;

    word         fileGroup;              // Last file area accessed
    word         msgGroup;               // Last msg area accessed

    byte         extra[390];
  };

struct _Config
  {
   char        shellmsg[81];         // Message to show when shelling
   char        sysopname[36];        // Name of sysop
   char        txtpath[61];          // Path for textfiles
   char        mnupath[61];          // Path for menu-files
   char        msgpath[61];          // Path for message base
   char        uploadpath[61];       // Uploadpath
   char        editorname[61];       // Name of external editor
   word        newuserlevel;         // Level for new user
   int         newuserloglevel;       // Loglevel for new user
   AccessFlags newuserflags;  // New user flags
   int         max_passinput;         // Maximum attempts for password entry
   int         min_passlength;        // Minimum password length
   int         inactivity_time;       // Inactivity time-out limit
   int         max_sysop_pages;       // Maximum times sysop can be paged
   int         pagebell_length;       // Length of page-bell (secs)
   int         mailcheck;             // Check for mail at logon?
   int         europe;                // European date format?
   int         numnodes;              // # nodes
   bool        allowansi;             // Allow ANSI?
   bool        allowavatar;           // Allow AVATAR?
   int         discrete;              // Hide sysop activity?
   int         askvoicephone;         // Ask for phone number?
   int         allowoneword;          // Allow one-word names
   unsigned    crashlevel;            // Level needed for crashmail
   AccessFlags crashflags;            // Flags needed for crashmail
   unsigned    attachlevel;           // Level needed for file attach
   AccessFlags attachflags;           // Flags needed for file attach
   int         allowmsgupload;        // Allow message uploads
   int         allowstacking;         // Allow command stacking
   Time        page_start;            // Paging hours start
   Time        page_end;              // Paging hours end
   int         handshaking;           // I/O Handshaking
   int         allowalias;            // Allow alias for login
   int         loglocal;              // Log local calls
   int         doswap;                // Allow swapping
   char        originline[61];        // Origin line
   char        nodelistdir[61];       // Nodelist directory
   char        sysopkeys[10][60];     // Sysop hotkeys
   Time        dl_start;              // Download hours start
   Time        dl_end;                // Download hours end
   int         uploadspace;           // Space needed for uploads
   char        pvtuploadpath[61];     // Directory for files uploads
   char        quotestring[6];        // String used for quoting
   int         fastmode;              // Use fast mode
   bool        killsent;              // Kill netmail after sent
   bool        multiline;             // Use message base sharing?
   bool        egamode;               // Use 43/50 line mode
   bool        showuserinfo;          // Show user info while in EGA mode?
   char        pexpath[61];           // Directory for PEX files
   bool        allowquicklogin;       // Allow quick sysop login?
   bool        suspendmsgtime;        // Suspend time when writing msgs
   int         securityboard;         // MsgBoard for security messages
   int         pwdmessages;           // Write security-messages?
   char        bbsname[36];           // Name of the BBS
   char        pwdchar;               // Password character
   int         tb_maxtimedeposit;
   int         tb_maxkbdeposit;
   int         tb_maxtimewithdrawal;
   int         tb_maxkbwithdrawal;
   int         usage_days;            // Days to keep usage graphs
   char        systempwd[16];
   bool        usesystempwd;
   bool        askBirthDay;
   int         binlogdays;
   bool        binloglocal;
   int         pageArea;
   bool        indexfiles;
   bool        checkdupes;
   bool        killdupes;
   bool        ignore_ext;
   char        RIPpath[61];          // Path for RIP scripts
   char        iconspath[61];
   char        location[36];
   char        phone[26];
   char        QWKid[9];
   word        IObuffersize;
   TimeFrame   pagingHours;
   char        defaultLanguage[9];
   bool        addUploaderName;
   TimeFrame   downloadHours;
   bool        askdataphone;
   bool        askfaxphone;
   bool        askaddress;
   bool        asksex;
   bool        askdateformat;
   bool        askstate;
   bool        askcountry;
   short       fuzzyRate;
   bool        hidePassword;
   bool        valConfirm;

   char        dateStampSignature[10];
   Date        dateStamp;
   dword       dateStampCrc;

   char        extChat[81];
   char        virScanCommand[61];
   byte        virScanType;
   char        virScanSemaphore[13];

   byte        listColorTag;
   byte        listColorFileName;
   byte        listColorSize;
   byte        listColorDate;
   byte        listColorCounter;
   byte        listColorDesc;
   byte        listColorSeperator;
   byte        listColorMissing;
   bool        hideFileCounter;

   bool        disableRIP;

   char        extMultiChat[81];

   byte        virScanFailedAction;
   word        virScanFailedArea;

   byte        promptColor;

   bool        internalfsed;

   char        extra[3];
  };

const byte VIRSCAN_ACTION_NONE   = 0;
const byte VIRSCAN_ACTION_MOVE   = 1;
const byte VIRSCAN_ACTION_DELETE = 2;

struct FilesIdx
  {
   char name[13];
   word area;
  };

struct _AreaGroup
  {
   char        name[80];
   word        level;
   AccessFlags flags;
   AccessFlags flagsNot;
   byte        extra[10];
  };

struct _FileArea
  {
   char name[80];        // Name of file area
   char listpath[80];    // Path for file-list
   char filepath[80];    // Path for files
   word level;           // Access level
   AccessFlags flags;    // Access flags
   bool cdrom;           // 1 = CDROM File listing type
   int maxfiles;         // Max files per day in this area downloadable
   int maxkb;            // Max Kbytes per day in this area
   bool notops;          // TRUE if not to appear in TOPFILES.A??
   bool free;            // TRUE if area is free
   byte groups[4];       // Groups belonging to
   bool allGroups;       // Belongs to all groups
   byte minAge;          // Minimum age
   AccessFlags flagsNot; // Access flags
   byte dateFormat;
   byte extra[2];
  };

#define MSG_BOTH  0
#define MSG_PVT   1
#define MSG_PUB   2
#define MSG_TOALL 3

#define MSG_LOCAL   0
#define MSG_NET     1
#define MSG_ECHO    2
#define MSG_PVTECHO 3

const byte MSGBASE_HUDSON = 0;
const byte MSGBASE_SQUISH = 1;
const byte MSGBASE_SDM    = 2;
const byte MSGBASE_JAM    = 3;

/*
#define MSGATTR_DELETED 1
#define MSGATTR_UNMOVED_NET 2
#define MSGATTR_NET 4
#define MSGATTR_PVT 8
#define MSGATTR_RECEIVED 16
#define MSGATTR_UNMOVED_ECHO 32
#define MSGATTR_LOCAL 64
#define MSGATTR_RESERVED 128

#define NETATTR_KILL 1
#define NETATTR_SENT 2
#define NETATTR_ATTACH 4
#define NETATTR_CRASH 8
#define NETATTR_REQ 16
#define NETATTR_AUD 32
#define NETATTR_RET 64
#define NETATTR_RESERVED 128
*/

/*
struct _MsgArea1
  {
   int  areaNum;
   char name[31];             // Name of message areas                             2
   byte msgType;              // Type of message (Net/Echo/Local)                 33
   byte msgKind;              // Kind of message (Private only/Public only/...)   34
   byte msgBaseType;          // Type of message base                             35
   char path[80];             // Path to Squish or *.MSG                          36
   byte flags;                // Alias allowed/forced/prohibited                 116
   word readLevel;            // Minimum level needed to read msgs               117
   AccessFlags readFlags;     // flags needed to read msgs                       119
   word writeLevel;           // Minimum level needed to write msgs              123
   AccessFlags writeFlags;    // flags needed to write msgs                      125
   word sysopLevel;           // Minimum level needed to change msgs             129
   AccessFlags sysopFlags;    // flags needed to change msgs                     131

   char origin[62];           // Origin line                                     135
   int aka;                   // AKA                                             197

   int rcvKillDays;           //                                                 199
   int msgKillDays;           //                                                 201
   int maxMsgs;               //                                                 203

   char sysop[36];            //                                                 205
   int  replyBoard;           //                                                 241

   char tag[21];              // EchoMail Tag Name                               243
   char qwkname[13];          // QWK Area Name                                   264

   byte groups[4];            // Groups belonging to                             277
   bool allGroups;            // Belongs to all groups                           281
   byte minAge;               // Minimum age for this area                       282
   AccessFlags readFlagsNot;  // flags non-grata to read msgs                    283
   AccessFlags writeFlagsNot; // flags non-grata to read msgs                    287
   AccessFlags sysopFlagsNot; // flags non-grata to read msgs                    291

   byte extra[5] ;            //                                                 295
  };
*/

struct _MsgArea
  {
   word areaNum;              // Area number                                       0
   word hudsonBase;           // Hudson area number                                2
   char name[81];             // Name of message areas                             4
   byte msgType;              // Type of message (Net/Echo/Local)                 85
   byte msgKind;              // Kind of message (Private only/Public only/...)   86
   byte msgBaseType;          // Type of message base                             87
   char path[81];             // Path to Squish or *.MSG                          88
   byte flags;                // Alias allowed/forced/prohibited                 169
   word readLevel;            // Minimum level needed to read msgs               170
   AccessFlags readFlags;     // flags needed to read msgs                       172
   AccessFlags readFlagsNot;  // flags non-grata to read msgs                    176
   word writeLevel;           // Minimum level needed to write msgs              180
   AccessFlags writeFlags;    // flags needed to write msgs                      182
   AccessFlags writeFlagsNot; // flags non-grata to write msgs                   186
   word sysopLevel;           // Minimum level needed to change msgs             190
   AccessFlags sysopFlags;    // flags needed to change msgs                     192
   AccessFlags sysopFlagsNot; // flags non-grata to change msgs                  196

   char origin[62];           // Origin line                                     200
   word aka;                  // AKA #                                           262

   word rcvKillDays;          //                                                 264
   word msgKillDays;          //                                                 266
   word maxMsgs;              //                                                 268

   char sysop[36];            //                                                 270
   word replyBoard;           //                                                 306

   char tag[61];              // EchoMail Tag Name                               308
   char qwkname[13];          // QWK Area Name                                   369

   byte groups[4];            // Groups belonging to                             382
   bool allGroups;            // Belongs to all groups                           386
   byte minAge;               // Minimum age for this area                       387

   byte extra[112] ;          //                                                 388
  };


struct _OldModemConfig
  {
   unsigned baud;
   int port;
   int quiet;
   int blanktime;
   char initstring[70];
   char busystring[70];
   char initok[40];
   char busyok[40];
   char connect300[40];
   char connect1200[40];
   char connect2400[40];
   char connect4800[40];
   char connect9600[40];
   char connect19200[40];
   char connect38400[40];
   char ring[40];
   char answer[40];
   char connect7200[40];
   char connect12000[40];
   int forceanswer;
   char extra2[4];
   int delay;
   char connect14400[40];
   char initstring2[70];
   char connect16800[40];
   char connect28800[40];
   char connect24000[40];
   char connect64000[40];
   int  ext_errlevel1;
   int  ext_errlevel2;
   byte extra[36];
  };

const dword MODEM_LOCKED        = 0x00000001;
const dword MODEM_MANUAL_ANSWER = 0x00000002;


struct _ModemConfig
   {
      long  maxBps;
      long  lockedBps;
      dword flags;
      int   port;
      int   commandDelay;
      int   answerDelay;
      int   blankTime;

      char  msgCon300       [80];
      char  msgCon1200      [80];
      char  msgCon1275      [80];
      char  msgCon2400      [80];
      char  msgCon4800      [80];
      char  msgCon7200      [80];
      char  msgCon9600      [80];
      char  msgCon12000     [80];
      char  msgCon14400     [80];
      char  msgCon16800     [80];
      char  msgCon19200     [80];
      char  msgCon21600     [80];
      char  msgCon24000     [80];
      char  msgCon26400     [80];
      char  msgCon28800     [80];
      char  msgCon38400     [80];
      char  msgCon57600     [80];
      char  msgCon64000     [80];
      char  msgCon115200    [80];

      char  msgConExternal  [80];

      char  msgRing         [80];
      char  msgOk           [80];

      char  cmdInit1        [80];
      char  cmdInit2        [80];
      char  cmdInit3        [80];
      char  cmdAnswer       [80];
      char  cmdOffHook      [80];
      char  cmdDown         [80];
      char  cmdAttention    [80];

      int   externalErrorLevel;

      char  msgCon31200     [80];
      char  msgCon33600     [80];
      char  msgCon36000     [80];

      char  msgConUser1     [80];
      long  userConBps1;
      char  msgConUser2     [80];
      long  userConBps2;
      char  msgConUser3     [80];
      long  userConBps3;
      char  msgConUser4     [80];
      long  userConBps4;
      char  msgConUser5     [80];
      long  userConBps5;
      char  msgConUser6     [80];
      long  userConBps6;

      byte  extra[920];
   };


struct _TimeLog
  {
  int numdays;
  Date lastdate;
  long hours[24];
  long days[7];
  long weeks[53];
  long totalcalls;
  int  daysactive;
  byte extra[100];
  };

struct _AccessEvent
  {
    bool        hoursActive[48];
    bool        daysActive[7];

    word        minLevel;
    AccessFlags flagsNeeded;

    int         downloadPercent;
    word        downloadPercent_ol;
    AccessFlags downloadPercent_of;

    int         downloadLimit;
    word        downloadLimit_ol;
    AccessFlags downloadLimit_of;

    int         timePercent;
    word        timePercent_ol;
    AccessFlags timePercent_of;

    int         timeLimit;
    word        timeLimit_ol;
    AccessFlags timeLimit_of;

    bool        allowYell;
    word        allowYell_ol;
    AccessFlags allowYell_of;

    byte        extra[106];   // Makes 200 bytes
  };

#define QMSG_DELETED 1
#define QMSG_UNMOVED_NET 2
#define QMSG_NET 4
#define QMSG_PVT 8
#define QMSG_RECEIVED 16
#define QMSG_UNMOVED_ECHO 32
#define QMSG_LOCAL 64
#define QMSG_RESERVED 128

struct _QbbsMsgHdr
  {
   int  msgnum;
   int  prevmsg;
   int  nextmsg;
   int  tread;
   word startrec;
   int  numrecs;
   int  destnet,destnode,orgnet,orgnode;
   char destzone,orgzone;
   int  cost;
   byte msgattr,netattr,area;
   char posttime[6];
   char postdate[9];
   char to[36];
   char from[36];
   char subj[67];
   Date recvdate;
   Time recvtime;

   bool fix();      // Message valid?
  };

struct LanguageFile
   {
      char name[41];
      bool avail;
      word level;
      AccessFlags flags;
      char menuPath[61];
      char txtPath[61];
      char questPath[61];
      char copyright[81];
      byte attrib;
      byte extra[500];
   };

const byte LANG_PROMPT  = 0x01;
const byte LANG_NOCOLOR = 0x02;

struct _LanguageString
  {
   word len;               // Length of string (excl. terminating '\0')
   word numHotkeys;        // # Hotkeys defined
   byte color;             // Main color
   byte highlight;         // Highlight color
   byte promptcolor;       // Prompt color
   byte flags;             // Prompt/NoColor
   char *s;                // Actual string
   char *hotkeys;          // List of hotkeys

   _LanguageString();
   _LanguageString( const _LanguageString& );
   _LanguageString( char *s , char *hotkeys , byte color , byte highlight , byte flags = 0x00, byte promptcolor = 0x00);

  ~_LanguageString();

   bool read( File& );
   bool write( File& );
   void operator=( const _LanguageString& );
   void clear();
  };

const dword MENU_RIP = 1;

const dword MENU_RIP_SHOWREMOTE = 1;
const dword MENU_RIP_HIDELOCAL  = 2;
const dword MENU_RIP_NO_RESET   = 4;
const dword MENU_PASSWORD       = 8;

struct _MenuFile
   {
      dword attr;   // bit 0 = RIP menu
      char prompt[200];
      byte color;
      byte highlight;
      char RIPname[9];
      byte extra[100];
   };

struct _MenuItem
   {
      dword attr;     // Show remote/local - reset RIP
      char text[160];
      char data[160];
      byte color;
      byte hotKey;
      word function;
      char password[16];

      AccessRecord access;

      byte extra[50];
   };


const dword PVTFILE_KEEP = 0x00000001uL;

struct _PrivateFile
   {
      Date date;
      char fname  [80];
      char to     [36];
      char from   [36];
      char desc   [80];

      dword attr;

      byte extra  [61];
   };

String parse_lang(char *fmt , ...);

void getsyspath(char *syspath,char *argv0);
dword RA_CRC(char *s);
dword RaCrc(char *s);
dword JamCrc(char *s);
