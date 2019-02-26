#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tslib.hpp>
#include "convert.hpp"
#include "qbbs.hpp"
#include "pb.hpp"

char qbbs_mnu_path[61];
char qbbs_path[61];

static int
read_qbbs_paths()
{
 File f;
 FileName fn(qbbs_path,"QUICKCFG.DAT");

 if(!f.open(fn))
   {
    printf("Can't open %s\n",(char *)fn);
    return -1;
   }

 f.seek(554);
 f.read(qbbs_mnu_path,61);
 f.close();

 pas2c(qbbs_mnu_path);

 append_backspace(qbbs_mnu_path);

 return 0;
}


static long
swapbits(long x)
{
 long y = 0;

 for(int i=0;i<32;i++) if(TESTBIT(x,long(i))) SETBIT(y,long(31-i));

 return y;
}

static byte
cvtcolor(byte fg,byte bg)
{
 char colors[] = { 0,4,2,6,1,5,3,7 };

 return ((fg&8) | colors[fg&7] | (colors[bg&7] << 4)) ^ 8;
}

int
convert_qbbs_msgs()
{
 QBBS_MsgArea qbbs_ma;
 _MsgArea pb_ma;

 File fi,fo;

 FileName fn(qbbs_path,"MSGCFG.DAT");
 if(!fi.open(fn))
   {
    printf("Can't open %s",(char *)fn);
    return -1;
   }

 fo.open(FileName(syspath,"MSGAREAS.PB"),fmode_create);

 for(int num = 1;;num++)
   {
    if(fi.read(&qbbs_ma,sizeof(qbbs_ma)) != sizeof(qbbs_ma)) break;

    pas2c(qbbs_ma.Name);
    pas2c(qbbs_ma.OriginLine);

    memset(&pb_ma,0,sizeof(pb_ma));

    pb_ma.areaNum = num;

    strcpy(pb_ma.name,qbbs_ma.Name);
    strcpy(pb_ma.origin,qbbs_ma.OriginLine);

    *((long *)&pb_ma.readFlags)  = swapbits(qbbs_ma.ReadFlags);
    *((long *)&pb_ma.writeFlags) = swapbits(qbbs_ma.WriteFlags);
    *((long *)&pb_ma.sysopFlags) = swapbits(qbbs_ma.SysopFlags);

    pb_ma.readLevel  = qbbs_ma.ReadLevel;
    pb_ma.writeLevel = qbbs_ma.WriteLevel;
    pb_ma.sysopLevel = qbbs_ma.SysopLevel;

    pb_ma.rcvKillDays = qbbs_ma.KillRcvd;
    pb_ma.msgKillDays = qbbs_ma.KillOld;
    pb_ma.maxMsgs      = qbbs_ma.KeepCnt;

    pb_ma.msgType       = qbbs_ma.Kind;
    pb_ma.msgKind       = (qbbs_ma.Type > 2) ? 2 : qbbs_ma.Type;

    fo.write(&pb_ma,sizeof(pb_ma));
   }

 return 0;
}

int
convert_qbbs_files()
{
 QBBS_FileArea qbbs_ma;
 PB_FileArea pb_ma;

 File fi,fo;

 FileName fn(qbbs_path,"FILECFG.DAT");
 if(!fi.open(fn))
   {
    printf("Can't open %s",(char *)fn);
    return -1;
   }

 fo.open(FileName(syspath,"FILECFG.PRO"),fmode_create);

 for(;;)
   {
    if(fi.read(&qbbs_ma,sizeof(qbbs_ma)) != sizeof(qbbs_ma)) break;

    pas2c(qbbs_ma.Name);
    pas2c(qbbs_ma.FilePath);
    pas2c(qbbs_ma.ListPath);

    memset(&pb_ma,0,sizeof(pb_ma));

    strcpy(pb_ma.name,qbbs_ma.Name);
    strcpy(pb_ma.filepath,qbbs_ma.FilePath);
    strcpy(pb_ma.listpath,qbbs_ma.ListPath);

    append_backspace(pb_ma.filepath);

    if(!strchr(pb_ma.listpath,'\\'))
      {
       strcpy(pb_ma.listpath,pb_ma.filepath);
       strcat(pb_ma.listpath,"FILES.BBS");
      }

    pb_ma.flags = swapbits(qbbs_ma.Flags);
    pb_ma.level = qbbs_ma.Level;

    fo.write(&pb_ma,sizeof(pb_ma));
   }

 return 0;
}

int
convert_qbbs_menus()
{
 struct menuitem
   {
    union {
           char type;
           char highlight;
          };
    int level;
    long flags;
    char string[76];
    char key;
    char data[81];
    char fg,bg;
   };

 unsigned func_cvt[] = {  0, 1, 2, 3, 4, 5,61, 7, 8, 9,
                         10,11,12,13,14,00,16,17,18,19,
                         20,21,22,23,24,25,00,27,28,00,
                         30,31,32,33,34,35,36,37,38,39,
                         40,41,59,00,00,00,55,00,32,49,
                         54,00,50,00,53,00,00,57,58,00,
                         00,53,00,00,00,00,00,00,00,00,
                         00,00,00,00,00
                        };

 if(read_qbbs_paths()<0) return -1;

 FileName scan_fn(qbbs_mnu_path,"*.MNU");

 DirScan scan(scan_fn);

 if(!int(scan))
   {
    printf("No menus found!");
    return -1;
   }

 for(;int(scan);scan++)
   {
    File fi,fo;
    FileName fn(qbbs_mnu_path,scan.name());

    if(!fi.open(fn)) continue;

    fo.open(FileName(cfg.mnupath,scan.name()),fmode_create);

    menuitem mi;

    fi.read(&mi,sizeof(mi));

    pas2c(mi.string);
    mi.type = cvtcolor(mi.type,mi.bg);
    mi.fg   = cvtcolor(mi.fg,mi.bg);
    fo.write(&mi,sizeof(mi));

    for(;;)
      {
       if(fi.read(&mi,sizeof(mi))!=sizeof(mi)) break;
       mi.type = func_cvt[mi.type];
       mi.fg   = cvtcolor(mi.fg,mi.bg);
       pas2c(mi.string);
       pas2c(mi.data);
       mi.flags = swapbits(mi.flags);

       fo.write(&mi,sizeof(mi));
      }
   }

 return 0;
}
