#include <string.h>


#define RA1_UFLAG1_DELETED  1
#define RA1_UFLAG1_CLRSCR   2
#define RA1_UFLAG1_MORE     4
#define RA1_UFLAG1_ANSI     8
#define RA1_UFLAG1_NOKILL  16
#define RA1_UFLAG1_IGNORE  32
#define RA1_UFLAG1_FSE     64
#define RA1_UFLAG1_QUIET  128

#define RA1_UFLAG2_HOTKEYS 1
#define RA1_UFLAG2_AVATAR  2
#define RA1_UFLAG2_FSMSG   4
#define RA1_UFLAG2_HIDDEN  8

#define RA1_UFLAG3_NOIBM   1
#define RA1_UFLAG3_NOTOPS  2
#define RA1_UFLAG3_AVTPLUS     4
#define RA1_UFLAG3_ATTEN       8

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
   dword Flags;
   sword Credit;
   sword Pending;
   word
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
   word XIrecord;
   char extra[3];
  };

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
   CombinedType    mailcheckboards;
   CombinedType    boards;
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

struct RA1_MsgArea
  {
   char Name[41];
   char Type;      // (local,net,echo)
   char Kind;      // (Both, Private, Public, ROnly)
   char Attribute;

//    { Bit 0 : Enable EchoInfo
//          1 : Combined access
//          2 : File attaches
//          3 : Allow aliases
//          5 : Force handle     }

   byte DaysKill;
   byte RecvKill;
   int  CountKill;
   int  ReadLevel;
   long ReadFlags;
   int  WriteLevel;
   long WriteFlags;
   int  SysopLevel;
   long SysopFlags;

   char OriginLine[61];
   byte AkaAddress;
  };

struct RA1_FileArea
  {
   char Name[31];
   byte Attrib;
   char FilePath[41];
   byte extra[41];
   int  Level;
   long Flags;
   byte extra2[6];
  };

struct RA1_Config
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
    byte DefaultCombined[25];
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

    byte FutureExpansion[97];
  };
