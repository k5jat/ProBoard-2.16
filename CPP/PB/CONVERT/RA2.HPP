/*
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

   byte         extra[400];
  };

struct RA2_UsersIdxBBS
  {
   dword NameCRC;
   dword HandleCRC;
  };
*/

#define RA2_UFLAG1_DELETED  1
#define RA2_UFLAG1_CLRSCR   2
#define RA2_UFLAG1_MORE     4
#define RA2_UFLAG1_ANSI     8
#define RA2_UFLAG1_NOKILL  16
#define RA2_UFLAG1_IGNORE  32
#define RA2_UFLAG1_FSE     64
#define RA2_UFLAG1_QUIET  128

#define RA2_UFLAG2_HOTKEYS     1
#define RA2_UFLAG2_AVATAR      2
#define RA2_UFLAG2_FSMSG       4
#define RA2_UFLAG2_HIDDEN      8
#define RA2_UFLAG2_PAGEPRI    16
#define RA2_UFLAG2_NOECHOSCAN 32
#define RA2_UFLAG2_GUEST      64
#define RA2_UFLAG2_POSTBILL  128

#define RA2_UFLAG3_NOIBM       1
#define RA2_UFLAG3_NOTOPS      2
#define RA2_UFLAG3_AVTPLUS     4
#define RA2_UFLAG3_ATTEN       8

struct RA2_MsgArea
   {
      byte extra1[4];
      char Name[41];
      byte Typ;
      byte MsgKinds;
      byte Attribute;
                      //{ Bit 0 : Enable EchoInfo
                      //      1 : Combined access
                      //      2 : File attaches
                      //      3 : Allow aliases
                      //      4 : Use SoftCRs as characters
                      //      5 : Force handle
                      //      6 : Allow deletes
                      //      7 : Is a JAM area }
      byte DaysKill;
      byte RecvKill;
      word CountKill;
      word ReadSecurity;
      long ReadFlags;
      long ReadNotFlags;
      word WriteSecurity;
      long WriteFlags;
      long WriteNotFlags;
      word SysopSecurity;
      long SysopFlags;
      long SysopNotFlags;
      char OriginLine[61];
      byte AkaAddress;
      byte Age;
      char JAMbase[61];
      word Group;
      word AltGroup[3];
      byte Attribute2;
                      //{ Bit 0 : Include in all groups }
      byte extra2[9];
   };


struct RA2_FileArea
   {
      byte Unused[4];
      char Name[41];
      byte Attrib;
      char FilePath[41];
      word KillDaysDL;
      word KillDaysFD;
      char Password[16];
      word MoveArea;
      byte Age;
      byte ConvertExt;
      word Group;
      byte Attrib2;
      word DefCost;
      word UploadArea;
      word UploadSecurity;
      dword UploadFlags;
      dword UploadNotFlags;
      word Security;
      dword Flags;
      dword NotFlags;
      word  ListSecurity;
      dword ListFlags;
      dword ListNotFlags;
      word  AltGroup[3];
      byte Device;
      byte FreeSpace[13];
   };


struct RA2_Config
  {
   int VersionID;
   byte CommPort;
   long Baud;
   byte InitTries;
   char InitStr[71];
   char BusyStr[71];
   char InitResp[41];
   char BusyResp[41];
   char Connect300[41];
   char Connect1200[41];
   char Connect2400[41];
   char Connect4800[41];
   char Connect9600[41];
   char Connect19k[41];
   char Connect38k[41];
   bool AnswerPhone;
   char Ring[21];
   char AnswerStr[21];
   bool FlushBuffer;
   int  ModemDelay,
        MinimumBaud,
        GraphicsBaud,
        TransferBaud;
   char SlowBaudTimeStart[6];
   char SlowBaudTimeEnd[6];
   char DownloadTimeStart[6];
   char DownloadTimeEnd[6];
   char PagingTimeStart[6];
   char PagingTimeEnd[6];
   char LoadingMsg[71];
   char ListPrompt[71];
   int  PwdExpiry;

   char MenuPath[61];
   char TextPath[61];
   char AttachPath[61];
   char NodelistPath[61];
   char MsgBasePath[61];
   char SysPath[61];
   char ExternalEdCmd[61];

   struct
     {
      int zone,net,node,point;
     } Address[10];

//   int Address[40];
   char SystemName[31];

   int NewSecurity,
       NewCredit;

   long NewFlags;

   char OriginLine[61];
   char QuoteString[16];
   char Sysop[36];
   char LogFileName[61];
   bool FastLogon,
        AllowSysRem,
        MonoMode,
        StrictPwdChecking,
        DirectWrite,
        SnowCheck;
   int  CreditFactor,
        UserTimeOut,
        LogonTime,
        PasswordTries,
        MaxPage,
        PageLength;
   bool CheckForMultiLogon,
        ExcludeSysopFromList,
        OneWordNames;
   byte CheckMail;
   bool AskVoicePhone,
        AskDataPhone,
        DoFullMailCheck,
        AllowFileShells,
        FixUploadDates,
        ShowFileDates;
   byte ANSI,
        ClearScreen,
        MorePrompt;
   bool UploadMsgs;
   byte KillSent;

   int  CrashAskSec;
   long CrashAskFlags;
   int  CrashSec;
   long CrashFlags;
   int  FAttachSec;
   long FAttachFlags;

   byte NormFore,
        NormBack,
        StatFore,
        StatBack,
        HiBack,
        HiFore,
        WindFore,
        WindBack,
        ExitLocal,
        Exit300,
        Exit1200,
        Exit2400,
        Exit4800,
        Exit9600,
        Exit19k,
        Exit38k;

    bool MultiLine;
    byte MinPwdLen;
    int  MinUpSpace;
    byte HotKeys;
    byte BorderFore,
         BorderBack,
         BarFore,
         BarBack,
         LogStyle,
         MultiTasker,
         PwdBoard;
    int  BufferSize;
    char FKeys[10][61];

    bool WhyPage;
    byte LeaveMsg;
    bool ShowMissingFiles;
    char MissingString[11];
    bool AllowNetmailReplies;
    char LogonPrompt[41];
    byte CheckNewFiles;
    char ReplyHeader[61];
    byte BlankSecs;
    byte ProtocolAttrib[6];
    char ErrorFreeString[16];
    byte xDefaultCombined[25];
    int  RenumThreshold;
    char LeftBracket,
         RightBracket;
    bool AskForHandle,
         AskForBirthDate;

    int  Unused;

    bool ConfirmMsgDeletes;
    char LocationPrompt[61];
    char PagePrompt[61];
    char UserfilePrompt[41];
    byte NewUserGroup;
    byte AVATAR;
    byte BadPwdArea;
    char Location[41];
    byte DoAfterAction;
    char CRprompt[41];
    byte CRfore,
         CRback;
    char ContinuePrompt[41];
    bool SendBreak;
    char ListPath[61];
    byte FullMsgView;
    bool EMSI_Enable,
         EMSI_NewUser;

    char EchoChar[2];
    char Connect7200[41];
    char Connect12000[41];
    char Connect14400[41];
    byte Exit7200;
    byte Exit12000;
    byte Exit14400;
    char ChatCommand[61];
    bool ExtEd;                          //??
    byte NewuserLanguage;
    char LanguagePrompt[41];
    byte VideoMode;                      //??
    bool AutoDetectANSI;
    bool OffHook;
    byte NewUserDateFormat;
    char KeyboardPwd[16];
    bool CapLocation;
    byte NewuserSub;
    char PrinterName[5];
    byte HilitePromptFore;
    byte HiLitePromptBack;
    char InitStr2[71];
    bool AltJSwap;
    bool SemPath[61];
    bool AutoChatCapture;

    char FileBasePath[61];
    bool NewFileTag;
    bool IgnoreDupeExt;
    char TempCDFilePath[61];
    byte TagFore;
    byte TagBack;
    char xConnect16k[41];
    byte Exit16k;
    byte FilePayback;
    char FileLine[201];
    char FileMissingLine[201];
    byte NewUserULCredit;
    word NewUserULCreditK;
    byte ArcInfo[1260];
    char RAMGRAltFKeys[5][61];
    char ArcViewCmd[61];
    char xConnectFax[41];
    byte ExitFax;
    bool UseXMS;
    bool UseEMS;
    byte CheckDOB;
    byte EchoCheck;
    word ccSec;
    word ReturnRecSec;
    bool HonourNetReq;
    word DefaultCombined[200];
    bool AskForSex;
    bool AskForAddress;
    byte DLdesc;
    bool NewPhoneScan;

    byte FutureExpansion[587];
  };


