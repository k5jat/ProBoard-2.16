#ifndef _PB_SDK_H
#define _PB_SDK_H

#define PBSDK_VERSION 215

/*ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸*/
/*³ ProBoard Software Development Kit v2.15 ³  C/C++ library declarations  ³*/
/*ÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµ*/
/*³           Only functions in this header file are supported             ³*/
/*ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´*/
/*³     !! ÄÄÄÄÄÄÄÄÄÄÄ>> Only use LARGE memory model <<ÄÄÄÄÄÄÄÄÄÄÄÄ !!     ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/


#ifndef PB_SDK
  #define PB_SDK 1
#endif

#ifdef __cplusplus
 extern "C" {
#endif

#define NULL (0L)
#define FAR far
#define TRUE (1)
#define FALSE (0)

typedef int           FILE;
typedef unsigned      size_t;
typedef long          clock_t;
typedef long          time_t;
typedef char         *va_list;

#define va_start(ap,parmn)  ((ap) = (va_list)&(parmn) + sizeof(parmn))
#define va_arg(ap,type) (*((type *)(ap))++)
#define va_end(ap)

struct find_t              /* used by dos_findfirst() & dos_findnext() */
  {
   char     reserved[21];      /* reserved by DOS                      */
   char     attrib;            /* attribute found (FA_XXXX)            */
   unsigned time,date;         /* file's last write                    */
   unsigned long size;         /* file's size                          */
   char     name[13];          /* filename followed by 0 byte          */
  };

/* Used in dos_findfirst & dos_findnext */

#define FA_NORMAL 0x00
#define FA_RDONLY	0x01
#define FA_HIDDEN	0x02
#define FA_SYSTEM	0x04
#define FA_LABEL	0x08
#define FA_DIREC	0x10
#define FA_ARCH		0x20

#define FP_SEG(fp)	((unsigned)((unsigned long)(fp) >> 16))
#define FP_OFF(fp)  ((unsigned)((unsigned long)(fp)))

#define MK_FP(seg,offset) ((void FAR *)(((unsigned long)(seg)<<16) | (unsigned)(offset)))

struct WREGS
   {
    unsigned ax,bx,cx,dx,si,di,cflag,flags;
   };

struct BREGS
   {
    unsigned char al,ah,bl,bh,cl,ch,dl,dh;
   };

union REGS
   {
    struct WREGS x;
    struct BREGS h;
   };

struct SREGS
   {
    unsigned es,cs,ss,ds;
   };

extern int errno;

#define ENOENT  2
#define ENOTDIR	3
#define EMFILE	4
#define EACCES	5
#define EBADF	6
#define ENOMEM	8
#define EINVAL	22
#define EEXIST	80

#define E2BIG	1000
#define ENOEXEC	1001
#define EDOM	1002
#define	ERANGE	1003

#define O_RDONLY  0
#define O_WRONLY	1
#define O_RDWR		2

#define O_NOINHERIT 0x80
#define O_DENYALL   0x10
#define O_DENYWRITE 0x20
#define O_DENYREAD  0x30
#define O_DENYNONE  0x40

#define S_IREAD  0x0100
#define S_IWRITE 0x0080

#define O_BINARY        0

#define _IOFBF  0
#define _IOLBF  1
#define _IONBF  2

#define F_OK    0       /* does file exist?     */
#define X_OK	1	/* execute permission?	*/
#define W_OK	2	/* write permission?	*/
#define R_OK	4	/* read permission?	*/

#define SEEK_SET	0	/* seek from start of file	*/
#define SEEK_CUR	1	/* relative to current position	*/
#define SEEK_END	2	/* relative to end of file	*/

struct tm
  {
   int tm_sec,         /* seconds 0..59                        */
       tm_min,         /* minutes 0..59                        */
       tm_hour,        /* hour of day 0..23                    */
       tm_mday,        /* day of month 1..31                   */
       tm_mon,         /* month 0..11                          */
       tm_year,        /* years since 1900                     */
       tm_wday,        /* day of week, 0..6 (Sunday..Saturday) */
       tm_yday,        /* day of year, 0..365                  */
       tm_isdst;       /* >0 if daylight savings time          */
                       /* ==0 if not DST                       */
                       /* <0 if don't know                     */
  };

FILE *  fopen(const char *,const char *);
FILE *  freopen(const char *,const char *,FILE *);
int     fseek(FILE *,long,int);
long    ftell(FILE *);
char *  fgets(char *,int,FILE *);
int     fgetc(FILE *);
int     fflush(FILE *);
int     fclose(FILE *);
int     fputs(const char *,FILE *);
int     getc(FILE *);
int     getchar(void);
char *  gets(char *);
int     fputc(int,FILE *);
int     putc(int,FILE *);
int     putchar(int);
int     puts(const char *);
size_t  fread(void *,size_t,size_t,FILE *);
size_t  fwrite(const void *,size_t,size_t,FILE *);
int     printf(const char *,...);
int     fprintf(FILE *,const char *,...);
int     vfprintf(FILE *,const char *,va_list);
int     vprintf(const char *,va_list);
int     sprintf(char *,const char *,...);
int     vsprintf(char *,const char *,va_list);
void    setbuf(FILE *,char *);
int     setvbuf(FILE *,char *,int,size_t);
int     remove(const char *);
int     rename(const char *,const char *);
void    rewind(FILE *);
void    clearerr(FILE *);
int     feof(FILE *);

int     isalnum(int);
int     isalpha(int);
int     iscntrl(int);
int     isdigit(int);
int     isgraph(int);
int     islower(int);
int     isprint(int);
int     ispunct(int);
int     isspace(int);
int     isupper(int);
int     isxdigit(int);
int     toupper(int);
int     tolower(int);

int     int86(int,union REGS *,union REGS *);
int     int86x(int,union REGS *,union REGS *,struct SREGS *);
int     intdos(union REGS *,union REGS *);
int     intdosx(union REGS *,union REGS *,struct SREGS *);
int     dos_findfirst(const char *, unsigned, struct find_t *);
int     dos_findnext(struct find_t *);

int     mkdir(const char *);
int     rmdir(const char *);
int     chdir(const char *);

int     read(int,void *,unsigned);
int     write(int,const void *,unsigned);
int     open(const char *,int);
int     creat(const char *,int);
int     close(int);
int     unlink(const char *);
int     chsize(int, long);
int     dup(int);
int     dup2(int, int);
long    lseek(int,long,int);
int     access(const char *,int);
long    filelength(int);
int     isatty(int);

int     atoi(const char *);
long    atol(const char *);
long    strtol(const char *,char **,int);
int     rand(void);
void    srand(unsigned);
void *  calloc(size_t,size_t);
void    free(void *);
void *  malloc(size_t);
void *  realloc(void *,size_t);
char *  getenv(const char *);
int     putenv(const char *);
int     abs(int);
long    labs(long);

void *  memcpy(void *,const void *,size_t);
void *  memmove(void *,const void *,size_t);
char *  strcpy(char *,const char *);
char *  strncpy(char *,const char *,size_t);
char *  strcat(char *,const char *);
char *  strncat(char *,const char *,size_t);
int     memcmp(const void *,const void *,size_t);
int     strcmp(const char *,const char *);
int     strncmp(const char *,const char *,size_t);
void *  memchr(const void *,int,size_t);
char *  strchr(const char *,int);
size_t  strcspn(const char *,const char *);
char *  strpbrk(const char *,const char *);
char *  strrchr(const char *,int);
size_t  strspn(const char *,const char *);
char *  strstr(const char *,const char *);
char *  strtok(char *,const char *);
void *  memset(void *,int,size_t);
size_t  strlen(const char *);

int     memicmp(const void *,const void *,size_t);
char *  stpcpy(char *,const char *);
int     strcmpl(const char *,const char *);
int     strnicmp(const char *,const char *, size_t);
char *  strdup(const char *);
char *  strlwr(char *);
char *  strupr(char *);
char *  strnset(char *,int,size_t);
char *  strrev(char *);
char *  strset(char *,int);
void    swab(char *,char *,size_t);

#define strncmpl(a,b,c) strnicmp((a),(b),(c))
#define stricmp(a,b)    strcmpl((a),(b))
#define delay(a)        msleep(a)

clock_t clock(void);
time_t  time(time_t *);
time_t  mktime(struct tm *);
char *  asctime(const struct tm *);
char *  ctime(time_t *);
struct tm * localtime(const time_t *);
struct tm * gmtime(const time_t *);
size_t  strftime(char *,size_t,const char *,struct tm *);

#define difftime(t1,t2)   (((time_t)(t1) - (time_t)(t2)))

void sleep(time_t);
void usleep(unsigned long);
void msleep(unsigned long);

/* ------------------------------------------------------------------------- */
/* ------ END OF STANDARD LIBRARY SECTION ---------------------------------- */
/* ------------------------------------------------------------------------- */

/*****************************************************************************/
/****  ProBoard specific #defines and structures  ****************************/
/*****************************************************************************/

typedef unsigned char  bool;
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;
typedef word           KEY;

typedef byte  DateType[3];
typedef byte  TimeType[3];

typedef byte  TimeFrame[7][6];

typedef dword accessflags;
typedef byte  combinedboards[125];

/* Modes for Input() */
#define INPUT_ALL       0
#define INPUT_UPFIRST   1
#define INPUT_UPALL     2
#define INPUT_DIGITS    3
#define INPUT_PWD      64   /* OR */
#define INPUT_NOFIELD 128   /* OR */

/* Paramaters for SetColor() */
#define BLACK   0x00
#define RED     0x01
#define GREEN   0x02
#define YELLOW  0x03
#define MAGENTA 0x04
#define BLUE    0x05
#define CYAN    0x06
#define WHITE   0x07

#define BLINK   0x10   /* OR together for flashing colors      */


/* Used for MenuFunction() */
#define MENU_GOTOMENU            1
#define MENU_GOSUBMENU           2
#define MENU_PREVMENU            3
#define MENU_GOTOMENUCLEAR       4
#define MENU_SHOWANSASC          5
#define MENU_COUNTRY_CHANGE      6
#define MENU_SHELL               7
#define MENU_VERSION_INFO        8
#define MENU_LOGOFF              9
#define MENU_USAGE_GRAPH        10
#define MENU_PAGESYSOP          11
#define MENU_QUESTIONNAIRE      12
#define MENU_USER_LIST          13
#define MENU_TIME_STAT          14
#define MENU_VIEW_ANS_WAIT      15
#define MENU_CITY_CHANGE        16
#define MENU_PASSWORD_CHANGE    17
#define MENU_LINES_CHANGE       18
#define MENU_CLS_TOGGLE         19
#define MENU_PAUSE_TOGGLE       20
#define MENU_ANSI_TOGGLE        21
#define MENU_MAILCHECK          22
#define MENU_READMSG            23
#define MENU_SCAN_MSG           24
#define MENU_QSCAN_MSG          25
#define MENU_DAYS_GRAPH         26
#define MENU_WRITEMSG           27
#define MENU_COMBINED_SELECT    28
#define MENU_WEEKS_GRAPH        29
#define MENU_RAW_DIR            30
#define MENU_LIST_FILES         31
#define MENU_DOWNLOAD           32
#define MENU_UPLOAD             33
#define MENU_LIST_ARCHIVE       34
#define MENU_KEYWORD_SEARCH     35
#define MENU_FILENAME_SEARCH    36
#define MENU_NEW_FILES          37
#define MENU_VIEW_FILE          38
#define MENU_VIEW_NAMED_FILE    39
#define MENU_FSED_TOGGLE        41
#define MENU_HOTKEY_TOGGLE      42
#define MENU_CLEARMARKED        43
#define MENU_COMBINED_CLEAR     44
#define MENU_VIEW_FILE_WAIT     45
#define MENU_CHANGE_ACCESS      46
#define MENU_LOGENTRY           47
#define MENU_TOPS               48
#define MENU_SET_MSGAREA        49
#define MENU_SHOW_USERS_ONLINE  50
#define MENU_LASTCALLERS        51
#define MENU_USEREDITOR         52
#define MENU_MULTICHAT          53
#define MENU_SET_FILEAREA       54
#define MENU_VIEW_GIF           55
#define MENU_IBM_TOGGLE         56
#define MENU_PHONE_CHANGE       57
#define MENU_DATAPHONE_CHANGE   58
#define MENU_HANDLE_CHANGE      59
#define MENU_RUN_SDKFILE        60
#define MENU_SHOW_BULLETIN      61
#define MENU_AVT0               62
#define MENU_AVT1               63
#define MENU_SHOW_GRAPH         64
#define MENU_RIP_FONT           66
#define MENU_RIP_GRAPHICS       67
#define MENU_EDIT_TAGGED        68
#define MENU_SELECT_LANGUAGE    69
#define MENU_CHANGE_DATE_FORMAT     70
#define MENU_CHANGE_MAILING_ADDRESS 71
#define MENU_CHANGE_FAX_NUMBER  72
#define MENU_CHANGE_COUNTRY     73


/* Handler modes */
#define HANDLER_SYSOPKEY         6
#define HANDLER_HANGUP           7

#define HANDLED     1
#define NOT_HANDLED 0

/* Loglevels */
#define LOG_FRIEND     0
#define LOG_NORMAL     1
#define LOG_SUSPICIOUS 2
#define LOG_DANGEROUS  3

/**** KEY scan codes ****/
#define KEY_F1   0x3B00U
#define KEY_F2   0x3C00U
#define KEY_F3   0x3D00U
#define KEY_F4   0x3E00U
#define KEY_F5   0x3F00U
#define KEY_F6   0x4000U
#define KEY_F7   0x4100U
#define KEY_F8   0x4200U
#define KEY_F9   0x4300U
#define KEY_F10  0x4400U

#define KEY_SF1   0x5400U
#define KEY_SF2   0x5500U
#define KEY_SF3   0x5600U
#define KEY_SF4   0x5700U
#define KEY_SF5   0x5800U
#define KEY_SF6   0x5900U
#define KEY_SF7   0x5A00U
#define KEY_SF8   0x5B00U
#define KEY_SF9   0x5C00U
#define KEY_SF10  0x5D00U

#define KEY_CF1   0x5E00U
#define KEY_CF2   0x5F00U
#define KEY_CF3   0x6000U
#define KEY_CF4   0x6100U
#define KEY_CF5   0x6200U
#define KEY_CF6   0x6300U
#define KEY_CF7   0x6400U
#define KEY_CF8   0x6500U
#define KEY_CF9   0x6600U
#define KEY_CF10  0x6700U

#define KEY_AF1   0x6800U
#define KEY_AF2   0x6900U
#define KEY_AF3   0x6A00U
#define KEY_AF4   0x6B00U
#define KEY_AF5   0x6C00U
#define KEY_AF6   0x6D00U
#define KEY_AF7   0x6E00U
#define KEY_AF8   0x6F00U
#define KEY_AF9   0x7000U
#define KEY_AF10  0x7100U

#define KEY_ALT1 0x7800U
#define KEY_ALT2 0x7900U
#define KEY_ALT3 0x7A00U
#define KEY_ALT4 0x7B00U
#define KEY_ALT5 0x7C00U
#define KEY_ALT6 0x7D00U
#define KEY_ALT7 0x7E00U
#define KEY_ALT8 0x7F00U
#define KEY_ALT9 0x8000U
#define KEY_ALT0 0x8100U

#define KEY_ALTA 0x1E00U
#define KEY_ALTB 0x3000U
#define KEY_ALTC 0x2E00U
#define KEY_ALTD 0x2000U
#define KEY_ALTE 0x1200U
#define KEY_ALTF 0x2100U
#define KEY_ALTG 0x2200U
#define KEY_ALTH 0x2300U
#define KEY_ALTI 0x1700U
#define KEY_ALTJ 0x2400U
#define KEY_ALTK 0x2500U
#define KEY_ALTL 0x2600U
#define KEY_ALTM 0x3200U
#define KEY_ALTN 0x3100U
#define KEY_ALTO 0x1800U
#define KEY_ALTP 0x1900U
#define KEY_ALTQ 0x1000U
#define KEY_ALTR 0x1300U
#define KEY_ALTS 0x1F00U
#define KEY_ALTT 0x1400U
#define KEY_ALTU 0x1600U
#define KEY_ALTV 0x2F00U
#define KEY_ALTW 0x1100U
#define KEY_ALTX 0x2D00U
#define KEY_ALTY 0x1500U
#define KEY_ALTZ 0x2C00U

#define KEY_CTLA 0x0001U
#define KEY_CTLB 0x0002U
#define KEY_CTLC 0x0003U
#define KEY_CTLD 0x0004U
#define KEY_CTLE 0x0005U
#define KEY_CTLF 0x0006U
#define KEY_CTLG 0x0007U
#define KEY_CTLH 0x0008U
#define KEY_CTLI 0x0009U
#define KEY_CTLJ 0x000AU
#define KEY_CTLK 0x000BU
#define KEY_CTLL 0x000CU
#define KEY_CTLM 0x000DU
#define KEY_CTLN 0x000EU
#define KEY_CTLO 0x000FU
#define KEY_CTLP 0x0010U
#define KEY_CTLQ 0x0011U
#define KEY_CTLR 0x0012U
#define KEY_CTLS 0x0013U
#define KEY_CTLT 0x0014U
#define KEY_CTLU 0x0015U
#define KEY_CTLV 0x0016U
#define KEY_CTLW 0x0017U
#define KEY_CTLX 0x0018U
#define KEY_CTLY 0x0019U
#define KEY_CTLZ 0x001AU

#define KEY_DEL  0x5300U
#define KEY_INS  0x5200U
#define KEY_HOME 0x4700U
#define KEY_END  0x4F00U

#define KEY_PGUP 0x4900U
#define KEY_PGDN 0x5100U
#define KEY_UP   0x4800U
#define KEY_DN   0x5000U
#define KEY_LT   0x4B00U
#define KEY_RT   0x4D00U

#define KEY_ESC  0x001BU
#define KEY_ENT  0x000DU
#define KEY_RET  0x000DU
#define KEY_TAB  0x0009U
#define KEY_STAB 0x0F00U
#define KEY_CPGUP 0x8400U
#define KEY_CPGDN 0x7600U


/** User flags *******************************************************/
#define UFLAG_DELETED    (0x00000001L) /*  0 // User deleted                */
#define UFLAG_ANSI       (0x00000002L) /*  1 // ANSI mode ON                */
#define UFLAG_PAUSE      (0x00000004L) /*  2 // Pause mode ON               */
#define UFLAG_CLEAR      (0x00000008L) /*  3 // Screenclear mode ON         */
#define UFLAG_HOTKEYS    (0x00000010L) /*  4 // Hotkeys?                    */
#define UFLAG_NOIBM      (0x00000020L) /*  5 // User has no IBM-graphics    */
#define UFLAG_FSED       (0x00000040L) /*  6 // Fullscreen editor used      */
#define UFLAG_NOKILL     (0x00000080L) /*  7 // Do not kill user            */
#define UFLAG_IGNORE     (0x00000100L) /*  8 // Ignore DL-hours             */
#define UFLAG_ATTEN      (0x00000200L) /*  9 // Attention flag              */
#define UFLAG_NOTOPS     (0x00000400L) /* 10 // Don't appear in tops        */
#define UFLAG_HIDDEN     (0x00000800L) /* 11 // Hide from lists             */
#define UFLAG_QUIET      (0x00001000L) /* 12 // Quiet??                     */
#define UFLAG_AVATAR     (0x00002000L) /* 13 // AVT/0 codes                 */
#define UFLAG_AVTPLUS    (0x00004000L) /* 14 // AVT/0+ codes                */
#define UFLAG_GUEST      (0x00008000L) /* 15 // Guest account               */
#define UFLAG_PAGEPRI    (0x00010000L) /* 16 // Page priority               */
#define UFLAG_LOCALONLY  (0x00020000L) /* 17 // Local login only            */
#define UFLAG_MULTILOGIN (0x00040000L) /* 18 // Allow multiple login        */
#define UFLAG_FREECHAT   (0x00080000L) /* 19 // Freeze timer in chat        */
#define UFLAG_NORIP      (0x00100000L) /* 20 // Disable RIP                 */

#define MSG_LOCAL   0
#define MSG_NET     1
#define MSG_ECHO    2
#define MSG_PVTECHO 3

#define MSG_BOTH  0
#define MSG_PVT   1
#define MSG_PUB   2
#define MSG_TOALL 3

#define MSGATTR_PRIVATE     (0x00000001L)
#define MSGATTR_RECEIVED    (0x00000002L)
#define MSGATTR_DELETED     (0x00000004L)
#define MSGATTR_NETMAIL     (0x00000008L)
#define MSGATTR_UNSENT_ECHO (0x00000010L)
#define MSGATTR_UNSENT_NET  (0x00000020L)
#define MSGATTR_LOCAL       (0x00000040L)
#define MSGATTR_KILL        (0x00000080L)
#define MSGATTR_CRASH       (0x00000100L)
#define MSGATTR_SENT        (0x00000200L)
#define MSGATTR_FILE        (0x00000400L)
#define MSGATTR_REQ         (0x00000800L)
#define MSGATTR_AUDIT       (0x00001000L)
#define MSGATTR_RET         (0x00002000L)


/**** NOTE *******************************************************************/
/*                                                                           */
/*  Dates are stored as an array of 3 bytes, the first byte is the day       */
/*  portion, the second byte is the month portion, and the third byte is the */
/*  year portion (00-99). Times are stored in a similar way: the first byte  */
/*  is the hour portion (00-24), the second byte is the minute portion       */
/*  (00-59), and the third byte is the seconds portion (00-59).              */
/*                                                                           */
/*****************************************************************************/

typedef struct
   {
    long         record;                /* Record number in USERS.BBS           */

    char         name[36];              /* User name                            */
    char         alias[36];             /* Alias/Handle                         */
    char         passWord[16];          /* Password                             */
    dword        passWordCRC;
    word         level;                 /* Security level                       */
    char         country[26];           /* Country                              */
    char         state[26];             /* State                                */
    char         city[26];              /* City                                 */
    char         company[51];           /* Company                              */

    char         address1[51];          /* Address line 1                       */
    char         address2[51];          /* Address line 2                       */
    char         address3[51];          /* Address line 3                       */

    char         comment[81];           /* Comment                              */
    char         forwardTo[36];         /* Forward mail to                      */

    DateType     birthDate;             /* Birthday                             */

    char         voicePhone[16];        /* Voice phone #                        */
    char         dataPhone [16];        /* Data phone #                         */
    char         faxPhone  [16];        /* Fax phone #                          */

    byte         sex;                   /* 0 = Unknown, 1 = Male, 2 = Female    */
    byte         dateFormat;            /* Date Format                          */
    byte         defaultProtocol;       /* Default protocol hotkey              */
    char         language[9];           /* Language                             */
    word         screenWidth;           /* Screen Width                         */
    word         screenLength;          /* # of lines                           */

    long         timeUsed;              /* time used today                      */

    dword        timesCalled;           /* # times called                       */
    dword        numDownloads;          /* # downloads                          */
    dword        kbDownloaded;          /* K downloaded                         */
    dword        numUploads;            /* # downloads                          */
    dword        kbUploaded;            /* K downloaded                         */
    dword        msgsPosted;            /* # messages posted                    */

    DateType     lastDate;              /* Date last called                     */
    DateType     lastTime;              /* Time last called                     */
    DateType     lastPwdChange;         /* Date of last password change         */
    DateType     lastNewFilesCheck;     /* Date of last check for new files     */

    accessflags  aFlags;                /* Access flags       (A-Z,1-6)         */
    dword        uFlags;                /* Flags (DELETED/ANSI/PAUSE/...)       */
    word         logLevel;              /* Log level of user                    */

    byte         mailCheckBoards[125];  /* Msg areas to check for mail          */
    byte         combinedBoards[125];   /* Areas to be used in combined mode    */

    dword        totalTimeUsed;         /* Total time used (minutes)            */

    DateType     expDate;               /* Expiration date                      */
    word         expLevel;              /* Fallback level after expiration      */
    accessflags  expFlagsOn;            /* Flags to be enabled after expiration */
    accessflags  expFlagsOff;           /* Flags to be disabled after expiration*/

    DateType     firstDate;             /* Date of first login                  */
    long         kbToday;               /* K downloaded today                   */
    long         credit;                /* Netmail credit                       */
    long         pending;               /* Netmail credit pending               */
    word         fileArea;              /* Last file area accessed              */
    word         msgArea;               /* Last msg area accessed               */

    int          tbTimeBalance;         /* Time balance           (Time Bank)   */
    int          tbKbBalance;           /* Kbyte balance          (Time Bank)   */
    int          tbTimeWithdrawn;       /* Time withdrawn today   (Time Bank)   */
    int          tbKbWithdrawn;         /* Kbytes withdrawn today (Time Bank)   */
    word         tbTimeDeposited;
    word         tbKbDeposited;
    word         tbTimeLoaned;
    word         tbKbLoaned;
    DateType     tbLastUsed;            /* Date last used         (Time Bank)   */

    byte         checkMail;             /* Check for mail at logon              */
    byte         checkNewFiles;         /* Check for new files at logon         */

    dword        highMsgRead;

    word         qwkMaxMsgsPerArea;
    word         qwkMaxMsgs;
    byte         qwkArchiver;
    byte         ripFont;

    DateType     tbTimePayback;
    DateType     tbKbPayback;

    word         fileGroup;
    word         msgGroup;

    byte         extra[390];
  } USER_REC;



typedef struct
  {
   word     level;            /* User Level                            */
   int      timelimit;        /* Time Limit                            */
   int      daily_klimit;     /* Daily download limit                  */
   int      pre_download;     /* Pre-download time limit               */
   char     id[6];            /* Usergroup ID                          */
   unsigned free;             /* Free upload in Kb.                    */
   byte     factor;           /* Percentage upload required            */
   unsigned max_download;     /* Max download for this level           */
   word     fallto;           /* Fall to level x when max. reached     */
   int      msgfactor;        /* # Kbytes granted per message written  */
   char extra[5];
 } LIMIT;

typedef struct
   {
      word        areaNum;
      word        hudsonBase;    /* Number of Hudson message base           */
      char        name[81];      /* Name of message areas                   */
      byte        msgType;       /* Kind of message area (Net/Echo/Local)   */
      byte        msgKind;       /* Type of message (Pvt only/Pub only/...) */
      byte        msgBaseType;   /* Type of message base                    */
      char        path[81];      /* Path to Squish or *.MSG                 */
      byte        flags;         /* Alias allowed/forced/prohibited         */
      word        readLevel;     /* Minimum level needed to read msgs       */
      accessflags readFlags;     /* flags needed to read msgs               */
      accessflags readFlagsNot;  /* flags non-grata to read msgs            */
      word        writeLevel;    /* Minimum level needed to write msgs      */
      accessflags writeFlags;    /* flags needed to write msgs              */
      accessflags writeFlagsNot; /* flags non-grata to read msgs            */
      word        sysopLevel;    /* Minimum level needed to change msgs     */
      accessflags sysopFlags;    /* flags needed to change msgs             */
      accessflags sysopFlagsNot; /* flags non-grata to read msgs            */

      char        origin[62];    /* Origin line                             */
      int         aka;           /* AKA                                     */

      word        rcvKillDays;
      word        msgKillDays;
      word        maxMsgs;

      char        sysop[36];     /* Area Sysop                              */
      int         replyBoard;    /* Area number where replies should go     */

      char        echoTag[61];   /* Echomail Tag Name                       */
      char        qwkTag[13];    /* QWK Area Name                           */

      byte        groups[4];     /* Groups belonging to                     */
      bool        allGroups;     /* Belongs to all groups                   */
      byte        minAge;        /* Minimum age for this area               */

      byte        extra[112];
   } MSGAREA;

typedef struct
   {
      char        name[80];             /* Name of file area                           */
      char        listpath[80];         /* Path for file-list                          */
      char        filepath[80];         /* Path for files                              */
      word        level;                /* Level needed to access file area            */
      accessflags flags;                /* Flags needed to access file area            */
      bool        copyLocal;            /* TRUE = CDROM File listing type              */
      int         maxfiles;             /* Max files per day in this area downloadable */
      int         maxkb;                /* Max Kbytes per day in this area             */
      bool        notops;               /* Set to TRUE if not to appear in TOPFILES.A* */
      bool        free;                 /* If TRUE, this area is free                  */
      byte        groups[4];            /* Groups belonging to                         */
      bool        allGroups;            /* Belongs to all groups                       */
      byte        minAge;               /* Minimum age                                 */
      long        flagsNot;             /* Access flags                                */

      char        extra[3];
   } FILEAREA;

typedef struct
   {
      long        num;
      long        id;

      char        from[36];
      char        to  [36];
      char        subj[66];

      dword       attr;

      DateType    postDate;
      TimeType    postTime;
      DateType    recvDate;
      TimeType    recvTime;

      long        next;
      long        prev;

      int         origZone,
                  origNet,
                  origNode,
                  origPoint;

      int         destZone,
                  destNet,
                  destNode,
                  destPoint;

      int   cost;

      int   area;

      int   reserved2;
   } MESSAGE;

typedef struct
   {
      short zone;
      short net;
      short node;
      short point;
   } FIDO_NODE;

typedef struct
   {
      char      shellmsg[81];         /* Message to show when shelling               */
      char      sysopname[36];        /* Name of sysop                               */
      char      txtpath[61];          /* Path for textfiles                          */
      char      mnupath[61];          /* Path for menu-files                         */
      char      msgpath[61];          /* Path for message base                       */
      char      uploadpath[61];       /* Uploadpath                                  */
      char      editorname[61];       /* Name of external editor                     */
      word      newuserlevel;         /* Level for new user                          */
      word      newuserloglevel;      /* Loglevel for new user                       */
      long      newuserflags;         /* New user flags                              */
      int       max_passinput;        /* Maximum attempts for password entry         */
      int       min_passlength;       /* Minimum password length                     */
      int       inactivity_time;      /* Inactivity time-out limit                   */
      int       max_sysop_pages;      /* Maximum times sysop can be paged            */
      int       pagebell_length;      /* Length of page-bell (secs)                  */
      int       mailcheck;            /* Check for mail at logon?                    */
      int       europe;               /* European date format?                       */
      int       numnodes;             /* # nodes                                     */
      int       allowansi;            /* Allow ANSI?                                 */
      int       discrete;             /* Hide sysop activity?                        */
      int       askphone;             /* Ask for phone number?                       */
      int       allowoneword;         /* Allow one-word names                        */
      word      crashlevel;           /* Level needed for crashmail                  */
      long      crashflags;           /* Flags needed for crashmail                  */
      word      attachlevel;          /* Level needed for file attach                */
      long      attachflags;          /* Flags needed for file attach                */
      int       allowmsgupload;       /* Allow message uploads                       */
      int       allowstacking;        /* Allow command stacking                      */
      byte      spare1[6];            /* not used                                    */
      int       handshaking;          /* I/O Handshaking                             */
      int       allowalias;           /* Allow alias for login                       */
      int       loglocal;             /* Log local calls                             */
      int       doswap;               /* Allow swapping                              */
      char      originline[61];       /* Origin line                                 */
      char      nodelistdir[61];      /* Nodelist directory                          */
      char      sysopkeys[10][60];    /* Sysop hotkeys                               */
      byte      spare2[6];            /* not used                                    */
      int       uploadspace;          /* Space needed for uploads                    */
      char      pvtuploadpath[61];    /* Directory for files uploads                 */
      char      quotestring[6];       /* String used for quoting                     */
      int       fastmode;             /* Use fast mode (=needs more memory)          */
      int       killsent;             /* Kill netmail after sent                     */
      bool      egamode;              /* Use 43/50 line mode                         */
      bool      showuserinfo;         /* Show user info while in EGA mode?           */
      char      pexpath[61];          /* Directory for PEX-files                     */
      int       allowquicklogin;      /* Allow quick sysop login?                    */
      int       securityboard;        /* MsgBoard for security messages              */
      int       pwdmessages;          /* Write security-messages?                    */
      char      bbsname[36];          /* Name of the BBS                             */
      char      pwdchar;              /* Password character                          */
      int       tb_maxtimedeposit;
      int       tb_maxkbdeposit;
      int       tb_maxtimewithdrawal;
      int       tb_maxkbwithdrawal;
      int       usage_days;           /* Days to keep usage graphs                   */
      char      systempwd[16];        /* System password                             */
      bool      usesystempwd;         /* Use system password?                        */
      bool      askbirthdate;         /* Ask Birth Date?                             */
      int       binlogdays;           /* # days to log in BINLOG.PB                  */
      bool      binloglocal;          /* Log local calls to BINLOG.PB yes/no         */
      int       pageArea;             /* Area number for page messages               */
      bool      indexfiles;           /* Indexed FILES.BBS (obsolete)                */
      bool      checkdupes;           /* Check duplicate uploads                     */
      bool      killdupes;            /* Kill duplicate uploads                      */
      bool      ignore_ext;           /* Ignore file extension for dupe-search       */
      char      RIPpath[61];          /* Path for RIP scripts                        */
      char      iconspath[61];        /* Path for RIP icons                          */
      char      location[36];         /* BBS Location (City)                         */
      char      phone[26];            /* BBS Phone Number                            */
      char      QWKid[9];             /* Base file name for QWK packets (BBSID)      */
      word      IObuffersize;         /* I/O buffer size in bytes                    */
      TimeFrame pagingHours;          /* Paging Hours                                */
      char      defaultLanguage[9];   /* Default language                            */
      bool      addUploaderName;      /* Add uploader's name to FILES.BBS            */
      TimeFrame downloadHours;        /* Download Hours                              */
      bool      askdataphone;         /* Ask for data phone #                        */
      bool      askfaxphone;          /* Ask for fax phone #                         */
      bool      askaddress;           /* Ask for mailing address                     */
      bool      asksex;               /* Ask sex                                     */
      bool      askdateformat;        /* Ask date format                             */
      bool      askstate;             /* Ask state                                   */
      bool      askcountry;           /* Ask country                                 */
      word      fuzzyRate;            /* Fuzzy search rate for user editor (0-100)   */
      bool      hidePassword;         /* Hide password in user editor                */
      bool      valConfirm;           /* Confirm validation in user editor           */

      byte      spare3[17];

      byte      extra[255];
  } CONFIG;

typedef struct
   {
      DateType date;
      TimeType timeIn;
      TimeType timeOut;
      char     name[36];
      char     city[26];
      char     country[26];
      long     baud;
      word     node;
      long     kbDown;
      long     kbUp;
      word     yells;
      word     level;
      dword    uflags;
      char     alias[36];
      byte     extra[45];
   } BINLOG;

typedef struct
   {
      char name[13];
      word area;
   } TAGGED_FILE;

/***** Long math substitution functions *****/

long l_mul(long val1,long val2);
long l_div(long val1,long val2);
long l_mod(long val1,long val2);
long l_shl(long val,unsigned s);
long l_shr(long val,unsigned s);

dword ul_div(dword val1,dword val2);
dword ul_mod(dword val1,dword val2);
dword ul_shl(dword val,word s);
dword ul_shr(dword val,word s);

#ifdef __cplusplus

 inline bool GetFlag(dword flags,int flag)
    {
     return ((flags & ul_shl(1,32-flag)) ? TRUE:FALSE );
    }

 inline void SetFlag(dword &flags,int flag)
    {
     flags |= ul_shl(1,32-flag);
    }

 inline void ClearFlag(dword &flags,int flag)
    {
     flags &= ~ul_shl(1,32-flag);
    }

 inline void InitMessage(MESSAGE *msg)
    {
     memset(msg,0,sizeof(MESSAGE));
    }

#else

 #define GetFlag(flags,flag)   (((flags) &   ul_shl(1,32-(flag)))?TRUE:FALSE)
 #define SetFlag(flags,flag)   { (flags) |=  ul_shl(1,32-(flag)); }
 #define ClearFlag(flags,flag) { (flags) &= ~ul_shl(1,32-(flag)); }

#endif

/* You can use these constants to manipulate user flags (by using the */
/* functions/macros GetFlag, SetFlag and ClearFlag                    */

#define FLAG_A 1
#define FLAG_B 2
#define FLAG_C 3
#define FLAG_D 4
#define FLAG_E 5
#define FLAG_F 6
#define FLAG_G 7
#define FLAG_H 8
#define FLAG_I 9
#define FLAG_J 10
#define FLAG_K 11
#define FLAG_L 12
#define FLAG_M 13
#define FLAG_N 14
#define FLAG_O 15
#define FLAG_P 16
#define FLAG_Q 17
#define FLAG_R 18
#define FLAG_S 19
#define FLAG_T 20
#define FLAG_U 21
#define FLAG_V 22
#define FLAG_W 23
#define FLAG_X 24
#define FLAG_Y 25
#define FLAG_Z 26
#define FLAG_1 27
#define FLAG_2 28
#define FLAG_3 29
#define FLAG_4 30
#define FLAG_5 31
#define FLAG_6 32

/*****************************************************************************/
/****  Global ProBoard variables  ********************************************/
/*****************************************************************************/

extern USER_REC     * const CurUser;      /* Current user online        (R/W) */
extern int            const UserRecNr;    /* Record # of current user   (R/O) */
extern int            const NumLimits;    /* # limits in limits[] array (R/O) */
extern LIMIT  const * const Limits;       /* Limits per level           (R/O) */
extern char         * const LoginDate;    /* Login date of user         (R/W) */
extern char         * const LoginTime;    /* Login time of user         (R/W) */
extern bool           const NetEntered;   /* Netmail entered            (R/O) */
extern bool           const EchoEntered;  /* Echomail entered           (R/O) */
extern int            const NumUsers;     /* # users in userfile        (R/O) */
extern int            const NodeNumber;   /* Current node number        (R/O) */
extern char   const * const CurMenu;      /* Current menu name          (R/O) */
extern char   const * const UserFirstName;/* First name of current user (R/O) */
extern char   const * const PrevUser;     /* Name of previous user      (R/O) */
extern char   const * const StartupPath;  /* Startup path               (R/O) */
extern char   const * const SysPath;      /* ProBoard system path       (R/O) */
extern CONFIG const * const Config;       /* ProBoard config-record     (R/O) */
extern word           const PBVersion;    /* Version number of ProBoard (R/O) */
extern long           const BaudRate;     /* Baud rate (0=local)        (R/O) */
extern word           const Beta;         /* Beta nr (0xFFFF=release)   (R/O) */
extern char         * const PageReason;   /* Reason for sysop paging    (R/W) */
extern word         * const PageCount;    /* Number of sysop pages      (R/W) */

char *form(char *, ...);                         /* From old C++ streams lib */

/*****************************************************************************/
/****  Message functions  ****************************************************/
/*****************************************************************************/

void MsgEd(void);
int  PostMessage(const char *from,const char *to,const char *subject,int area,bool pvt);
int  PostNetmail(const char *from,const char *to,const char *subject,int area,const FIDO_NODE *address,bool attach,bool crash,bool kill);
bool ReadMsgArea(int area,MSGAREA *ma);
int  NumMsgAreas(void);
long GetLastRead(int areanum , long user_recno);
void SetLastRead(int areanum , long user_recno , long msgid);
long NumMsgs(int areanum);
long HighMsg(int areanum);
long MsgNum(int areanum , long id);
long MsgId (int areanum , long num);
bool ReadMessage(MESSAGE *msg,long msgid,int areanum);
void WriteMSGTMP(const char *text);
void AppendMSGTMP(const char *text);
void ShowMessage(const MESSAGE *msg);
void CreateMessageText(const MESSAGE *msg);
void CreateMessageTextString(const MESSAGE *msg,char *text,int maxsize);
bool FirstMessage(MESSAGE *msg,int area,int order,long first);
bool NextMessage(MESSAGE *msg,int area,int order);
void DeleteMessage(MESSAGE *msg);
void MarkMessage(int area,long msgid);
void ReadMarkedMessages(void);
void ListMarkedMessages(void);
void UnMarkAllMessages(void);


/*****************************************************************************/
/****  Time functions  *******************************************************/
/*****************************************************************************/

void AddTime(int plusminus);
int  TimeLeft(void);
int  TimeOnline(void);
void SuspendTimer(void);
void RestartTimer(void);
void AdjustTime(void);
int  TimeUntilEvent(void);


/*****************************************************************************/
/****  Low-level I/O functions  **********************************************/
/*****************************************************************************/

bool IO_SendByte(byte);                   /* TRUE = byte sent, FALSE = buffer full */
int  IO_ReadByte(void);                   /* Returns -1 if no byte available       */
bool IO_Carrier(void);                    /* TRUE = carrier available              */
bool IO_ByteReady(void);                  /* TRUE = byte available in input buffer */
bool IO_BufEmpty(void);                   /* TRUE = output buffer is empty         */
void IO_StartBreak(void);                 /* Start sending a break                 */
void IO_StopBreak(void);                  /* Stop sending a break                  */
void IO_DTR(bool);                        /* Set/lower DTR line (TRUE = set)       */
word IO_SendBlock(byte *buf,word size);   /* Send block of bytes, returns # bytes actually sent */
word IO_ReadBlock(byte *buf,word size);   /* Read block of bytes, returns # bytes actually read */

/*****************************************************************************/
/****  I/O functions  ********************************************************/
/*****************************************************************************/
/* Note : some I/O is done through the standard library functions printf(),
 *        puts(), putchar(), getchar(), gets() and vprintf()
 *****************************************************************************/

char WaitKey(void);
char WaitKeys(const char *);
void Input(char *buf,int len,int readmode);
bool Ask(bool def);
char PeekChar(void);
void SetColor(char color);
void SetFullColor(byte color);
void GotoXY(int x,int y);
void ClrEol(void);
void EnableStop(void);
void DisableStop(void);
bool Stopped(void);
bool ExternalInput(void);
char ShowHotkeyFile(const char *fname,const char *hotkeys);
char ShowHotkeyANSIFile(const char *fname,const char *hotkeys);
bool ShowRIPscrip(char *fn);
void InitLineCounter(void);
bool LineCounter(void);
void ResetInactivity(void);
bool LocalDisplay( bool );
bool RemoteDisplay( bool );
bool RIP(void);
void ParseStringVars(char *input,char *result,int max_len);

int  FuzzySearch(char *text,char *search,int degree);

bool ReadUser(USER_REC *rec,long recnr);
void WriteUser(const USER_REC *rec);
bool SetUser(long recnr);

char PlayMusic(const char *fname,const char *hotkeys);
void PostInfo(const char *fname);
int  ReadFileArea(int area,FILEAREA *ma);
int  NumFileAreas(void);
long MemAvail(void);
void MenuFunction(int,const char *);
void Log(int loglevel,const char *fmtstring,...);
void HangUp(void);
bool CheckAccess(word level,accessflags flags);
KEY  ScanKey(void);
void exit(void);
void ExitTSR(void);
int  ErrorLevel(void);

int  GetTaggedFiles(TAGGED_FILE *array,int max);
bool PutTaggedFiles(TAGGED_FILE *array,int n);
bool AddTaggedFile(TAGGED_FILE *tag);
bool RemoveTaggedFile(TAGGED_FILE *tag);
void ClearTaggedFiles();
bool IsTagged(TAGGED_FILE *tag);

bool GetIniVar(char *fname,char *varname,char *value,int max);
bool SetIniVar(char *fname,char *varname,char *value);

int  _InstallHandler(int handler,int (*func)(void),unsigned ds);
void _RemoveHandler(int handler,int (*func)(void),unsigned ds);

#define InstallHandler(handler,func) _InstallHandler(handler,(int (*)(void))(func),FP_SEG(&NetEntered));
#define RemoveHandler(handler,func)  _RemoveHandler(handler,(int (*)(void))(func),FP_SEG(&NetEntered));

#ifdef __cplusplus
 }

inline void * operator new   (unsigned size)    { return malloc(size); }
inline void   operator delete(void *p)          { free(p); }

#endif
#endif
