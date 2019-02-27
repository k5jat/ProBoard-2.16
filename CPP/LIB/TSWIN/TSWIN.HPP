#include <tslib.hpp>

#ifndef TSWIN_HPP
#define TSWIN_HPP

#define SHADOW       1  // Window modes
#define EXPLODE      2  // idem
#define NOBORDER     4  // idem
#define NOSAVE       8  // idem
#define NOCLEAR     16  // idem
#define NOIMPLODE   32  // idem
#define CENTERED_X  64
#define CENTERED_Y 128
#define BRACKETS   256

#define SCAN_ALL   0x00 // Scan modes for Window::scan()
#define SCAN_UPPER 0x01 //
#define SCAN_NUM   0x02 //
#define SCAN_FIELD 0x08 //
#define SCAN_CLEAR 0x10 //

#define MONOCHROME 1  // tsw_scrmode
#define COLOR 2       //

#define HERCULES 1    //
#define EGA      2    // tsw_videocard
#define CGAMDA   3    //

#define UP 6          // for scroll()
#define DOWN 7        //

#define SF_RETURN 0   //
#define SF_ESC    1   //
#define SF_UP     2   // Return values for scan() functions
#define SF_DOWN   3   //
#define SF_HOT    4   //

#define ED_ESC 1      // for editform
#define ED_HOT 2

#define SL_ESC     (-1)
#define SL_HOTKEY  (-2)

#define VL_ESC     (-1)
#define VL_HOTKEY  (-2)


typedef byte ATTR;

#ifndef PB_SDK
  typedef word KEY;


const KEY KEY_NONE = 0x0000U;

const KEY KEY_F1   = 0x3B00U;
const KEY KEY_F2   = 0x3C00U;
const KEY KEY_F3   = 0x3D00U;
const KEY KEY_F4   = 0x3E00U;
const KEY KEY_F5   = 0x3F00U;
const KEY KEY_F6   = 0x4000U;
const KEY KEY_F7   = 0x4100U;
const KEY KEY_F8   = 0x4200U;
const KEY KEY_F9   = 0x4300U;
const KEY KEY_F10  = 0x4400U;

const KEY KEY_SF1  = 0x5400U;
const KEY KEY_SF2  = 0x5500U;
const KEY KEY_SF3  = 0x5600U;
const KEY KEY_SF4  = 0x5700U;
const KEY KEY_SF5  = 0x5800U;
const KEY KEY_SF6  = 0x5900U;
const KEY KEY_SF7  = 0x5A00U;
const KEY KEY_SF8  = 0x5B00U;
const KEY KEY_SF9  = 0x5C00U;
const KEY KEY_SF10 = 0x5D00U;

const KEY KEY_CF1  = 0x5E00U;
const KEY KEY_CF2  = 0x5F00U;
const KEY KEY_CF3  = 0x6000U;
const KEY KEY_CF4  = 0x6100U;
const KEY KEY_CF5  = 0x6200U;
const KEY KEY_CF6  = 0x6300U;
const KEY KEY_CF7  = 0x6400U;
const KEY KEY_CF8  = 0x6500U;
const KEY KEY_CF9  = 0x6600U;
const KEY KEY_CF10 = 0x6700U;

const KEY KEY_AF1  = 0x6800U;
const KEY KEY_AF2  = 0x6900U;
const KEY KEY_AF3  = 0x6A00U;
const KEY KEY_AF4  = 0x6B00U;
const KEY KEY_AF5  = 0x6C00U;
const KEY KEY_AF6  = 0x6D00U;
const KEY KEY_AF7  = 0x6E00U;
const KEY KEY_AF8  = 0x6F00U;
const KEY KEY_AF9  = 0x7000U;
const KEY KEY_AF10 = 0x7100U;

const KEY KEY_ALT1 = 0x7800U;
const KEY KEY_ALT2 = 0x7900U;
const KEY KEY_ALT3 = 0x7A00U;
const KEY KEY_ALT4 = 0x7B00U;
const KEY KEY_ALT5 = 0x7C00U;
const KEY KEY_ALT6 = 0x7D00U;
const KEY KEY_ALT7 = 0x7E00U;
const KEY KEY_ALT8 = 0x7F00U;
const KEY KEY_ALT9 = 0x8000U;
const KEY KEY_ALT0 = 0x8100U;

const KEY KEY_ALTA = 0x1E00U;
const KEY KEY_ALTB = 0x3000U;
const KEY KEY_ALTC = 0x2E00U;
const KEY KEY_ALTD = 0x2000U;
const KEY KEY_ALTE = 0x1200U;
const KEY KEY_ALTF = 0x2100U;
const KEY KEY_ALTG = 0x2200U;
const KEY KEY_ALTH = 0x2300U;
const KEY KEY_ALTI = 0x1700U;
const KEY KEY_ALTJ = 0x2400U;
const KEY KEY_ALTK = 0x2500U;
const KEY KEY_ALTL = 0x2600U;
const KEY KEY_ALTM = 0x3200U;
const KEY KEY_ALTN = 0x3100U;
const KEY KEY_ALTO = 0x1800U;
const KEY KEY_ALTP = 0x1900U;
const KEY KEY_ALTQ = 0x1000U;
const KEY KEY_ALTR = 0x1300U;
const KEY KEY_ALTS = 0x1F00U;
const KEY KEY_ALTT = 0x1400U;
const KEY KEY_ALTU = 0x1600U;
const KEY KEY_ALTV = 0x2F00U;
const KEY KEY_ALTW = 0x1100U;
const KEY KEY_ALTX = 0x2D00U;
const KEY KEY_ALTY = 0x1500U;
const KEY KEY_ALTZ = 0x2C00U;

const KEY KEY_CTLA = 0x0001U;
const KEY KEY_CTLB = 0x0002U;
const KEY KEY_CTLC = 0x0003U;
const KEY KEY_CTLD = 0x0004U;
const KEY KEY_CTLE = 0x0005U;
const KEY KEY_CTLF = 0x0006U;
const KEY KEY_CTLG = 0x0007U;
const KEY KEY_CTLH = 0x0008U;
const KEY KEY_CTLI = 0x0009U;
const KEY KEY_CTLJ = 0x000AU;
const KEY KEY_CTLK = 0x000BU;
const KEY KEY_CTLL = 0x000CU;
const KEY KEY_CTLM = 0x000DU;
const KEY KEY_CTLN = 0x000EU;
const KEY KEY_CTLO = 0x000FU;
const KEY KEY_CTLP = 0x0010U;
const KEY KEY_CTLQ = 0x0011U;
const KEY KEY_CTLR = 0x0012U;
const KEY KEY_CTLS = 0x0013U;
const KEY KEY_CTLT = 0x0014U;
const KEY KEY_CTLU = 0x0015U;
const KEY KEY_CTLV = 0x0016U;
const KEY KEY_CTLW = 0x0017U;
const KEY KEY_CTLX = 0x0018U;
const KEY KEY_CTLY = 0x0019U;
const KEY KEY_CTLZ = 0x001AU;

const KEY KEY_DEL  = 0x5300U;
const KEY KEY_INS  = 0x5200U;
const KEY KEY_HOME = 0x4700U;
const KEY KEY_END  = 0x4F00U;

const KEY KEY_PGUP = 0x4900U;
const KEY KEY_PGDN = 0x5100U;
const KEY KEY_UP   = 0x4800U;
const KEY KEY_DN   = 0x5000U;
const KEY KEY_LT   = 0x4B00U;
const KEY KEY_RT   = 0x4D00U;

const KEY KEY_ESC  = 0x001BU;
const KEY KEY_ENT  = 0x000DU;
const KEY KEY_RET  = 0x000DU;
const KEY KEY_TAB  = 0x0009U;
const KEY KEY_STAB = 0x0F00U;

const KEY KEY_CPGUP = 0x8400U;
const KEY KEY_CPGDN = 0x7600U;

const KEY KEY_CTL_LT = 0x7300U;
const KEY KEY_CTL_RT = 0x7400U;

#endif

extern char far *tsw_videobase; // Base address of video
extern byte      tsw_videocard; //
extern byte      tsw_scrmode;   // Screen mode
extern int       tsw_vsize;     // lines on screen
extern int       tsw_hsize;     // Colums on screen
extern int       tsw_OS;        // Operating environment
extern bool      tsw_shutup;    //
extern bool      tsw_cursorstate;

const int OS_DOS       = 1;
const int OS_DESQVIEW  = 2;
const int OS_DOUBLEDOS = 3;
const int OS_TOPVIEW   = 4;
const int OS_OS2       = 5;
const int OS_WINDOWS   = 6;

#define FRM_STRING    1
#define FRM_FILENAME  2
#define FRM_PATH      3
#define FRM_INT       4
#define FRM_UNSIGNED  5
#define FRM_LONG      6
#define FRM_ULONG     7
#define FRM_CHAR      8
#define FRM_SHORT     9
#define FRM_YESNO     10
#define FRM_CHOICE    11
#define FRM_FUNCTION  12
#define FRM_DATE      13
#define FRM_TIME      14

#define FRM_UPPER     1
#define FRM_ANYKEY    2
#define FRM_NOWAIT    4

class Window;

struct Field
   {
      int type;
      void *value;
      void *data;
      KEY (*func)(int,Window&,int,int,char,void *);
      int flags;
      int len;
      int width;
      int x,y;
   };

extern const char *DOUBLE_BORDER;
extern const char *SINGLE_BORDER;
extern const char *BLOCK_BORDER;
extern const char *CHISEL_BORDER;
extern const char *RECESS_BORDER;

extern "C"
   {   // Assembler functions
      void     tsw_mputc      (int x,int y,char c);
      void     tsw_maputc     (int x,int y,ATTR attr,char c);
      void     tsw_wait20ms   ();
      char     tsw_whaton     (int,int);
      void     tsw_changeatt  (ATTR attr,int x1,int y1,int x2,int y2);
      void     tsw_scroll     (char mode,char x1,char y1,char x2,char y2,char lines,ATTR attr);
      void     tsw_fillrect   (int x1,int y1,int x2,int y2,char c,ATTR attr);
      void     tsw_maputs     (int x,int y,ATTR attr,char *s);
      void     tsw_mputs      (int x,int y,char *s);
      void     tsw_gettext    (word *,int x1,int y1,int x2,int y2);
      void     tsw_puttext    (word *,int x1,int y1,int x2,int y2);
      int      tsw_printerstat(int printer=0);
      void     tsw_gotoxy     (int,int);

      int      check_key();
      void     slide(void _far *);
   }

void     tsw_cursoron   ();
void     tsw_cursoroff  ();


void tsw_beep           (word freq=1000,word dur=100);
char tsw_choice         (char *list);
void tsw_errorattr      (ATTR,ATTR);
void tsw_fatalerror     (char *string,ATTR attr = 0,ATTR tattr = 0);
void tsw_nonfatalerror  (char *string,ATTR attr = 0,ATTR tattr = 0);
void tsw_prependbs      (char *string);
int  tsw_checkdisk      (int);
void tsw_centerline     (int line,char *str,ATTR attr = 0);
void tsw_drawbox        (int x1,int y1,int x2,int y2,const char *border,ATTR);
void tsw_chiselbox      (int x1,int y1,int x2,int y2,const char *border,ATTR,ATTR);
void tsw_recessbox      (int x1,int y1,int x2,int y2,const char *border,ATTR,ATTR,ATTR,ATTR);
void tsw_showfooter     (char *string,ATTR attr=0x70, ATTR high=0);
void tsw_clearfooter    ( ATTR attr=0x07 );
void tsw_clearrect      (int x1,int y1,int x2,int y2,ATTR attr);
void tsw_selbar         (int line,int x1,int x2,ATTR attr,int height=1);
void tsw_fillscreen     (char c,ATTR attr);

void tsw_msgbox(byte attr, char *title , ... );

int  tsw_selbox( int    x,
                 int    y,
                 int    maxh,
                 byte   attr,
                 char **choices,
                 int    mode,
                 int    defChoice = 0,
                 const char *border = DOUBLE_BORDER,
                 ATTR  b_attr = 0,
                 char *title  = NULL,
                 ATTR  t_attr = 0 );

class CursorState
   {
      bool prev;
   public:
      CursorState(bool mode)
         {
            prev = tsw_cursorstate;

            if(mode)
               tsw_cursoron();
            else
               tsw_cursoroff();
         }
     ~CursorState()
         {
            if(prev)
               tsw_cursoron();
            else
               tsw_cursoroff();
         }
   };

class SaveBuffer
   {
      word *ptr;
      int   xs,ys;
   public:
      SaveBuffer  ();
      ~SaveBuffer ();
      void paste  (int x,int y);
      void paste  (int xf,int yf,int xt,int yt,int xsize,int ysize);
      void copy   (int x,int y,int xs,int ys);
      void clear  ();
   };

#define TERMINAL_NORMAL          0
#define TERMINAL_ANSI            1
#define TERMINAL_AVT_REPCHAR     2
#define TERMINAL_AVT             3
#define TERMINAL_AVT_ATTR        4
#define TERMINAL_AVT_MOVE        5
#define TERMINAL_AVT_RLE         6

class Window
   {
      SaveBuffer buf;
      int curX,curY;
      int x1,y1,x2,y2;
      const char *borderType;
      int mode;
      char titleStr[81];
      int oldX[5],oldY[5];
      int sp;
      bool noCursorUpdate;

      struct {
         byte status;
         byte needed;
         int  p[5];
         int  par;
         byte *rle_buf;
         } terminal;

      int lowlevel_scanstring(char *str,unsigned len,byte mode,unsigned w);
      void (*initFunc)();
      void putShadow(int x1,int y1,int x2,int y2);
      void explode();
      void implode();
      void updateCursor();
      void _init();
    protected:
      KEY *scanHotKeys;
      ATTR fieldAttr;
      ATTR attr,t_attr,b_attr;
      ATTR tt_attr;
      ATTR iShadowColor;
      ATTR iOuterColor;
    public:
      KEY scanHotKey;
      int minX,minY,maxX,maxY;

      void RecessBox( int x1,int y1,int x2,int y2,const char *border,ATTR,ATTR,ATTR,ATTR);

      Window();
      Window(int x1,int y1,int x2,int y2,ATTR attr,int type=EXPLODE,const char *border=DOUBLE_BORDER,ATTR b_attr=0,char *title=NULL,ATTR t_attr=0,ATTR o_attr=0, ATTR tt_attr=0);
      ~Window()  { close(); }
      void ShadowColor( ATTR iColor ) { iShadowColor = iColor; }
      void open( int x1,
                 int y1,
                 int x2,
                 int y2,
                 ATTR attr,
                 int type = EXPLODE,
                 const char *border = DOUBLE_BORDER,
                 ATTR b_attr  = 0,
                 char *title  = NULL,
                 ATTR t_attr  = 0, 
                 ATTR tt_attr = 0);
      void open();
      void close();
      void initFunction(void(*f)())
         {
            initFunc = f;
         }
      void clear();
      void border(const char *type,ATTR attr=0);
      void border(ATTR attr);
      void moveCursor (int dx,int dy);
      void cursorUp   (int n = 1);
      void cursorDown (int n = 1);
      void cursorLeft (int n = 1);
      void cursorRight(int n = 1);
      void getPos(int& x,int& y)
         {
            x = curX;
            y = curY;
         }
      void setPos(int x,int y);
      int  getX()
         {
            return curX;
         }
      int  getY()
         {
            return curY;
         }
      void saveCursor();
      void restoreCursor();
      void attrib(ATTR a) { attr=a; }
      ATTR attrib() { return attr; }
      void scroll(byte direction=UP,word numlines=1);
      void title(char *string,ATTR attr=0);
      void title(ATTR a);
      Window& operator<<(char *);
      Window& operator<<(char);
      void direct(int x,int y,ATTR a,char *str);
      void direct(int x,int y,char *str);
      void direct(int x,int y,char c);
      void direct(int x,int y,ATTR a,char c);
      void centerLine(int y,char *s,ATTR a=0);
      char whaton(int x,int y);
      int  scan(char *s,unsigned len,unsigned w=0,byte mode=0);
      int  scan(int& s,unsigned len,unsigned w=0,byte mode=0);
      int  scan(unsigned& s,unsigned len,unsigned w=0,byte mode=0);
      int  scan(long& s,unsigned len,unsigned w=0,byte mode=0);
      int  scan(Date& d,char *fmt);
      void setScanHotKeys(KEY *h = NULL)
         {
            scanHotKeys = h;
         }
      KEY  getScanHotkey()
         {
            return scanHotKey;
         }
      void zoom();
      void change(int x1,int y1,int x2,int y2);
      void frame();
      void setFieldAttr(ATTR a)
         {
            fieldAttr = a;
         }
      void gauge(int x,int y,ATTR a,int len,dword val,dword max,bool rewrite = TRUE);
      void gauge(int x,int y,int len,dword val,dword max,bool rewrite = TRUE)
         {
            gauge(x,y,attr,len,val,max,rewrite);
         }
      bool inEscape()
         {
            return bool(terminal.status != 0);
         }
      void disableCursor()
         {
            noCursorUpdate = TRUE;
         }
      void enableCursor()
         {
            noCursorUpdate = FALSE;
            updateCursor();
         }
      void placeCursor();
      int  selBox(char **choices,byte battr = 0x70,int defChoice = 0);
    };


class FormWindow : public Window
   {
      Field *f;
      int    numFields;
      ATTR   dispAttr;
      ATTR   selAttr;
      KEY   *hotkeys;
      void   (*helpFunc)(int);
      int    cur_field;

      int  editField(int);
   public:
      void showField(int);
      FormWindow();
      FormWindow(int x1,int y1,int x2,int y2,ATTR attr,int type=EXPLODE,const char *border=DOUBLE_BORDER,ATTR b_attr=0,char *title=NULL,ATTR t_attr=0);
      FormWindow(Field *,int num_forms,ATTR disp_attr=0,ATTR sel_attr=0x70,KEY *hotkeys=NULL,void(*func)(int)=NULLFUNC);
      void define(Field *,int num_forms,ATTR disp_attr=0,ATTR sel_attr=0x70,KEY *hotkeys=NULL,void(*func)(int)=NULLFUNC);
      int process();
      void setField(int field=0)
         {
            cur_field = field;
         }

      int getField()
         {
           return(cur_field);
         }
   };


class EditWindow : public Window
   {
      int ed_x,ed_y;       // Current cursor position
      int ed_x1,ed_y1;     // Upper left corner of window
      int numlines;        // # lines in text
      String *line;        // Array of lines

      void display(int line);
      void display();
      void display_cursor();
      void key_up();
      void key_down();
      void key_left();
      void key_right();
      void key_pageup();
      void key_pagedown();
      void key_del();
      void key_enter();
      void key_end();
      void key_home();
      void key_bs();
      void key_char(char k);
   public:
      EditWindow();
      ~EditWindow();
      void feed(char *text);
      KEY  edit(KEY *hotkeys=NULL);
      void retrieve(char *text,int maxsize=0);
   };

class ViewWindow : public Window
   {
      int start,end;
      KEY *hotkeys;
      void (*func)(int i,int x,int y);
      int num_entries;
   public:
      KEY hotkey;

      ViewWindow() { }
      ViewWindow(int num,void(*function)(int,int,int),KEY *hot);
      void define(int num,void(*function)(int,int,int),KEY *hot);
      int process();
      void showlast();
      void showfirst() { end=0; }
    };

class SelectWindow : public Window
   {
      int start,end;
      KEY *hotkeys;
      void (*func)(int,int,int);
      void (*emptyfunc)(int,int);
      void (*action)(int);
      char *selected;
      ATTR selAttr,bar_attr;
      int num_entries;
   public:
      int current;
      KEY hotkey;

      SelectWindow() { }

      SelectWindow( int  num,
                    ATTR battr,
                    void(*f)(int,int,int),
                    ATTR  sattr = 0,
                    char *sel   = NULL,
                    KEY  *hot   = NULL,
                    void(*actionf)(int) = NULLFUNC,
                    void(*emptyfunc)(int,int) = NULLFUNC );

      void define( int  num,
                   ATTR battr,
                   void(*f)(int,int,int),
                   ATTR  sattr = 0,
                   char *sel   = NULL,
                   KEY *hot    = NULL,
                   void(*actionf)(int)       = NULLFUNC,
                   void(*emptyfunc)(int,int) = NULLFUNC );
      void redefine(int num);
      int process();
      void show(int n);
      void showlast()  { show(num_entries-1); }
      void showfirst() { show(0); }
   };

struct menu_item
   {
      KEY   hotkey;
      int   highlight1,highlight2;
      char *s;
   };

class PopMenu : public Window
   {
      int current;
      menu_item *items;
      KEY *hotkeys;
      ATTR bar_attr,high_attr;
      int num_entries;
      void (*action)(int);
   public:
      KEY hotkey;

      PopMenu();
      PopMenu(menu_item *i,ATTR b_attr=0x70,ATTR h_attr=0x70,KEY *hot=NULL,void (*func)(int)=NULLFUNC);
      void define(menu_item *i,ATTR b_attr=0x70,ATTR h_attr=0x70,KEY *hot=NULL,void (*func)(int)=NULLFUNC);
      int process();
      void refresh(int n=1);
    };

class KeyBoard
   {
      KEY last;
      KEY pushed;
      void (*help)();
   public:
      KeyBoard();
      ~KeyBoard()
         {
            tsw_cursoron();
         }
      void push(KEY k)
         {
            pushed = k;
         }
      KEY get();
      void get(KEY& k)
         {
            k = get();
         }
      KEY uget();
      void uget(KEY& k)
         {
            k = uget();
         }
      void getlast(KEY& c)
         {
            c = last;
         }
      KEY getlast()
         {
            return last;
         }
      bool hit();
      void clear();
      char choice(const char *s);
      bool ask();
      void helpfunc(void(*helpfunc)())
         {
            help = helpfunc;
         }
   };

class ScreenRestore
   {
      word *buffer;
      word pos;
      byte mode;
   public:
      ScreenRestore(int mode=0);
     ~ScreenRestore();
   };

extern Window SCREEN; // For screen output
extern KeyBoard KB;   // For keyboard input

#endif

