#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <stdlib.h>
#endif

#include <tswin.hpp>

static ATTR err_attr  = 0x5f ,
            err_tattr = 0x5e ;

void
tsw_fatalerror(char *s,ATTR attr,ATTR tattr)
{
 if(attr  != 0) err_attr  = attr;

 if(tattr != 0) err_tattr = tattr;

 Window w(11,10,70,12,err_attr,SHADOW);
 w.open();

 tsw_centerline(10," FATAL ERROR ",err_tattr);
 tsw_centerline(11,s,err_tattr|byte(0x80));
 tsw_centerline(12," Press any key to exit ",err_attr);

 KB.clear();
 KB.get();

 SCREEN.clear();

#ifdef PB_SDK
 exit();
#else
 exit(100);
#endif
}

void
tsw_nonfatalerror(char *s,ATTR attr,ATTR tattr)
{
 if(attr  != 0) err_attr  = attr;
 if(tattr != 0) err_tattr = tattr;

 Window w(11,10,70,12,err_attr,SHADOW|EXPLODE);
 w.open();

 tsw_centerline(10," Error ",err_tattr);
 tsw_centerline(11,s,err_tattr|byte(0x80));
 tsw_centerline(12," Press any key to continue ",err_attr);

 KB.clear();
 KB.get();
}

void
tsw_errorattr(ATTR attr,ATTR tattr)
{
 err_attr  = attr;
 err_tattr = tattr;
}



