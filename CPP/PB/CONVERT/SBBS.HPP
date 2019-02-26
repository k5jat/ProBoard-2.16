struct SBBS_UsersBBS
 {
  char Name[36];
  char City[26];
  char Password[16];
  char DataPhone[13],HomePhone[13];
  char LastTime[6];
  char LastDate[9];
  char Attrib;
  char Flags[4];
  int  Credit,Pending;
  unsigned int MsgsPosted,
  HighMsgRead,
  SecLvl,
  Times,
  Ups,
  Downs,
  UpK,
  DownK;
  int TodayK,Elapsed,Len;

  char ExtraSpace1[2];
  char Age;
  int ExtraUserrecPtr;
  char ExtraSpace2[3];
 };

struct SBBS_SUsersBBS
 {
  char Name[36];
  char Birthday[9];
  unsigned int Attrib;
  char Flags[4];
  char Firsttime[6];
  char FirstDate[9];
  char CombinedBoards[25];
  char SysopComment[80];
  char DefaultProto;
  int UserRecPtr;
  char Colors[10];
  char FileListType;
  char Alias[36];
  long MinutesUsed;
  char ViewFileName[13];
  char MenuToRun[9];
  unsigned int Timeinbank;
  char TodayCalls;
  char LanguageFileN[9];
  char Expirationdate[9];
  char CurrentFArea, CurrentMArea,CurrentDoor;
  char DefaultPacker;
  char LastNewFilesCheck[9];
  char CallsSinceBverify;
  char ExtraSpace[402];
 };

struct sufi
{
 char Name[61];
 char Directory[80];
 int  Security;
 long Flags;
 int  PFilesSec;
 long PFlags;
 char Attrib;
 char Age;
 char ListName[68];
 unsigned int RequiredBPSrate;
 char ExtraSpace[28];
};

struct sumsg
{
 char Name[31];
 char QwkName[13];

 char Typ;
 char Kinds; // oeps!!!! 3=Read-Only
 char Aliases;

 int ReadSecLvl;
 long ReadFlags;

 int WriteSecLvl;
 long WriteFlags;

 int SysopSecLvl;
 long SysopFlags;

 char Group;
 char replystat;
 char age;
 char Attrib;
 char UseAka;
};

struct eventrec
{
 char Status;
 char RunTime[6];
 char ErrorLevel;
 char Days;
 int Forced;
 char LastTimeRun[9];
};

struct unusedboards
{
 char Name[17];
 char Typ;
 char Kinds;
 int Combined;
 char Aliases;

 int ReadSecLvl;
 char ReadFlags[4];

 int WriteSecLvl;
 char WriteFlags[4];

 int SysopSecLvl;
 char SysopFlags[4];
};

struct supcon
{
 int CommPort;
 unsigned int InitBaud,
 InitTimes,
 AnswerWait;
 char ModemInitStr[71],
 ModemBusyStr[71];
 char ModemInitResp[41],
 ModemBusyResp[41],
 Resp300[41],
 Resp1200[41],
 Resp2400[41];

 char MenuPath[67],
 TextPath[67],
 NetPath[67];

 int MinBaud,
 GraphicsBaud,
 XferBaud;
 char LowBaudStart[6],
 LowBaudEnd[6],
 DownloadStart[6],
 DownloadEnd[6],
 PagingStart[6],
 PagingEnd[6];

 int MatrixZone,
 MatrixNet,
 MatrixNode;
 int OldAkaNet[5],
 OldAkaNode[5];
 int NetMailBoard;

 int DefaultSec;
 int DefaultCredit;
 char DefaultFlags[4];

 char EditorCmdStr[71];
 char OriginLine[61];
 char SysopName[36];
 char AutoLogonChar,
 FastLogon,
 ScreenBlanking,
 UseLastRead,
 MonoMode,
 DirectWrite,
 SnowCheck,
 NetEchoExit,
 OneWordNames,
 CheckMail,
 AskHomePhone,
 AskDataPhone,
 GraphicsAvail;
 int InactiveTimeOut;
 int LogonTime;
 int DefFgColor;
 int DefBgColor;
 int PasswordTries;
 int MaxPageTimes;
 int PageBellLen;

 char Use_Xmodem;
 char Use_Xmodem1k;
 char Use_Ymodem;
 char Use_YmodemG;
 char Use_Sealink;
 char Use_Zmodem;
 char Inp_Fields;
 char QuoteStr[4];
 int UploadCredit;
 char LoadingMessage[71];
 char SelectionPrompt[71];

 unsigned int VersionID;
 char Resp4800[41],
 Resp9600[41];

 int OldAkaZone[5];

 int MatrixPoint;
 int OldAkaPoint[5];

 char OldUseAka[200];
 char AskAge;
 char SystemName[41];
 long RegKey;

 char EntryfieldColor;
 char MenuBorderColor;

 int SysOpSeclvl;
 int AllowDelMsgs;

 eventrec evrec[30];
 unusedboards BoardRecord[200];
};

struct
supev
{
 char Status;
 char RunTime[6];
 char ErrorLevel;
 char Days;
 char Forced;
 char LastTimeRun[9];
};

struct
supcon2
{
 char SystemPath[67];
 char MsgBasePath[67];
 char TempPath[67];
 char RedirectDevice[13];
 char Resp19200[41];
 char Resp38400[41];
 char ShowSecurity;
 char ShowPswdAtLogon;
 long BetaCode;
 char Unused[200];
 char Empty1[284];
 char AllowCat;
 char Answerboard;
 char Use7bitChars;
 unsigned int BufferSize;
 char Empty2[45];
 char EntryColor;
 char FilesBeforeRatio;
 unsigned int MinSpace;
 char ClearTemp;
 char Empty3[163];
 char UseAliasSystem;
 char Ansilogon;
 char ExcludeSysOp;
 char Empty4[122];
 char AltFkeys[10][61];
 char Checkfiles;
 char PasswordBoard;
 char Alt_J_Swaptoems;
 unsigned int MaxTimeInBank;
 char NoBirthDayAsk;
 char MaxDescLines;
 char Empty5[45];
 char FastMailCheck;
 char DefaultColors[10];
 char Empty6[45];
 char LinesInSystem;
 char CommonDir[80];
 unsigned int VersionNumber;
 unsigned int FullScrChatBaud;
 char PressEnterColor;
 char AreaSelectColor[3];
 char StatusLineColor;
 char VirusChkMode;
 char WorkPath[80];
 char WatchCDatDos;
 char UseCtrlX;
 char MsgAfterPageBoard;
 char SearchAliases;
 char EchoConvert;
 char SwapAtPackers;
 char InternalProtocols;
 char FirstMenuInSystem[9];
 char ColoredQuoteText;
 char AddUploadersName;
 unsigned int DaysUntilExpiration;
 char QWKBBSID[7];
 char QWKDefaultBoard;
 int QwkMaxNrToPack;
 char StatuslineStr[2][101];
 char Location[41];
 char UsPhoneNrFormat;
 char DlCounterLBracket;
 char DlCounterRBracket;
 char QWKAllowTagLines;
 unsigned int LogonSecLvl;
 char LogonAge;
 char LogonPassword[16];
 char NoGroupMailCheck;
 char NoUserListIntype27;
 unsigned int GroupMsgSecLvl;
 char AskSex;
 char AskAnsi;
 char AskMailCheck;
 char AskNewFilesCheck;
 char AskFullScrEditor;
 char AskFullScrViewer;
 char AskPagePause;
 char AskScreenClears;
 char PageStart[6][7];
 char PageEnd[6][6];
 char UseIEMSI;
 char CapitalizeCity;
 char CapitalizeAlias;
 char VirusScannerCmd[128];
 char NoExtractBeforeScan;
 char VirusFoundErrorLvl;
 char SysOpsFscrChatClr;
 char UsersFscrChatClr;
 char ScanForDupePhones;
 char NormalLongDesc;
 char NodelistPath[73];
 unsigned int MinCrashMailSec;
 char NoDorinfo1;
 char NoTouch;
 char ZoneMatching;
 char strictdupecheck;
 char noextendednodeinfo;
 char Birthdayverify;
 char realmsgnumbers;
 char LocalAttachPath[67];
 char NoLACheckAtLogon;
 char AllowedPackers;
 char BoardsFileToCurDir;
 char UnLockPwd[16];
 char Resp7200[41];
 char Resp12000[41];
 char Resp14400[41];
 char LineEditorQuote;
 char SevenBitOnly;
 char nofileflagging;
 char ReplyingToStr[81];
 char FreeFileTimeCheck;
 char PageReasonToSubj;
 char AgeGraphStartAge;
 unsigned int ETGCLocalAttrib;
 unsigned int ETGCRemoteAttrib;
 char AskCombinedBoards;
 char AskCombMailCheck;
 char UseNewFilesCheckDate;
 unsigned int BurnOutSecs;
 char NoNamesInGroupMail;
 char AutoChatCapture;
 char AskDefProto;
 char AskHotkeys;
 char FreeAttaches;
 char LogonFlags[4];
 char NoRemoteSysOp;
 char FscrReaderColors[12];
 char NewsDateCheck;
 char NoNameIdx;
 int AkaZone[15];
 int AkaNet[15];
 int AkaNode[15];
 int AkaPoint[15];
 char MinorVersionNumber;
 char Resp16800[41];
 char DlCounterDigits;
 char MultiWordNames;
 char AllowCtrlKeysInMsgs;
 char NoChatTimeCredit;
 char verifycity;
 char FilesBBSdatecheck;
 char Newuserpassword[16];
 char ShowRulesTimes;
 char FilesBBSWrapPos;
 char AutoAddDlCounters;
 char ExtChatCmdLine[80];
 char ArqResp[15];
 char Arqrequired;
 char Extraspace[2547];
};
