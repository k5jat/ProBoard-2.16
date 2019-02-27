#define NO_RA1

#include <stdio.h>
#include <io.h>
#include <ctype.h>
#include <string.h>
#include <tslib.hpp>
#include <stdarg.h>
#include "convert.hpp"
#include "ra1.hpp"
#include "ra2.hpp"
#include "pb.hpp"

struct PB1_MenuItem
   {
      union {
         char type;
         char highlight;
      };
      unsigned level;
      AccessFlags flags;
      char string[76];
      char key;
      char data[81];
      char color;
      byte attr;
   };

struct _OldPrivateFile
   {
      Date date;
      char fname [13];
      char to    [36];
      char from  [36];
  };


static void
dolog(char *str)
{
   Date date(TODAY);
   Time time(NOW);

   File fp(FileName(syspath,"CONVERT.LOG"),fmode_write | fmode_text | fmode_copen | fmode_append);

   fp.printf("[CONVERT PB] %s\n",str);
}


static void
Log(char *str ...)
{
   char s[100];
   va_list va;
   va_start(va,str);

   vsprintf(s,str,va);

   dolog(s);
}


inline bool
PascalStringOk( char *s , int maxlen )
{
   return !(s[0] < 0 || s[0] > maxlen);
}

bool
convert_userfile()
{
   RA1_UsersBBS    ra1_u;
   RA1_UsersXiBBS  ra1_uxi;
   RA1_UsersPbBBS  ra1_upb;
   RA2_UsersBBS    ra2_u;
   RA2_UsersXiBBS  ra2_uxi;
   RA2_UsersPbBBS  ra2_upb;
   RA2_UsersIdxBBS ra2_uidx;

   if(!access(FileName(cfg.msgpath,"USERSPB.BBS"),0))
   {
      printf("Already converted!!");

      return FALSE;
   }

   FileName fni_u   (cfg.msgpath , "USERS.BBS");
   FileName fni_uxi (cfg.msgpath , "USERSXI.BBS");
   FileName fni_upb (cfg.msgpath , "USERS.PB");
   FileName fno_u   (cfg.msgpath , "USERS.$$$");
   FileName fno_uxi (cfg.msgpath , "USERSXI.$$$");
   FileName fno_upb (cfg.msgpath , "USERSPB.$$$");
   FileName fno_uidx(cfg.msgpath , "USERSIDX.$$$");

   File  fi_u,
         fi_uxi,
         fi_upb,
         fo_u,
         fo_uxi,
         fo_upb,
         fo_uidx;

   fi_u.open  (fni_u   , fmode_read | fmode_excl , 2048);
   fi_uxi.open(fni_uxi , fmode_read | fmode_excl , 2048);
   fi_upb.open(fni_upb , fmode_read | fmode_excl , 2048);

   if(!fi_u.opened() || !fi_uxi.opened() || !fi_upb.opened())
   {
      printf("Error opening user file!");
      return FALSE;
   }

   fo_u.open   (fno_u   , fmode_create , 2048);
   fo_uxi.open (fno_uxi , fmode_create , 2048);
   fo_upb.open (fno_upb , fmode_create , 2048);
   fo_uidx.open(fno_uidx, fmode_create , 2048);

   bool error = FALSE;

   for(long record = 0;;record++)
   {
      CLEAR_OBJECT(ra1_u);
      CLEAR_OBJECT(ra1_uxi);
      CLEAR_OBJECT(ra1_upb);
      CLEAR_OBJECT(ra2_u);
      CLEAR_OBJECT(ra2_uxi);
      CLEAR_OBJECT(ra2_upb);

      fi_upb.seek( long(record) * sizeof(ra1_u) );

      if(fi_u.read(&ra1_u , sizeof(ra1_u)) != sizeof(ra1_u))
         break;

      fi_uxi.seek( long(ra1_u.XIrecord) * sizeof(ra1_uxi) );

      if(fi_uxi.read(&ra1_uxi , sizeof(ra1_uxi)) != sizeof(ra1_uxi))
      {
         if(!error) printf("Error! Check CONVERT.LOG");
         Log("User Record #%ld - Invalid XIrec (%ld)",record+1,ra1_u.XIrecord);
         error = TRUE;
         continue;
      }

      fi_upb.seek( long(record) * sizeof(ra1_upb) );

      if(fi_upb.read(&ra1_upb , sizeof(ra1_upb)) != sizeof(ra1_upb))
      {
         if(!error) printf("Error! Check CONVERT.LOG");
         Log("User Record #%ld - USERS.PB EOF",record+1);
         error = TRUE;
         continue;
      }

      if(ra1_u.Password[0] < 0 || ra1_u.Password[0]>15)
      {
         if(!error) printf("Error! Check CONVERT.LOG");
         Log("User Record #%ld - Invalid password len (%d)",record+1,ra1_u.Password[0]);
         error = TRUE;
         continue;
      }

      pas2c(ra1_u.Password);
      ra2_u.PasswordCRC = RaCrc(ra1_u.Password);

      if(   !PascalStringOk( ra1_u.Name       , 35 )
         || !PascalStringOk( ra1_u.Location   , 25 )
         || !PascalStringOk( ra1_u.DataPhone  , 12 )
         || !PascalStringOk( ra1_u.VoicePhone , 12 )
         || !PascalStringOk( ra1_u.LastTime   ,  5 )
         || !PascalStringOk( ra1_u.LastDate   ,  8 ))
      {
         if(!error) printf("Error! Check CONVERT.LOG");
         Log("User Record #%ld - Invalid",record+1);
         error = TRUE;
         continue;
      }

      memcpy( ra2_u.Name       , ra1_u.Name       , 36 );
      memcpy( ra2_u.Location   , ra1_u.Location   , 26 );
      memcpy( ra2_u.DataPhone  , ra1_u.DataPhone  , 13 );
      memcpy( ra2_u.VoicePhone , ra1_u.VoicePhone , 13 );
      memcpy( ra2_u.LastTime   , ra1_u.LastTime   ,  6 );
      memcpy( ra2_u.LastDate   , ra1_u.LastDate   ,  9 );

      if(ra1_u.Attribute & RA1_UFLAG1_DELETED ) ra2_u.Attribute |= RA2_UFLAG1_DELETED;
      if(ra1_u.Attribute & RA1_UFLAG1_CLRSCR  ) ra2_u.Attribute |= RA2_UFLAG1_CLRSCR ;
      if(ra1_u.Attribute & RA1_UFLAG1_MORE    ) ra2_u.Attribute |= RA2_UFLAG1_MORE   ;
      if(ra1_u.Attribute & RA1_UFLAG1_ANSI    ) ra2_u.Attribute |= RA2_UFLAG1_ANSI   ;
      if(ra1_u.Attribute & RA1_UFLAG1_NOKILL  ) ra2_u.Attribute |= RA2_UFLAG1_NOKILL ;
      if(ra1_u.Attribute & RA1_UFLAG1_IGNORE  ) ra2_u.Attribute |= RA2_UFLAG1_IGNORE ;
      if(ra1_u.Attribute & RA1_UFLAG1_FSE     ) ra2_u.Attribute |= RA2_UFLAG1_FSE    ;
      if(ra1_u.Attribute & RA1_UFLAG1_QUIET   ) ra2_u.Attribute |= RA2_UFLAG1_QUIET  ;

      if(ra1_u.Attribute2 & RA1_UFLAG2_HOTKEYS) ra2_u.Attribute2 |= RA2_UFLAG2_HOTKEYS ;
      if(ra1_u.Attribute2 & RA1_UFLAG2_AVATAR )
      {
         ra2_u.Attribute2  |= RA2_UFLAG2_AVATAR;
         ra2_u.Attribute   |= RA2_UFLAG1_ANSI;
      }
      if(ra1_u.Attribute2 & RA1_UFLAG2_FSMSG  ) ra2_u.Attribute2 |= RA2_UFLAG2_FSMSG   ;
      if(ra1_u.Attribute2 & RA1_UFLAG2_HIDDEN ) ra2_u.Attribute2 |= RA2_UFLAG2_HIDDEN  ;

      if(ra1_upb.uflags & RA1_UFLAG3_NOIBM  )   ra2_upb.uFlags |= RA2_UFLAG3_NOIBM   ;
      if(ra1_upb.uflags & RA1_UFLAG3_NOTOPS )   ra2_upb.uFlags |= RA2_UFLAG3_NOTOPS  ;
      if(ra1_upb.uflags & RA1_UFLAG3_ATTEN  )   ra2_upb.uFlags |= RA2_UFLAG3_ATTEN   ;

      if(ra1_upb.uflags & RA1_UFLAG3_AVTPLUS)
      {
         ra2_upb.uFlags    |= RA2_UFLAG3_AVTPLUS;
         ra2_u.Attribute2  |= RA2_UFLAG2_AVATAR;
         ra2_u.Attribute   |= RA2_UFLAG1_ANSI;
      }


      ra2_u.Flags          = ra1_u.Flags;
      ra2_u.Credit         = ra1_u.Credit;
      ra2_u.Pending        = ra1_u.Pending;
      ra2_u.MsgsPosted     = ra1_u.MsgsPosted;
      ra2_u.LastRead       = ra1_u.LastRead;
      ra2_u.Security       = ra1_u.Security;
      ra2_u.NoCalls        = ra1_u.NoCalls;
      ra2_u.Uploads        = ra1_u.Uploads;
      ra2_u.Downloads      = ra1_u.Downloads;
      ra2_u.UploadsK       = ra1_u.UploadsK;
      ra2_u.DownloadsK     = ra1_u.DownloadsK;
      ra2_u.TodayK         = ra1_u.TodayK;
      ra2_u.Elapsed        = ra1_u.Elapsed;
      ra2_u.ScreenLength   = ra1_u.ScreenLength;

      memcpy( ra2_u.Handle    , ra1_uxi.Handle     , 36 );
      memcpy( ra2_u.Comment   , ra1_uxi.Comment    , 81 );
      memcpy( ra2_u.FirstDate , ra1_uxi.FirstDate  ,  9 );
      memcpy( ra2_u.BirthDate , ra1_uxi.BirthDate  ,  9 );
      memcpy( ra2_u.SubDate   , ra1_uxi.SubDate    ,  9 );
      memcpy( ra2_u.ForwardTo , ra1_uxi.ForwardFor , 36 );

      ra2_u.ScreenWidth  = ra1_uxi.ScreenWidth ;
      ra2_u.MsgArea      = ra1_uxi.MsgArea     ;
      ra2_u.FileArea     = ra1_uxi.FileArea    ;
      ra2_u.DateFormat   = ra1_uxi.DateFormat  ;

      memcpy(ra2_upb.name , ra2_u.Name , 36);

      if(!PascalStringOk(ra2_upb.name,35))
      {
         if(!error) printf("Error! Check CONVERT.LOG");
         Log("User Record #%ld - Invalid name len (%d)",record+1,ra2_u.Name[0]);
         error = TRUE;
         continue;
      }

      pas2c(ra2_upb.name);

      ra2_upb.logLevel = ra1_upb.loglevel;
      strcpy(ra2_upb.country,ra1_upb.country);

      ra2_upb.tbTimeBalance   = ra1_upb.tb_timebalance;
      ra2_upb.tbKbBalance     = ra1_upb.tb_kbytebalance;
      ra2_upb.tbTimeWithdrawn = ra1_upb.tb_timewithdrawn;
      ra2_upb.tbKbWithdrawn   = ra1_upb.tb_kbytewithdrawn;
      ra2_upb.tbLastUsed      = ra1_upb.tb_lastused;
      ra2_upb.expLevel        = ra1_upb.expirlevel;
      ra2_upb.totalTimeUsed   = ra1_upb.totaltimeused;

      memcpy(ra2_u.CombinedInfo       , &ra1_upb.boards          , 125);
      memcpy(&ra2_upb.mailCheckBoards , &ra1_upb.mailcheckboards , 125);

      ra2_upb.tbTimeDeposited         = ra1_upb.tb_timedeposited;
      ra2_upb.tbKbDeposited           = ra1_upb.tb_kbytedeposited;
      ra2_upb.tbTimeLoaned            = ra1_upb.tb_timelended;
      ra2_upb.tbKbLoaned              = ra1_upb.tb_kbytelended;
      ra2_upb.tbTimePayback           = ra1_upb.tb_timepayback;
      ra2_upb.tbKbPayback             = ra1_upb.tb_kbytepayback;

      ra2_upb.ripFont                 = ra1_upb.rip_font;
      ra2_upb.qwkArchiver             = ra1_upb.packer;

      ra2_u.DefaultProtocol           = ra1_upb.protocol;

      strcpy(ra2_upb.language,ra1_upb.language);
      strcpy(ra2_upb.passWord,ra1_u.Password);

      ra2_u.XIrecord = record;

      fo_u.write  (&ra2_u   , sizeof(ra2_u));
      fo_uxi.write(&ra2_uxi , sizeof(ra2_uxi));
      fo_upb.write(&ra2_upb , sizeof(ra2_upb));

      pas2c(ra2_u.Name);
      pas2c(ra2_u.Handle);

      ra2_uidx.NameCRC   = RaCrc(ra2_u.Name);
      ra2_uidx.HandleCRC = RaCrc(ra2_u.Handle);

      fo_uidx.write(&ra2_uidx , sizeof(ra2_uidx));
   }

   fi_u.close();
   fi_uxi.close();
   fi_upb.close();
   fo_u.close();
   fo_uxi.close();
   fo_upb.close();
   fo_uidx.close();

   FileName fnb_u   (cfg.msgpath , "USERS.BAK");
   FileName fnb_uxi (cfg.msgpath , "USERSXI.BAK");
   FileName fnb_upb (cfg.msgpath , "USERSPB.BAK");
   FileName fni_uidx(cfg.msgpath , "USERSIDX.BBS");

   unlink(fnb_u);
   unlink(fnb_uxi);
   unlink(fnb_upb);
   unlink(fni_uidx);

   rename(fni_u,fnb_u);
   rename(fni_uxi,fnb_uxi);
   rename(fni_upb,fnb_upb);

   rename(fno_u,fni_u);
   rename(fno_uxi,fni_uxi);
   rename(fno_upb,FileName(cfg.msgpath,"USERSPB.BBS"));
   rename(fno_uidx,fni_uidx);

   return !error;
}

bool
convert_menus()
{
   bool errorsfound = FALSE;
   FileName wcMenus(cfg.mnupath,"*.MNU");

   DirScan scan(wcMenus);

   while(int(scan))
   {
      File fi,fo;

      FileName fnInput(cfg.mnupath,scan.name());
      FileName fnOutput(fnInput);

      fnOutput.changeExt("PBM");

      if(!fi.open(fnInput))
      {
         if(!errorsfound)
            printf("Error!  Check CONVERT.LOG");

         Log("Error opening %s",(char *)fnInput);

         errorsfound++;
         scan++;
         continue;
      }

      if(!fo.open(fnOutput,fmode_create))
      {
         if(!errorsfound)
            printf("Error!  Check CONVERT.LOG");

         Log("Error creating %s",(char *)fnOutput);

         errorsfound++;
         scan++;
         continue;
      }

      PB1_MenuItem old_item;
      _MenuFile menu_header;

      if(fi.read(&old_item,sizeof(old_item)) != sizeof(old_item))
      {
         if(!errorsfound)
            printf("Error!  Check CONVERT.LOG");

         Log("Bad .MNU file: %s",(char *)fnInput);

         errorsfound++;
         scan++;
         continue;
      }

      CLEAR_OBJECT(menu_header);
      strcpy(menu_header.prompt, old_item.string);
      menu_header.color = old_item.color;
      menu_header.highlight = old_item.highlight;
      strcpy(menu_header.RIPname,old_item.data);

      fo.write(&menu_header,sizeof(menu_header));

      for(;;)
      {
         if(fi.read(&old_item,sizeof(old_item)) != sizeof(old_item))
            break;

         _MenuItem mi;

         CLEAR_OBJECT(mi);

         strcpy(mi.text , old_item.string);
         strcpy(mi.data , old_item.data);
         mi.attr = old_item.attr;
         mi.color = old_item.color;
         mi.hotKey = old_item.key;
         mi.function = old_item.type;
         mi.access.minLevel = old_item.level;
         mi.access.maxLevel = 0;
         mi.access.flags = old_item.flags;
         mi.access.minAge = 0;
         mi.access.maxAge = 0;
         mi.access.sex = 0;
         mi.access.minTimeLeft = 0;
         mi.access.minTimeOnline = 0;
         mi.access.timeFrame.setAll();
         mi.access.terminals = 0xFF;
         mi.access.minSpeed = 0;
         mi.access.maxSpeed = 0;

         fo.write(&mi,sizeof(mi));
      }

      fi.close();
      fo.close();

      scan++;
   }

   return !(errorsfound);
}

bool
convert_msgareas()
{
   struct PB200_MsgArea
   {
      int  areaNum;
      char name[31];             // Name of message areas                             2
      byte msgType;              // Type of message (Net/Echo/Local)                 33
      byte msgKind;              // Kind of message (Private only/Public only/...)   34
      byte msgBaseType;          // Type of message base                             35
      char path[80];             // Path to Squish or *.MSG                          36
      byte flags;                // Alias allowed/forced/prohibited                 116
      word readLevel;            // Minimum level needed to read msgs               117
      AccessFlags readFlags;     // flags needed to read msgs                       119
      word writeLevel;           // Minimum level needed to write msgs              123
      AccessFlags writeFlags;    // flags needed to write msgs                      125
      word sysopLevel;           // Minimum level needed to change msgs             129
      AccessFlags sysopFlags;    // flags needed to change msgs                     131

      char origin[62];           // Origin line                                     135
      int aka;                   // AKA                                             197

      int rcvKillDays;           //                                                 199
      int msgKillDays;           //                                                 201
      int maxMsgs;               //                                                 203

      char sysop[36];            //                                                 205
      int  replyBoard;           //                                                 241

      char tag[21];              // EchoMail Tag Name                               243
      char qwkname[13];          // QWK Area Name                                   264

      byte groups[4];            // Groups belonging to                             277
      bool allGroups;            // Belongs to all groups                           281
      byte minAge;               // Minimum age for this area                       282
      AccessFlags readFlagsNot;  // flags non-grata to read msgs                    283
      AccessFlags writeFlagsNot; // flags non-grata to read msgs                    287
      AccessFlags sysopFlagsNot; // flags non-grata to read msgs                    291

      byte extra[5] ;            //                                                 295
   };

   File fi,fo;
   PB200_MsgArea old_ma;
        _MsgArea new_ma;

   if(!fi.open(FileName(syspath,"MSGAREAS.PB"),fmode_read,8192))
   {
      printf("Already Converted");
      return FALSE;
   }

   if(!fo.open(FileName(syspath,"MESSAGES.PB"),fmode_create,8192))
   {
      printf("Error creating MESSAGES.PB!");
      return FALSE;
   }

   for(;;)
   {
      CLEAR_OBJECT(new_ma);

      if(fi.read(&old_ma,sizeof(old_ma)) != sizeof(old_ma))
         break;

      new_ma.areaNum = old_ma.areaNum;
      strcpy(new_ma.name,old_ma.name);
      new_ma.msgType = old_ma.msgType;
      new_ma.msgKind = old_ma.msgKind;
      new_ma.msgBaseType = old_ma.msgBaseType;
      strcpy(new_ma.path,old_ma.path);
      new_ma.flags = old_ma.flags;
      new_ma.readLevel = old_ma.readLevel;
      new_ma.readFlags = old_ma.readFlags;
      new_ma.writeLevel = old_ma.writeLevel;
      new_ma.writeFlags = old_ma.writeFlags;
      new_ma.sysopLevel = old_ma.sysopLevel;
      new_ma.sysopFlags = old_ma.sysopFlags;
      strcpy(new_ma.origin,old_ma.origin);
      new_ma.aka = old_ma.aka;
      new_ma.rcvKillDays = old_ma.rcvKillDays;
      new_ma.msgKillDays = old_ma.msgKillDays;
      new_ma.maxMsgs = old_ma.maxMsgs;
      strcpy(new_ma.sysop,old_ma.sysop);
      new_ma.replyBoard = old_ma.replyBoard;
      strcpy(new_ma.tag,old_ma.tag);
      strcpy(new_ma.qwkname,old_ma.qwkname);

      new_ma.groups[0] = old_ma.groups[0];
      new_ma.groups[1] = old_ma.groups[1];
      new_ma.groups[2] = old_ma.groups[2];
      new_ma.groups[3] = old_ma.groups[3];
      new_ma.allGroups = old_ma.allGroups;
      new_ma.minAge = old_ma.minAge;
      new_ma.readFlagsNot  = old_ma.readFlagsNot ;
      new_ma.writeFlagsNot = old_ma.writeFlagsNot;
      new_ma.sysopFlagsNot = old_ma.sysopFlagsNot;

      if(new_ma.msgBaseType == MSGBASE_HUDSON)
         new_ma.hudsonBase = new_ma.areaNum;

      fo.write(&new_ma,sizeof(new_ma));
   }

   fi.close();
   fo.close();

   FileName fn1(syspath,"MSGAREAS.PB"),
            fn2(syspath,"MSGAREAS.OLD");

   unlink(fn2);
   rename(fn1,fn2);

   return TRUE;
}


bool
convert_modem()
{
   static _ModemConfig newcfg;
   static _OldModemConfig oldcfg;

   File fi,fo;

   if(!fi.open(FileName(syspath,"MODEM.PRO"),fmode_read,8192))
   {
      printf("Already Converted");
      return FALSE;
   }

   if(!fo.open(FileName(syspath,"MODEM.PB"),fmode_create,8192))
   {
      printf("Error creating MODEM.PB!");
      return FALSE;
   }

   CLEAR_OBJECT(newcfg);

   if(fi.read(&oldcfg,sizeof(oldcfg)) != sizeof(oldcfg))
   {
      printf("Error in MODEM.PRO!");
      return FALSE;
   }

   strcpy(newcfg.msgCon300   , oldcfg.connect300   );
   strcpy(newcfg.msgCon1200  , oldcfg.connect1200  );
   strcpy(newcfg.msgCon1275  , "CONNECT 1275"      );
   strcpy(newcfg.msgCon2400  , oldcfg.connect2400  );
   strcpy(newcfg.msgCon4800  , oldcfg.connect4800  );
   strcpy(newcfg.msgCon7200  , oldcfg.connect7200  );
   strcpy(newcfg.msgCon9600  , oldcfg.connect9600  );
   strcpy(newcfg.msgCon12000 , oldcfg.connect12000 );
   strcpy(newcfg.msgCon14400 , oldcfg.connect14400 );
   strcpy(newcfg.msgCon16800 , oldcfg.connect16800 );
   strcpy(newcfg.msgCon19200 , oldcfg.connect19200 );
   strcpy(newcfg.msgCon21600 , "CONNECT 21600"     );
   strcpy(newcfg.msgCon24000 , oldcfg.connect24000 );
   strcpy(newcfg.msgCon26400 , "CONNECT 26400"     );
   strcpy(newcfg.msgCon28800 , oldcfg.connect28800 );
   strcpy(newcfg.msgCon57600 , "CONNECT 57600"     );
   strcpy(newcfg.msgCon64000 , oldcfg.connect64000 );
   strcpy(newcfg.msgCon115200, "CONNECT 115200"    );

   newcfg.maxBps        = oldcfg.baud;
   newcfg.port          = oldcfg.port;
   newcfg.commandDelay  = oldcfg.delay;
   newcfg.blankTime     = oldcfg.blanktime;
   newcfg.flags         = MODEM_LOCKED;

   if(oldcfg.forceanswer) newcfg.flags |= MODEM_MANUAL_ANSWER;

   sprintf(newcfg.msgRing   ,"%s|",oldcfg.ring);
   sprintf(newcfg.msgOk     ,"%s|",oldcfg.initok);
   sprintf(newcfg.cmdInit1  ,"%s|",oldcfg.initstring);
   sprintf(newcfg.cmdOffHook,"%s|",oldcfg.busystring);
   sprintf(newcfg.cmdDown   ,"%s|",oldcfg.busystring);
   sprintf(newcfg.cmdAnswer ,"%s|",oldcfg.answer);

   fo.write(&newcfg,sizeof(newcfg));

   fi.close();
   fo.close();

   FileName fn1(syspath,"MODEM.PRO"),
            fn2(syspath,"MODEM.OLD");

   unlink(fn2);
   rename(fn1,fn2);

   return TRUE;
}

bool
convert_pvtfiles()
{
   _OldPrivateFile oldfile;
   _PrivateFile newfile;

   File fi,fo;

   if(!fi.open(FileName(syspath,"PVTFILE.PRO"),fmode_read,8192))
   {
      printf("Already Converted");
      return FALSE;
   }

   if(!fo.open(FileName(syspath,"PVTFILES.PB"),fmode_create,8192))
   {
      printf("Error creating PVTFILES.PB!");
      return FALSE;
   }

   for(;;)
   {
      CLEAR_OBJECT(newfile);

      if(fi.read(&oldfile,sizeof(oldfile)) != sizeof(oldfile))
         break;

      strcpy(newfile.from  , oldfile.from  );
      strcpy(newfile.to    , oldfile.to    );
      strcpy(newfile.fname , oldfile.fname );

      newfile.date = oldfile.date;

      fo.write(&newfile,sizeof(newfile));
   }

   fi.close();
   fo.close();

   FileName fn1(syspath,"PVTFILE.PRO"),
            fn2(syspath,"PVTFILE.OLD");

   unlink(fn2);
   rename(fn1,fn2);

   return TRUE;
}

bool
convert_config()
{
   byte *cfgptr = (byte *)&cfg;
   byte colors[] = { 0x0E , 0x0B , 0x0A , 0x0C , 0x0D , 0x0B , 0x0F , 0x0C };
   byte zeroes[] = { 0,0,0,0,0,0,0,0 };

   cfgptr += 0x76D;

   if(!memcmp(cfgptr,zeroes,8))
   {
      File f;

      memcpy(cfgptr,colors,8);

      if(!f.open(FileName(syspath,"CONFIG.PRO"),fmode_create))
      {
         printf("Can't create CONFIG.PRO");

         return FALSE;
      }

      f.write(&cfg,sizeof(cfg));
      f.close();

      return TRUE;
   }
   else
   {
      printf("Already Converted");

      return FALSE;
   }
}

static bool
UpdateLanguage(FileName fn,word def_n,_LanguageString *def_p)
{
   static int count = 0;
   word n;
   LanguageFile lf;
   _LanguageString tmp;
   FileName fn2(fn);

   fn2.stripPath();

   if(count++)
      printf("                                  ");

   printf("%-12.12s...",(char *)fn2);

   File f;

   if(!f.open(fn,fmode_rw))
      return FALSE;

   if(f.read(&lf,sizeof(lf)) != sizeof(lf))
      return FALSE;

   long p = f.pos();

   f >> n;

   if(n == def_n)
   {
      printf("Already upgraded\n");
      return TRUE;
   }

   for(int i=1;i<=n;i++)
   {
      if(!tmp.read(f))
         return FALSE;
   }

   for(;i<=def_n;i++)
   {
      def_p[i-351].write(f);
   }

   f.seek(p);
   f << def_n;

   printf("Ok\n");

   return TRUE;
}

bool
convert_languages()
{
   word deflang_n;
   File f;
   LanguageFile lf;
   FileName fnDeflang(syspath,"DEFLANG.PB");

   if(!f.open(fnDeflang))
   {
      printf("Error opening %s",(char *)fnDeflang);

      return FALSE;
   }

   f.read(&lf,sizeof(lf));
   f >> deflang_n;

   _LanguageString *defprompts = new _LanguageString[deflang_n - 350];

   for(word i=1;i<=deflang_n;i++)
   {
      _LanguageString tmp;

      if(i <= 350)
      {
         if(!tmp.read(f))
         {
            printf("Error reading DEFLANG.PB");
            return FALSE;
         }
      }
      else
      {
         if(!defprompts[i-351].read(f))
         {
            printf("Error reading DEFLANG.PB");
            return FALSE;
         }
      }
   }

   f.close();


   DirScan scan(FileName(syspath,"*.PBL"));

   for( ; int(scan) ; scan++)
   {
      FileName pbl_fn(syspath,scan.name());

      if(!UpdateLanguage(pbl_fn,deflang_n,defprompts))
         printf("Error!\n");
   }

   delete [] defprompts;

   return TRUE;
}
