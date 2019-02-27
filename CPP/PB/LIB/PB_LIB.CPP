#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <tslib.hpp>

#include "pb_lib.hpp"

#ifndef max
 #define max(a,b) (((a)>(b)) ? (a) : (b))
 #define min(a,b) (((a)<(b)) ? (a) : (b))
#endif


CombinedType::CombinedType()
{
 for(int i=0;i<125;i++) bits[i] = 0xFF;
}

bool
CombinedType::connected(int brdnr)
{
 if(brdnr < 1 || brdnr > 1000) return FALSE;

 brdnr--;
 return (bits[brdnr/8] & (1<<(brdnr%8))) ? TRUE : FALSE;
}

void
CombinedType::connect(int brdnr)
{
 if(brdnr < 1 || brdnr > 1000) return;

 brdnr--;
 bits[brdnr/8] |= (1 << (brdnr%8));
}

void
CombinedType::disconnect(int brdnr)
{
 if(brdnr < 1 || brdnr > 1000) return;

 brdnr--;
 bits[brdnr/8] &= ~(1 << (brdnr%8));
}

void
CombinedType::toggle(int brdnr)
{
 if(brdnr < 1 || brdnr > 1000) return;

 if(connected(brdnr)) disconnect(brdnr);
                 else connect(brdnr);
}

bool
AccessFlags::getflag(int flagnr)
{
 return ((v&(1L<<(32-flagnr)))?1:0);
}

void
AccessFlags::setflag(int flagnr)
{
 v |= 1L << (32-flagnr);
}

void
AccessFlags::clearflag(int flagnr)
{
 v &= ~(1L << (32-flagnr));
}

void
AccessFlags::flagstostr(char *str)
{
 for(int i=1;i<=32;i++)
   {
    if(i>26) *str++ = getflag(i) ? char(i-27+'1') : '-';
        else *str++ = getflag(i) ? char(i-1 +'A') : '-';
   }

 *str = 0;
}

/*
void
_User::clear()
{
 memset(this,0,sizeof(*this));

 int i;

 for(i = 1 ; i <= 1000 ; i++) boards.connect(i);
 for(i = 1 ; i <= 1000 ; i++) mailcheckboards.connect(i);

 loglevel = 1;
 lastfilearea=lastmsgarea=1;
}

_User::_User()
{
 clear();
}

_User::~_User()
{
}
*/


/*
dword
RA_CRC(char *s)
{
 char x[36];
 strcpy(x,s);
 strupr(x);
 c2pas(x);

 return (x[0]) ? ~crc32(x,x[0]) : 0;
}
*/

/*
static long near
swapbits(long x)
{
 long y = 0;

 for(int i=0;i<32;i++)
   if(TESTBIT(x,i))
      SETBIT(y,31-i);

 return y;
}
*/

/*
void
_User::from_ra(UsersBBS &u,UsersXiBBS &uxi,UsersPbBBS &up)
{
 clear();

 u.Name[0]         = min(u.Name[0],35);         if(u.Name[0]<0)         u.Name[0] = 0;
 u.Location[0]     = min(u.Location[0],25);     if(u.Location[0]<0)     u.Location[0] = 0;
 u.Password[0]     = min(u.Password[0],15);     if(u.Password[0]<0)     u.Password[0] = 0;
 u.DataPhone[0]    = min(u.DataPhone[0],12);    if(u.DataPhone[0]<0)    u.DataPhone[0] = 0;
 u.VoicePhone[0]   = min(u.VoicePhone[0],12);   if(u.VoicePhone[0]<0)   u.VoicePhone[0] = 0;
 u.LastTime[0]     = min(u.LastTime[0],5);      if(u.LastTime[0]<0)     u.LastTime[0] = 0;
 u.LastDate[0]     = min(u.LastDate[0],8);      if(u.LastDate[0]<0)     u.LastDate[0] = 0;
 uxi.Handle[0]     = min(uxi.Handle[0],35);     if(uxi.Handle[0]<0)     uxi.Handle[0] = 0;
 uxi.Comment[0]    = min(uxi.Comment[0],80);    if(uxi.Comment[0]<0)    uxi.Comment[0] = 0;
 uxi.BirthDate[0]  = min(uxi.BirthDate[0],8);   if(uxi.BirthDate[0]<0)  uxi.BirthDate[0] = 0;
 uxi.SubDate[0]    = min(uxi.SubDate[0],8);     if(uxi.SubDate[0]<0)    uxi.SubDate[0] = 0;
 uxi.FirstDate[0]  = min(uxi.FirstDate[0],8);   if(uxi.FirstDate[0]<0)  uxi.FirstDate[0] = 0;
 uxi.ForwardFor[0] = min(uxi.ForwardFor[0],35); if(uxi.ForwardFor[0]<0) uxi.ForwardFor[0] = 0;

 pas2c(u.Name);
 pas2c(u.Location);
 pas2c(u.Password);
 pas2c(u.DataPhone);
 pas2c(u.VoicePhone);
 pas2c(u.LastTime);
 pas2c(u.LastDate);
 pas2c(uxi.Handle);
 pas2c(uxi.Comment);
 pas2c(uxi.BirthDate);
 pas2c(uxi.SubDate);
 pas2c(uxi.FirstDate);
 pas2c(uxi.ForwardFor);

 strcpy(name,u.Name);
 strcpy(alias,uxi.Handle);
 strcpy(city,u.Location);
 strcpy(password,u.Password);
 strcpy(datatelnr,u.DataPhone);
 strcpy(telnr,u.VoicePhone);
 strcpy(comment,uxi.Comment);
 strcpy(forwardto,uxi.ForwardFor);

 birthday[1] = atoi(strtok(uxi.BirthDate,"-"));
 birthday[0] = atoi(strtok(NULL,"-"));
 birthday[2] = atoi(strtok(NULL,"-"));

 firstdate[1] = atoi(strtok(uxi.FirstDate,"-"));
 firstdate[0] = atoi(strtok(NULL,"-"));
 firstdate[2] = atoi(strtok(NULL,"-"));

 expir[1] = atoi(strtok(uxi.SubDate,"-"));
 expir[0] = atoi(strtok(NULL,"-"));
 expir[2] = atoi(strtok(NULL,"-"));

 lastdate[1] = atoi(strtok(u.LastDate,"-"));
 lastdate[0] = atoi(strtok(NULL,"-"));
 lastdate[2] = atoi(strtok(NULL,"-"));

 lasttime[0] = atoi(strtok(u.LastTime,":"));
 lasttime[1] = atoi(strtok(NULL,":"));

 level = u.Security;

 timescalled  = u.NoCalls;
 timeused     = u.Elapsed;
 numdownloads = u.Downloads;
 kdownloaded  = u.DownloadsK;
 numuploads   = u.Uploads;
 kuploaded    = u.UploadsK;
 messposted   = u.MsgsPosted;
 todayk       = u.TodayK;
 credit       = u.Credit;
 pending      = u.Pending;
 high_msgread = u.LastRead;

 boards = up.boards;
 for(int z=0;z<25;z++) ((byte *)(&boards))[z] = uxi.CombinedInfo[z];

 *((long *)(&aflags)) = swapbits(u.Flags);

 screenlines = u.ScreenLength;

 if(u.Attribute & RA_UFLAG_DELETED)      uflags |= UFLAG_DELETED;
 if(u.Attribute & RA_UFLAG_CLRSCR)       uflags |= UFLAG_CLEAR;
 if(u.Attribute & RA_UFLAG_MORE)         uflags |= UFLAG_PAUSE;
 if(u.Attribute & RA_UFLAG_ANSI)         uflags |= UFLAG_ANSI;
 if(u.Attribute & RA_UFLAG_NOKILL)       uflags |= UFLAG_NOKILL;
 if(u.Attribute & RA_UFLAG_IGNORE)       uflags |= UFLAG_IGNORE;
 if(u.Attribute & RA_UFLAG_FSE)          uflags |= UFLAG_FSED;
// if(u.Attribute & RA_UFLAG_QUIET)        uflags |= UFLAG_QUIET;

 if(u.Attribute2 & RA_UFLAG2_HIDDEN)     uflags |= UFLAG_HIDDEN;
 if(u.Attribute2 & RA_UFLAG2_AVATAR)     uflags |= UFLAG_AVATAR;

 if(up.uflags & RA_UFLAG3_NOTOPS)        uflags |= UFLAG_NOTOPS;
 if(up.uflags & RA_UFLAG3_NOIBM)         uflags |= UFLAG_NOIBM;
 if(up.uflags & RA_UFLAG3_AVTPLUS)       uflags |= UFLAG_AVTPLUS;
 if(up.uflags & RA_UFLAG3_ATTEN)         uflags |= UFLAG_ATTEN;
 if(up.uflags & RA_UFLAG3_NORIP)         uflags |= UFLAG_NORIP;

 if(!(u.Attribute2 & RA_UFLAG2_HOTKEYS)) uflags |= UFLAG_STACKING;

 lastmsgarea  = uxi.MsgArea  + up.hibyte_msgarea  * 0x100;
 lastfilearea = uxi.FileArea + up.hibyte_filearea * 0x100;

 loglevel      = up.loglevel;
 totaltimeused = up.totaltimeused;
 expirlevel    = up.expirlevel;

 mailcheckboards = up.mailcheckboards;

 for(z=1;z<=1000;z++) if(mailcheckboards.connected(z)) break;

 if(z>1000) for(z=1;z<=1000;z++) mailcheckboards.connect(z);

 tb_timebalance    = up.tb_timebalance;
 tb_kbytebalance   = up.tb_kbytebalance;
 tb_timewithdrawn  = up.tb_timewithdrawn;
 tb_kbytewithdrawn = up.tb_kbytewithdrawn;
 tb_lastused       = up.tb_lastused;

 tb_timedeposited  = up.tb_timedeposited;
 tb_kbytedeposited = up.tb_kbytedeposited;
 tb_timelended     = up.tb_timelended;
 tb_kbytelended    = up.tb_kbytelended;
 tb_timepayback    = up.tb_timepayback;
 tb_kbytepayback   = up.tb_kbytepayback;

 rip_font          = up.rip_font;
 packer            = up.packer;
 protocol          = up.protocol;

 passwordCrc       = up.passwordCrc;

 strcpy(country,up.country);
 strcpy(language,up.language);
}

void
_User::to_ra(UsersBBS &ra_u,UsersXiBBS &ra_uxi,UsersPbBBS &ra_upb)
{
 CLEAR_OBJECT(ra_u);
 CLEAR_OBJECT(ra_uxi);
 CLEAR_OBJECT(ra_upb);


 name[35]      = '\0';
 alias[35]     = '\0';
 city[25]      = '\0';
 password[15]  = '\0';
 datatelnr[12] = '\0';
 telnr[12]     = '\0';
 comment[80]   = '\0';
 forwardto[35] = '\0';

 strcpy(ra_u.Name,name);
 strcpy(ra_u.Location,city);
 strcpy(ra_u.Password,password);
 strcpy(ra_u.DataPhone,datatelnr);
 strcpy(ra_u.VoicePhone,telnr);
 strcpy(ra_uxi.Handle,alias);
 strcpy(ra_uxi.Comment,comment);
 strcpy(ra_uxi.ForwardFor,forwardto);

 sprintf(ra_uxi.BirthDate,"%02d-%02d-%02d",birthday[1],birthday[0],birthday[2]);
 sprintf(ra_uxi.FirstDate,"%02d-%02d-%02d",firstdate[1],firstdate[0],firstdate[2]);
 sprintf(ra_uxi.SubDate  ,"%02d-%02d-%02d",expir[1],expir[0],expir[2]);
 sprintf(ra_u  .LastDate ,"%02d-%02d-%02d",lastdate[1],lastdate[0],lastdate[2]);
 sprintf(ra_u  .LastTime ,"%02d:%02d"     ,lasttime[0],lasttime[1]);

 ra_u.Security     = level        ;
 ra_u.NoCalls      = timescalled  ;
 ra_u.Elapsed      = timeused     ;
 ra_u.Downloads    = numdownloads ;
 ra_u.DownloadsK   = kdownloaded  ;
 ra_u.Uploads      = numuploads   ;
 ra_u.UploadsK     = kuploaded    ;
 ra_u.MsgsPosted   = messposted   ;
 ra_u.TodayK       = todayk       ;
 ra_u.ScreenLength = screenlines  ;
 ra_u.Credit       = credit       ;
 ra_u.Pending      = pending      ;
 ra_u.LastRead     = high_msgread ;

 for(int z=0;z<25;z++) ra_uxi.CombinedInfo[z] = ((byte *)(&boards))[z];
 ra_upb.boards = boards;

 ra_u.Flags       = swapbits(*((long *)&aflags));

 if(uflags & UFLAG_DELETED) ra_u.Attribute |= RA_UFLAG_DELETED;
 if(uflags & UFLAG_CLEAR)   ra_u.Attribute |= RA_UFLAG_CLRSCR;
 if(uflags & UFLAG_PAUSE)   ra_u.Attribute |= RA_UFLAG_MORE;
 if(uflags & UFLAG_ANSI)    ra_u.Attribute |= RA_UFLAG_ANSI;
 if(uflags & UFLAG_NOKILL)  ra_u.Attribute |= RA_UFLAG_NOKILL;
 if(uflags & UFLAG_IGNORE)  ra_u.Attribute |= RA_UFLAG_IGNORE;
 if(uflags & UFLAG_FSED)    ra_u.Attribute |= RA_UFLAG_FSE;
// if(uflags & UFLAG_QUIET)   ra_u.Attribute |= RA_UFLAG_QUIET;

 if(uflags & UFLAG_HIDDEN)  ra_u.Attribute2 |= RA_UFLAG2_HIDDEN;
 if(uflags & UFLAG_AVATAR)  ra_u.Attribute2 |= RA_UFLAG2_AVATAR;

 if(!(uflags & UFLAG_STACKING)) ra_u.Attribute2 |= RA_UFLAG2_HOTKEYS;

 if(uflags & UFLAG_NOTOPS)  ra_upb.uflags |= RA_UFLAG3_NOTOPS;
 if(uflags & UFLAG_NOIBM)   ra_upb.uflags |= RA_UFLAG3_NOIBM;
 if(uflags & UFLAG_AVTPLUS) ra_upb.uflags |= RA_UFLAG3_AVTPLUS;
 if(uflags & UFLAG_ATTEN)   ra_upb.uflags |= RA_UFLAG3_ATTEN;
 if(uflags & UFLAG_NORIP)   ra_upb.uflags |= RA_UFLAG3_NORIP;

 ra_upb.loglevel      = loglevel;
 ra_upb.totaltimeused = totaltimeused;

 strcpy(ra_upb.username,name);
 strcpy(ra_upb.country,country);
 strcpy(ra_upb.language,language);

 ra_upb.expirlevel = expirlevel;

 ra_upb.mailcheckboards = mailcheckboards;

 ra_upb.tb_timebalance    = tb_timebalance;
 ra_upb.tb_kbytebalance   = tb_kbytebalance;
 ra_upb.tb_timewithdrawn  = tb_timewithdrawn;
 ra_upb.tb_kbytewithdrawn = tb_kbytewithdrawn;
 ra_upb.tb_lastused       = tb_lastused;

 ra_upb.tb_timedeposited  = tb_timedeposited;
 ra_upb.tb_kbytedeposited = tb_kbytedeposited;
 ra_upb.tb_timelended     = tb_timelended;
 ra_upb.tb_kbytelended    = tb_kbytelended;
 ra_upb.tb_timepayback    = tb_timepayback;
 ra_upb.tb_kbytepayback   = tb_kbytepayback;

 ra_upb.rip_font          = rip_font;
 ra_upb.packer            = packer;
 ra_upb.protocol          = protocol;

 ra_upb.passwordCrc       = passwordCrc;

 ra_uxi.MsgArea  = lastmsgarea & 0xFF;
 ra_uxi.FileArea = lastfilearea & 0xFF;

 ra_upb.hibyte_msgarea  = (lastmsgarea >> 8)  & 0xFF;
 ra_upb.hibyte_filearea = (lastfilearea >> 8) & 0xFF;

 c2pas(ra_u.Name);
 c2pas(ra_u.Location);
 c2pas(ra_u.Password);
 c2pas(ra_u.DataPhone);
 c2pas(ra_u.VoicePhone);
 c2pas(ra_u.LastTime);
 c2pas(ra_u.LastDate);
 c2pas(ra_uxi.Handle);
 c2pas(ra_uxi.Comment);
 c2pas(ra_uxi.BirthDate);
 c2pas(ra_uxi.SubDate);
 c2pas(ra_uxi.FirstDate);
 c2pas(ra_uxi.ForwardFor);
}
*/

void
getsyspath(char *syspath,char *argv0)
{
 syspath[0] = 0;

 if(getenv("PROBOARD") != NULL)
   {
    strcpy(syspath,getenv("PROBOARD"));
   }
  else
   {
    FileName fn(argv0);
    fn.stripName();
    strcpy(syspath,fn);
   }

 append_backspace(syspath);
}

bool
_QbbsMsgHdr::fix()
{
 bool damaged = FALSE;

 if(nextmsg < 0 || nextmsg > 32000) nextmsg = 0;
 if(prevmsg < 0 || prevmsg > 32000) prevmsg = 0;

 if(msgnum  < 1 || msgnum > 32000)
   {
    damaged = TRUE;
    msgnum = 0;
   }

 if(area < 1 || area > 200)
   {
    damaged = TRUE;
    area = 1;
   }

 posttime[0] = 5;
 postdate[0] = 8;

 if(from[0] < 1 || from[0] > 35) { from[0] = 1; from[1] = '*'; }
 if(to[0]   < 1 || to[0]   > 35) { to[0]   = 1; to[1]   = '*'; }
 if(subj[0] > 66) subj[0] = 66;
 if(subj[0] < 1 ) { subj[0] = 1; subj[1] = '*'; }

 return !damaged;
}

TimeFrame::TimeFrame()
{
   clear();
}

void
TimeFrame::clear()
{
   memset(periods , 0, sizeof(periods));
}

void
TimeFrame::setAll()
{
   memset(periods , 0xFF, sizeof(periods));
}

bool
TimeFrame::enabled() const
{
   return enabled(Date(TODAY),Time(NOW));
}

bool
TimeFrame::enabled(Time& time) const
{
   return enabled(Date(TODAY), time);
}

bool
TimeFrame::enabled(Date& date,Time& time) const
{
   return enabled(date.weekDay() , time);
}

bool
TimeFrame::enabled(int dow,Time& time) const
{
   return enabled(dow,(time[0] * 60 + time[1]) / 30);
}

bool
TimeFrame::enabled(int dow,int period) const
{
   return (periods[dow][period/8] & (1<<(period%8))) ? TRUE : FALSE;
}

void
TimeFrame::enable(int dow,Time& time)
{
   enable(dow,(time[0] * 60 + time[1]) / 30);
}

void
TimeFrame::enable(int dow,int period)
{
   periods[dow][period/8] |= (1 << (period%8));
}

void
TimeFrame::disable(int dow,Time& time)
{
   disable(dow,(time[0] * 60 + time[1]) / 30);
}

void
TimeFrame::disable(int dow,int period)
{
   periods[dow][period/8] &= ~(1 << (period%8));
}

dword
JamCrc(char *s)
{
   String tmp(s);
   tmp.lowerCase();

   return ~crc32(tmp);
}

dword
RaCrc(char *s)
{
   char x[36];
   strcpy(x,s);
   strupr(x);

   return (s[0]) ? ~crc32(x) : 0;
}

