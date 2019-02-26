#ifndef TOOLS_HPP
#define TOOLS_HPP



#ifndef NULL
 #if sizeof(void *)==4
  #define NULL 0L
 #else
  #define NULL 0
 #endif
#endif



#ifndef NULLFUNC
 #if sizeof(void (*)())==4
  #define NULLFUNC 0L
 #else
  #define NULLFUNC 0
 #endif
#endif



#ifndef PB_SDK
  typedef unsigned char  byte;
  typedef unsigned short word;
  typedef          short sword;
  typedef unsigned long  dword;
  typedef          long  sdword;
  typedef unsigned char  bool;
#endif



#ifdef __OS2__
   #define INCL_DOSFILEMGR
   #include <os2.h>

   #define _near
   #define near
   #define _far
   #define far
#endif



#undef TRUE
#undef FALSE



//------------------------------------------------------------------
// This is the maximum size of a date field that should be presented
// to the user.  They might enter 8-bytes of data (i.e., a short
// year), or a 10-byte value (i.e., a fully-qualified year).
//------------------------------------------------------------------

#define MAX_DATE_FIELD_SIZE 10



//--------------------------------------------------------------------------
// This macro defines the year value where the decision is made to consider
// the year in the 1900's, or the 2000's when the user enters a two-digit
// year.  Since InputDate() is only being used for two purposes in ProBoard,
// this works well.  It's only being used for the following:
//
//        - To enter the user's birthdate
//        - To enter a date for a new file listing
//
// We'll start off by assigning the crossover date to 10.  This means that
// any date entered as 00-09 will be interpretted as 2000-2009.  Any date
// from 10-99 will be interpretted as 1910 through 1999.  This will
// accommodate birthdates for the most aged of people, leaving the
// possibility of a screwup very small.  This will also allow users the
// ability to write the year out in shorthand for file listings after the
// year 2000 hits us.
//--------------------------------------------------------------------------

#define TWO_DIGIT_CROSSOVER 10



class String;

const bool TRUE  = 1;
const bool FALSE = 0;

#ifndef MAKELONG
   #define MAKELONG(a, b)      dword(word(a) | (dword(word(b)) << 16))
#endif

#ifndef LOWORD
   #define LOWORD(l)           word(l)
#endif

#ifndef HIWORD
   #define HIWORD(l)           word((dword(l) >> 16) & 0xFFFF)
#endif

#ifndef LOBYTE
   #define LOBYTE(w)           byte(w)
#endif

#ifndef HIBYTE
   #define HIBYTE(w)           byte((word(w) >> 8) & 0xFF)
#endif

inline bool
TESTBIT(word val,int bit)
{
   return (bool)!!(val & (1<<bit));
}

inline bool
TESTBIT(int val,int bit)
{
   return (bool)!!(val & (1<<bit));
}

inline bool
TESTBIT(dword val,int bit)
{
   return (bool)!!(val & (1L<<bit));
}

inline bool
TESTBIT(long val,int bit)
{
   return (bool)!!(val & (1L<<bit));
}

inline void
SETBIT(word& val,int bit)
{
   val |= word(1<<bit);
}

inline void
SETBIT(int& val,int bit)
{
   val |= int(1<<bit);
}

inline void
SETBIT(dword& val,int bit)
{
   val |= dword(1L<<bit);
}

inline void
SETBIT(long& val,int bit)
{
   val |= long(1L<<bit);
}

inline void
CLEARBIT(word& val,int bit)
{
   val &= word(~(1<<bit));
}

inline void
CLEARBIT(int& val,int bit)
{
   val &= ~(1<<bit);
}

inline void
CLEARBIT(dword& val,int bit)
{
   val &= ~(1L<<bit);
}

inline void
CLEARBIT(long& val,int bit)
{
   val &= ~(1L<<bit);
}

#ifndef max
   #define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
   #define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#define CLEAR_OBJECT(obj) memset(&(obj),0,sizeof(obj))

/* ----- Miscellaneous functions ------ */

/*** 16 bit CRC functions ***/

word crc(const char *);
word crc(const void *,unsigned);
word upcrc(const char *);

/*** 32 bit CRC functions ***/

dword crc32   (const char *);
dword upcrc32 (const char *);
dword crc32   (const void *,unsigned);

void  sound(word freq);
void  stopsound();
void  soundclick();

extern "C"
   {
      /*** Constants for File::seek() ***/

      const seek_set           = 0;
      const seek_cur           = 1;
      const seek_end           = 2;
   }

#ifndef PB_SDK
  bool get_ini_var(const char *fname,const char *varname,char *var,int max);
  bool set_ini_var(const char *fname,const char *varname,const char *var);
#endif

bool   ts_MakeFullDir(char *dir);                      /* Creates directory even in multiple levels       */
bool   ts_DirExists(char *dir);                        /* Check if directory exists                       */
long   ts_CopyFile(char *from,char *to,int bufsize);   /* Copy file using buffer size                     */
String ts_ResolveDir(char *dir);                       /* Resolve directory (strip .. and other elements) */
bool   ts_IsFullPath(char *s);                         /* Check if path consists of drive + directory     */
void   ts_MakeInputPath(String& s);                    /* Make path suitable for editing                  */
void   ts_MakeInputPath(char *s);                      /* Make path suitable for editing                  */
bool   ts_SyncFileDate(char *fnSource,char *fn);       /* Sync file date of 'fn' with 'fnSource'          */


//-------------------------------------  
// Added in ProBoard v2.2 - JEFF REEDER
//-------------------------------------  

int NormalizeYear( int iYearVal );


//--------------------------------------------------------------------------


void append_backspace(char *);
void strip_linefeed(char *);
void strip_trailing(char *);
void strip_leading(char *);
void strip_all(char *);

void c2pas(char *);
void pas2c(char *);

#ifdef __OS2__
   #define sleep_clock()
#else
   void sleep_clock();
#endif

#ifndef PB_SDK
  #define strcmpl(a,b) stricmp(a,b)
  #define msleep(a)    delay(a)
#endif

#if defined(__DOS_H)
inline long
dos_getdiskfreespace(int drive)
{
   struct dfree fr;

   getdfree(drive, &fr);

   if(fr.df_sclus == (unsigned)-1)
      return 0L;

   return  long(fr.df_avail)
         * long(fr.df_bsec)
         * long(fr.df_sclus);
}
#endif

#ifndef __OS2__
inline dword
clockticks()
{
   return *((dword _far *)0x46C);
}

inline dword
clockdiff(dword prev)
{
   return clockticks() - prev;
}
#endif

class String
   {
   public:
      enum justification { left , right , centered };

   private:
      void adjustSize();
      void adjustSize(int l);

      int size;

   protected:
      char *s;

   public:
      String();
      String(const String&);
      String(const char *);
      String(const char,int len=1);
     ~String();
      void  clear();
      char& operator[](int i)
         {
            return s[i];
         }
      operator char*() const
         {
            return s;
         }

      void operator=(const char*);
      void operator=(const String&);
      void operator=(char);
      void operator=(int);
      void operator=(long);
      void operator+=(const String&);
      void operator+=(const char *);
      void operator+=(char);

      int operator==(const char *str) const;
      int operator==(const String& str) const;
      int operator!=(const char *str) const;
      int operator!=(const String& str) const;

      String& operator<<(const String&);
      String& operator<<(const char *);
      String& operator<<(char);
      String& operator<<(int);
      String& operator<<(long);

      int replace(const char *s,const char *r,bool = FALSE,int max_count = 0);
      int find(const char *s,bool = FALSE) const;   // returns pos (<0 -> not found)

      int fuzzySearch(const char *pattern , int degree = 75); // <0 -> not found

      String sub(int pos,int l) const;

      void del(int pos,int len);              // pos>=0
      void insert(int pos,const char *str);   // pos>=0

      char lastChar() const;
      char firstChar() const { return *s; }

      void delFirst(char = '\0');
      void delLast(char = '\0');

      int len() const;
      word crc() const;
      word upcrc() const;

      void upperCase();
      void lowerCase();
      void mixedCase();

      void ltrim();
      void rtrim();
      void  trim();

      friend String operator+(const String&,const String&);
      friend String operator+(const String&,const char * );
      friend String operator+(const char * ,const String&);

      void justify(justification , int len = 0);
   };


/*** Date class ***/

#define TODAY (-1L)
#define NOW   0


//------------------------------------------------------------------------  
// These are the constant index values for accessing parts of a date field
//------------------------------------------------------------------------  

#define DATE_DAY   0
#define DATE_MONTH 1
#define DATE_YEAR  2



class Date
   {
      char _d ,
           _m ,
           _y ;

      int compare(const Date&) const;
      void  parse_key(String&) const;

      static char *weekdays[7];
      static char *s_weekdays[7];
      static char *l_weekdays[7];
      static char *months[12];
      static char *l_months[12];

   public:
      Date();                   // Initializes date to nothing
      Date(long);               // Initializes date to julian/today
      Date(int,int,int);        // Initializes date with specific values
      Date(const Date&);

      void operator=( const Date& );

      void operator()(int day,int month,int year) // Equal to set()
         {
            set(day,month,year);
         }

      char& operator[](int i);  // 0 = day , 1 = month , 2 = year

      int day() const           // Returns day (1-31)
         {
            return _d;
         }
      int month() const         // Returns month (1-12)
         {
            return _m;
         }
      int year() const          // Returns year (0-127)
         {
            return _y;
         }

      bool ok() const;           // Returns !=0 if date is valid

      int dayNum() const;       // Returns the number of the day in the year (1-366)
      int weekDay() const;      // Returns the day of week (0=sunday)
      int weekNum() const;      // Returns the week number (1-53)
      int daysInMonth(int = 0) const; // Returns # days in given month
      int daysInYear(int = 0) const;  // Returns # days in given year
      int weeksInYear(int = 0) const; // Returns # weeks in given year

      int age(const Date& birthdate);

      operator long() const;    // Converts to julian date
      Date& operator=(long);    // Converts julian date to normal date

      void operator++()
         {
            (*this) = long(*this) + 1;
         }
      void operator++(int)
         {
            operator++();
         }
      void operator--()
         {
            (*this) = long(*this) - 1;
         }
      void operator--(int)
         {
            operator--();
         }
      void operator+=(int n)
         {
            (*this) = long(*this) + n;
         }
      void operator-=(int n)
         {
            (*this) = long(*this) - n;
         }

      void set(int,int,int);    // Set date to a specific date
      void today();             // Set date to today

      int operator<(const Date&) const;     //  ]
      int operator>(const Date&) const;     //  ]
      int operator==(const Date&) const;    //  ] Compare
      int operator!=(const Date&) const;    //  ]      functions
      int operator<=(const Date&) const;    //  ]
      int operator>=(const Date&) const;    //  ]

      int operator-(const Date&) const;     // Returns difference in number of days

      bool leap()               const;

      String format(const char *fmt) const;

      void fileDate(word);  // Sets date to packed date
   };


/*** Time class ***/

class Time
   {
      char _h ,
           _m ,
           _s ;

      void Time::parse_key(String& key) const;
   public:

      Time();
      Time(int)
         {
            now();
         }
      Time(int h,int m,int s)
         {
            set(h,m,s);
         }

      Time(const Time&);

      void operator=(const Time&);

      void operator()(int h,int m,int s)
         {
            set(h,m,s);
         }

      char& operator[](int i);

      int hour() const
         {
            return _h;
         }
      int minute() const
         {
            return _m;
         }
      int seconds() const
         {
            return _s;
         }

      operator long() const;

      long operator-(const Time& t) const;

      void operator-=(const Time&);
      void operator+=(const Time&);
      void operator=(long);

      void set(int h,int m,int s);     // Set to specific time
      void now();                      // Set to right now

      String format(const char *) const;

      void fileTime(word);

      int operator<(const Time&)  const;   // ]
      int operator>(const Time&)  const;   // ]
      int operator==(const Time&) const;   // ]  Compare
      int operator!=(const Time&) const;   // ]      functions
      int operator<=(const Time&) const;   // ]
      int operator>=(const Time&) const;   // ]
   };


#undef rewind

class FileName : public String
  {
 public:
   FileName() : String() {}
   FileName(const char *p,const char *f=NULL,const char *e=NULL);
   void operator()(const char *p);
   void operator()(const char *p,const char *f);
   void operator()(const char *p,const char *f,const char *e);
   void appendBS();
   void stripPath();
   void stripExt();
   void stripName();
   void changeExt(const char *ext);
   void changePath(const char *path);
   void operator=(const char*);
   void operator=(const FileName&);
   void operator=(char);
   void operator=(int);
   void operator=(long);
   void fit(int size);
  };

const DIR_NORMAL = 0x00;
const DIR_RDONLY = 0x01;
const DIR_HIDDEN = 0x02;
const DIR_SYSTEM = 0x04;
const DIR_LABEL  = 0x08;
const DIR_DIREC  = 0x10;
const DIR_ARCH   = 0x20;

class DirScan
  {
   void *data;
   int ok;
 public:

   DirScan();
   DirScan(const char *wildcard,int attr);
   DirScan(const char *wildcard);
  ~DirScan();

   int first(const char *fname,int attr);           // 0 -> error
   int first(const char *fname);                    // 0 -> error
   int next();                                // 0 -> error
   int operator++()    { return next(); }     // 0 -> error
   int operator++(int) { return next(); }     // 0 -> error

   operator int() const
      {
         return ok;
      }

   char *name() const;
   Date date()  const;
   Time time()  const;
   long size()  const;
   int flags()  const;

   operator char*() const  { return name(); }
   operator Date()  const  { return date(); }
   operator Time()  const  { return time(); }
  };


// Constants for "File" class

const word fmode_read     = 0x000;  // Open for read               ]
const word fmode_write    = 0x001;  // Open for write              ] XOR
const word fmode_rw       = 0x002;  // Open for read/write         ]

const word fmode_create   = 0x004;  // Create new file             ] OR

const word fmode_excl     = 0x010;  // Open for exclusive acccess  ]
const word fmode_denywr   = 0x020;  // Deny writes by others       ] XOR
const word fmode_denyrd   = 0x030;  // Deny reads by other         ]
const word fmode_shared   = 0x040;  // Allow read/write by others  ]

const word fmode_text     = 0x100;  // Open in text mode           ]
const word fmode_copen    = 0x200;  // Create if not exist         ] OR
const word fmode_buffered = 0x400;  // Buffered I/O                ]
const word fmode_append   = 0x800;  // Open for append             ]

const word file_error_ok       = 0;    // No error
const word file_error_nofile   = 2;    // No such file
const word file_error_nopath   = 3;    // No such directory
const word file_error_numfiles = 4;    // No more file handles
const word file_error_noaccess = 5;    // Access denied
const word file_error_badfh    = 6;    // Bad file handle

class File
   {
#ifdef __OS2__
      HFILE fhos2;
#endif
      int   fh;             // file handle (in OS/2: set to -1 if invalid)
      byte *buffer;         // buffer pointer
      byte *bufPtr;         // position in buffer
      word  bufSize;        // buffer size
      word  bytesInBuf;     // # bytes in buffer
      long  bufPos;         // position of buffer in file
      word  mode;           // file mode
      byte  lastAction;     // last action (read/write/seek)
      bool  atEof;          // at EOF flag
      word  errorCode;      // last error code

      void clearError()
         {
            atEof     = FALSE;
            errorCode = FALSE;
         }
      void init();
      bool buffered() const
         {
            return (bool)!!(mode & fmode_buffered);
         }
      bool textMode() const
         {
            return (bool)!!(mode & fmode_text);
         }
      bool fillBuffer();

   public:
      File(const char *fname,word mode = fmode_read,word bufsize = 0);
      File()
         {
            init();
         }
      ~File()
         {
            close();
         }
      bool open(const char *fname,word mode = fmode_read,word bufsize = 0);
      void close();
      bool eof() const
         {
            return atEof;
         }
      word error() const
         {
            return errorCode;
         }
      void enableBuffer();
      void disableBuffer();
      word read(void *p,word size);
      word write(const void *p,word size);
      long seek(long pos,int mode = seek_set);
      int  readByte();
      bool writeByte(byte c);
      bool readLine(char *s,int max);
      String readLine();

      File& operator<<(const char *);
      File& operator<<(int);
      File& operator<<(word);
      File& operator<<(long);
      File& operator<<(dword);
      File& operator<<(char);
      File& operator<<(const String& s) { return operator<<((char *)s); }

      File& operator>>(char&);
      File& operator>>(int&);
      File& operator>>(word&);
      File& operator>>(long&);
      File& operator>>(dword&);
      File& operator>>(String& s) { s = readLine(); return *this; }

      bool printf(const char *fmt,...);
      long pos();
      long len();
      void cut();
      void rewind()
         {
            seek(0);
         }
      bool opened() const
         {
            return bool(fh>0);
         }

      bool lock(long pos,long len);
      bool unlock(long pos,long len);
      void flush();
#ifdef __OS2__
      HFILE os2Handle() { return fhos2; }
#else
      int   dosHandle() { return fh;    }
#endif
   };


/*
template <class T>
File&
operator<<(File& f, T& obj)
{
   f.write(&obj,sizeof(obj));

   return f;
}
*/

class TextFile : public File
   {
   public:
      TextFile()
         {
         }
      TextFile(const char *fname , word mode = fmode_read , word bufsize=512)
      : File(fname , mode | fmode_text , bufsize)
         {
         }
      bool open(const char *fname , word mode = fmode_read , word bufsize=512)
         {
            return File::open(fname , mode | fmode_text , bufsize);
         }
   };

class BitArray
   {
      long start;
      word *array;
   public:
      BitArray(long size,long s = 0);
      ~BitArray();
      bool operator[](long i) const;
      void set(long i);
      void clear(long i);
   };

const byte LOGSTYLE_NORMAL = 0;

class LogFile
   {
      FileName name;
      byte     logStyle;
   public:
      LogFile();
     ~LogFile();

      bool open(const char *fn , byte style = LOGSTYLE_NORMAL);
      bool write(const char *str,...);
   };

char *form(const char * ...);

#ifdef __DOS_H

#ifdef __OS2__
   inline void *normalize_ptr(void *p) { return p; }
#else

inline void _far *
normalize_ptr(void _far *p)
{
   word seg = FP_SEG(p);
   word off = FP_OFF(p);

   return (void _far *) MK_FP(seg + (off>>4),off & 0xF);
}
#endif
#endif

#ifdef Use_Cache
  #include <cache.hpp>
#endif

#ifdef Use_RecordFile
  #include <recfile.hpp>
#endif

#ifdef Use_LinkedList
  #include <llist.hpp>
#endif

#ifdef Use_DynamicArrayFile
  #include <dynafile.hpp>
#endif

#endif
