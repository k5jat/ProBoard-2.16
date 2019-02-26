#define MSG_BOTH 0
#define MSG_PVT  1
#define MSG_PUB  2

#define MSG_LOCAL   0
#define MSG_NET     1
#define MSG_ECHO    2
#define MSG_PVTECHO 3

struct PB_FileArea
  {
  char name[80];       // Name of file area
  char listpath[80];   // Path for file-list
  char filepath[80];   // Path for files
  int level;           // Level needed
  long flags;          // Flags Needed
  byte cdrom;          // 1 = CDROM File listing type
  int maxfiles;        // Max files per day in this area downloadable
  int maxkb;           // Max Kbytes per day in this area
  byte extra[15];

  int read(int num);
  };


/*
#define UFLAG_DELETED  1    //  0   // User deleted
#define UFLAG_ANSI     2    //  1   // ANSI mode ON
#define UFLAG_PAUSE    4    //  2   // Pause mode ON
#define UFLAG_CLEAR    8    //  3   // Screenclear mode ON
#define UFLAG_STACKING 16   //  4   // Command stacking?
#define UFLAG_NOIBM    32   //  5   // User has no IBM-graphics
#define UFLAG_FSED     64   //  6   // Fullscreen editor used
#define UFLAG_NOKILL   128  //  7   // Do not kill user

#define UFLAG_IGNORE   256  //  8   // Ignore DL-hours
#define UFLAG_ATTEN    512  //  9   // Attention flag
#define UFLAG_NOTOPS  1024  // 10   // Don't appear in tops
*/

struct PB_TIMELOG
  {
   int numdays;
   Date lastdate;
   long hours[24];
   long days[7];
   long weeks[53];
   long totalcalls;
  };
