#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <tslib.hpp>

#include "convert.hpp"
#include "sbbs.hpp"

#define SBBS_FULLSCR 128
#define SBBS_NOHOT 512

char sbbs_path[80];

int
convert_sbbs_users()
{
/*
   File fi_users,
        fi_susers,
        fo_users,
        fo_usersxi,
        fo_userspb;

   UsersXiBBS pb_uxi;
   UsersPbBBS pb_upb;
   UsersBBS   pb_u;

   SBBS_UsersBBS sbbs_u;
   SBBS_SUsersBBS sbbs_su;

   if(!fi_users.open(FileName(sbbs_path,"USERS.BBS")) || !fi_susers.open(FileName(sbbs_path,"SUSERS.BBS")))
   {
      printf("Unable to open superBBS userbase\n");

      return -1;
   }

   if(access(FileName(cfg.msgpath,"USERS.BBS"),0))
   {
      printf("USERS.BBS already exists!!");
      return -1;
   }

   if(!fo_users.open(FileName(cfg.msgpath,"USERS.BBS"), fmode_create))
   {
      printf("Can't create USERS.BBS");

      return -1;
   }

   if(!fo_userspb.open(FileName(cfg.msgpath,"USERS.PB"), fmode_create))
   {
      printf("Can't create USERS.PB");

      return -1;
   }

   if(!fo_usersxi.open(FileName(cfg.msgpath,"USERSXI.BBS"), fmode_create))
   {
      printf("Can't create USERSXI.BBS");

      return -1;
   }

   for(word recno=0;;recno++)
   {
      CLEAR_OBJECT(pb_u);
      CLEAR_OBJECT(pb_uxi);
      CLEAR_OBJECT(pb_upb);
      CLEAR_OBJECT(sbbs_u);
      CLEAR_OBJECT(sbbs_su);

      if(fi_users.read(&sbbs_u,sizeof(sbbs_u)) != sizeof(sbbs_u)) break;

      fi_susers.seek(long(sbbs_u.ExtraUserrecPtr) * sizeof(sbbs_su));

      if(fi_susers.read(&sbbs_su,sizeof(sbbs_su)) != sizeof(sbbs_su)) break;

      memcpy(&pb_u,&sbbs_u,sizeof(pb_u) - 8);
      pb_u.XIrecord = recno;

      memcpy(pb_uxi.Handle       , sbbs_su.Alias          , 36 );
      memcpy(pb_uxi.Comment      , sbbs_su.SysopComment   , sbbs_su.SysopComment[0]+1);
      memcpy(pb_uxi.FirstDate    , sbbs_su.FirstDate      , sbbs_su.FirstDate[0]+1);
      memcpy(pb_uxi.CombinedInfo , sbbs_su.CombinedBoards , 25);
      memcpy(pb_uxi.BirthDate    , sbbs_su.Birthday       , sbbs_su.Birthday[0]+1);
      memcpy(pb_uxi.SubDate      , sbbs_su.Expirationdate , sbbs_su.Expirationdate[0]+1);

      pb_uxi.ScreenWidth = 80;
      pb_uxi.MsgArea     = sbbs_su.CurrentMArea;
      pb_uxi.FileArea    = sbbs_su.CurrentFArea;

      memcpy(pb_upb.username,sbbs_u.Name,36);
      pas2c(pb_upb.username);

      pb_upb.tb_timebalance= sbbs_su.Timeinbank;
      pb_upb.totaltimeused = sbbs_su.MinutesUsed;

      if(sbbs_su.Attrib & SBBS_NOHOT)    pb_upb.uflags |= UFLAG_STACKING;
      if(sbbs_su.Attrib & SBBS_FULLSCR)  pb_upb.uflags |= UFLAG_FSED;

      pb_upb.loglevel = 1;

      fo_users.write(&pb_u,sizeof(pb_u));
      fo_usersxi.write(&pb_uxi,sizeof(pb_uxi));
      fo_userspb.write(&pb_upb,sizeof(pb_upb));
   }
*/
   return 0;
};


// conversie file areas

int
convert_sbbs_files()
{
   File superf,pbfiar;
   sufi files;
   _FileArea pbfi;

   if(!superf.open(FileName(sbbs_path,"FLSEARCH.BBS")))
   {
      printf("Unable to open FLSEARCH.BBS");
      return -1;
   }

   if(!pbfiar.open("FILECFG.PRO", fmode_create))
   {
      printf("Can't create FILECFG.PRO");

      return -1;
   }

   for(;;)
   {
      memset(&files,0,sizeof(files));
      memset(&pbfi,0,sizeof(pbfi));

      if(superf.read(&files,sizeof(struct sufi))<sizeof(struct sufi)) break;

      pas2c(files.Name);
      pas2c(files.Directory);
      pas2c(files.ListName);

      strcpy(pbfi.name,files.Name);
      strcpy(pbfi.listpath,files.ListName);
      strcpy(pbfi.filepath,files.Directory);

      if(pbfi.listpath[0] == '\0')
      {
         strcpy(pbfi.listpath,pbfi.filepath);
         append_backspace(pbfi.listpath);
         strcat(pbfi.listpath,"FILES.BBS");
      }

      pbfi.level = files.Security;
      *((long *)&pbfi.flags) = swapbits(files.Flags);

      pbfiar.write(&pbfi,sizeof(pbfi));
   }

   superf.close();
   pbfiar.close();

   return 0;
};

int
convert_sbbs_msgs()
{
   File superm,pbmsgar;
   _MsgArea pbmsg;
   sumsg msgs;

   if(!superm.open(FileName(sbbs_path,"BOARDS.BBS")))
   {
      printf("Unable to open BOARDS.BBS");

      return -1;
   }

   if(!pbmsgar.open("MSGAREAS.PB", fmode_create))
   {
      printf("Can't create MSGAREAS.PB");

      return -1;
   }

   for(;;)
   {
      memset(&msgs,0,sizeof(msgs));
      memset(&pbmsg,0,sizeof(pbmsg));

      if(superm.read(&msgs,sizeof(struct sumsg))<sizeof(struct sumsg)) break;

      pas2c(msgs.Name);
      pas2c(msgs.QwkName);

      if(msgs.Name[0]=='\0' && *msgs.QwkName) strcpy(msgs.Name,msgs.QwkName);

      strcpy(pbmsg.name,msgs.Name);
      strcpy(pbmsg.qwkname,msgs.QwkName);

      pbmsg.msgKind=msgs.Typ;
      pbmsg.msgType=msgs.Kinds;

      if(pbmsg.msgKind==3)pbmsg.msgKind--;
      if(pbmsg.msgType==3)pbmsg.msgType=0;

      pbmsg.readLevel=msgs.ReadSecLvl;
      *((long *)&pbmsg.readFlags) = swapbits(msgs.ReadFlags);

      pbmsg.writeLevel=msgs.WriteSecLvl;
      *((long *)&pbmsg.writeFlags)=swapbits(msgs.WriteFlags);

      pbmsg.sysopLevel=msgs.SysopSecLvl;
      *((long *)&pbmsg.sysopFlags)=swapbits(msgs.SysopFlags);

      pbmsg.aka=msgs.UseAka;

      pbmsgar.write(&pbmsg,sizeof(pbmsg));
   }

   pbmsgar.close();

   return 0;
}

/*
int
convert_sbbs_menus()
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
*/
