#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <tslib.hpp>
#include <pb_lib.hpp>

String _User::sysDir;
String _User::lastSearchSpec;

bool   _User::lastSearchAlias;
bool   _User::lastSearchExact;
bool   _User::lastSearchFuzzy;

long   _User::lastSearchRecord = -1;

_User::_User()
{
   CLEAR_OBJECT( *this );
}

static void
RAdate2PBdate( char *radate , Date& pbdate)
{
   pbdate[1] = atoi(strtok(radate,"-"));
   pbdate[0] = atoi(strtok(NULL,"-"));
   pbdate[2] = atoi(strtok(NULL,"-"));
}

static void
PBdate2RAdate( char *radate , Date& pbdate)
{
   sprintf(radate,"%02d-%02d-%02d",pbdate[1],pbdate[0],pbdate[2]);
}

static void
RAtime2PBtime( char *ratime , Time& pbtime)
{
   pbtime[0] = atoi(strtok(ratime,":"));
   pbtime[1] = atoi(strtok(NULL,":"));
}

static void
PBtime2RAtime( char *ratime , Time& pbtime)
{
   sprintf(ratime,"%02d:%02d",pbtime[0],pbtime[1]);
}

static dword
swapbits(dword x)
{
   dword y = 0;

   for(int i=0;i<32;i++)
      if(TESTBIT(x,word(i)))
         SETBIT(y,word(31-i));

   return y;
}


void
_User::fromRa( RA2_UsersBBS &u , RA2_UsersXiBBS &uxi , RA2_UsersPbBBS &upb)
{
   u.Name[0]         = min(u.Name[0],35);         if(u.Name[0]<0)         u.Name[0] = 0;
   u.Location[0]     = min(u.Location[0],25);     if(u.Location[0]<0)     u.Location[0] = 0;
   u.Organisation[0] = min(u.Organisation[0],50); if(u.Organisation[0]<0) u.Organisation[0] = 0;
   u.Address1[0]     = min(u.Address1[0],50);     if(u.Address1[0]<0)     u.Address1[0] = 0;
   u.Address2[0]     = min(u.Address2[0],50);     if(u.Address2[0]<0)     u.Address2[0] = 0;
   u.Address3[0]     = min(u.Address3[0],50);     if(u.Address3[0]<0)     u.Address3[0] = 0;
   u.Handle[0]       = min(u.Handle[0],35);       if(u.Handle[0]<0)       u.Handle[0] = 0;
   u.Comment[0]      = min(u.Comment[0],80);      if(u.Comment[0]<0)      u.Comment[0] = 0;
   u.DataPhone[0]    = min(u.DataPhone[0],15);    if(u.DataPhone[0]<0)    u.DataPhone[0] = 0;
   u.VoicePhone[0]   = min(u.VoicePhone[0],15);   if(u.VoicePhone[0]<0)   u.VoicePhone[0] = 0;
   u.LastTime[0]     = min(u.LastTime[0],5);      if(u.LastTime[0]<0)     u.LastTime[0] = 0;
   u.LastDate[0]     = min(u.LastDate[0],8);      if(u.LastDate[0]<0)     u.LastDate[0] = 0;
   u.FirstDate[0]    = min(u.FirstDate[0],8);     if(u.FirstDate[0]<0)    u.FirstDate[0] = 0;
   u.BirthDate[0]    = min(u.BirthDate[0],8);     if(u.BirthDate[0]<0)    u.BirthDate[0] = 0;
   u.SubDate[0]      = min(u.SubDate[0],8);       if(u.SubDate[0]<0)      u.SubDate[0] = 0;
   u.ForwardTo[0]    = min(u.ForwardTo[0],35);    if(u.ForwardTo[0]<0)    u.ForwardTo[0] = 0;

   pas2c(u.Name);
   pas2c(u.Location);
   pas2c(u.Organisation);
   pas2c(u.Address1);
   pas2c(u.Address2);
   pas2c(u.Address3);
   pas2c(u.Handle);
   pas2c(u.Comment);
   pas2c(u.DataPhone);
   pas2c(u.VoicePhone);
   pas2c(u.LastTime);
   pas2c(u.LastDate);
   pas2c(u.FirstDate);
   pas2c(u.BirthDate);
   pas2c(u.SubDate);
   pas2c(u.ForwardTo);

   strcpy(name       , u.Name);
   strcpy(city       , u.Location);
   strcpy(company    , u.Organisation);
   strcpy(address1   , u.Address1);
   strcpy(address2   , u.Address2);
   strcpy(address3   , u.Address3);
   strcpy(alias      , u.Handle);
   strcpy(comment    , u.Comment);
   strcpy(dataPhone  , u.DataPhone);
   strcpy(voicePhone , u.VoicePhone);
   strcpy(forwardTo  , u.ForwardTo);

   strcpy(passWord   , upb.passWord);

   passWordCRC = u.PasswordCRC;

   if(RaCrc(upb.passWord) != u.PasswordCRC)
   {
      passWord[0] = '\0';
      passWordCRC = u.PasswordCRC;
   }

   RAtime2PBtime(u.LastTime , lastTime );

   RAdate2PBdate(u.LastDate , lastDate );
   RAdate2PBdate(u.FirstDate, firstDate);
   RAdate2PBdate(u.BirthDate, birthDate);
   RAdate2PBdate(u.SubDate  , expDate  );

   level       = u.Security;

   *((dword *)(&aFlags)) = swapbits(u.Flags);

   uFlags = 0;

   if(u.Attribute & RA_UFLAG_DELETED)      uFlags |= UFLAG_DELETED;
   if(u.Attribute & RA_UFLAG_CLRSCR)       uFlags |= UFLAG_CLEAR;
   if(u.Attribute & RA_UFLAG_MORE)         uFlags |= UFLAG_PAUSE;
   if(u.Attribute & RA_UFLAG_ANSI)         uFlags |= UFLAG_ANSI;
   if(u.Attribute & RA_UFLAG_NOKILL)       uFlags |= UFLAG_NOKILL;
   if(u.Attribute & RA_UFLAG_IGNORE)       uFlags |= UFLAG_IGNORE;
   if(u.Attribute & RA_UFLAG_FSE)          uFlags |= UFLAG_FSED;
   if(u.Attribute & RA_UFLAG_QUIET)        uFlags |= UFLAG_QUIET;

   if(u.Attribute2 & RA_UFLAG2_HOTKEYS)    uFlags |= UFLAG_HOTKEYS;
   if(u.Attribute2 & RA_UFLAG2_AVATAR)     uFlags |= UFLAG_AVATAR;
   if(u.Attribute2 & RA_UFLAG2_HIDDEN)     uFlags |= UFLAG_HIDDEN;
   if(u.Attribute2 & RA_UFLAG2_PAGEPRI)    uFlags |= UFLAG_PAGEPRI;
   if(u.Attribute2 & RA_UFLAG2_GUEST)      uFlags |= UFLAG_GUEST;

   if(upb.uFlags & RA_UFLAG3_NOIBM)        uFlags |= UFLAG_NOIBM;
   if(upb.uFlags & RA_UFLAG3_NOTOPS)       uFlags |= UFLAG_NOTOPS;
   if(upb.uFlags & RA_UFLAG3_AVTPLUS)      uFlags |= UFLAG_AVTPLUS;
   if(upb.uFlags & RA_UFLAG3_ATTEN)        uFlags |= UFLAG_ATTEN;
   if(upb.uFlags & RA_UFLAG3_NORIP)        uFlags |= UFLAG_NORIP;
   if(upb.uFlags & RA_UFLAG3_MULTILOGIN)   uFlags |= UFLAG_MULTILOGIN;
   if(upb.uFlags & RA_UFLAG3_FREECHAT)     uFlags |= UFLAG_FREECHAT;
   if(upb.uFlags & RA_UFLAG3_LOCALONLY)    uFlags |= UFLAG_LOCALONLY;

 //if(u.Attribute2 & RA_UFLAG2_FSMSG)
 //if(u.Attribute2 & RA_UFLAG2_NOECHOSCAN)
 //if(u.Attribute2 & RA_UFLAG2_POSTBILL)

   highMsgRead    = u.LastRead;
   timesCalled    = u.NoCalls;
   numDownloads   = u.Downloads;
   kbDownloaded   = u.DownloadsK;
   numUploads     = u.Uploads;
   kbUploaded     = u.UploadsK;
   kbToday        = u.TodayK;
   msgsPosted     = u.MsgsPosted;
   timeUsed       = u.Elapsed;
   screenLength   = u.ScreenLength;
   screenWidth    = u.ScreenWidth;
   credit         = u.Credit;
   pending        = u.Pending;
   dateFormat     = u.DateFormat;
   msgArea        = u.MsgArea;
   msgGroup       = u.MsgGroup;
   fileArea       = u.FileArea;
   fileGroup      = u.FileGroup;

   defaultProtocol= u.DefaultProtocol;
   sex            = u.Sex;

   if(dateFormat > 1)
      dateFormat = 2;

   strcpy(country , upb.country);
   strcpy(state   , upb.state);
   strcpy(faxPhone, upb.faxPhone);
   strcpy(language, upb.language);

   lastPwdChange        = upb.lastPwdChange;
   lastNewFilesCheck    = upb.lastNewFilesCheck;
   logLevel             = upb.logLevel;
   tbTimeBalance        = upb.tbTimeBalance;
   tbKbBalance          = upb.tbKbBalance;
   tbTimeWithdrawn      = upb.tbTimeWithdrawn;
   tbKbWithdrawn        = upb.tbKbWithdrawn;
   tbLastUsed           = upb.tbLastUsed;
   tbTimeDeposited      = upb.tbTimeDeposited;
   tbKbDeposited        = upb.tbKbDeposited;
   tbTimeLoaned         = upb.tbTimeLoaned;
   tbKbLoaned           = upb.tbKbLoaned;
   tbTimePayback        = upb.tbTimePayback;
   tbKbPayback          = upb.tbKbPayback;

   expLevel             = upb.expLevel;
   expFlagsOn           = upb.expFlagsOn;
   expFlagsOff          = upb.expFlagsOff;
   totalTimeUsed        = upb.totalTimeUsed;

   memcpy(&combinedBoards  , u.CombinedInfo       , 125);
   memcpy(&mailCheckBoards , &upb.mailCheckBoards , 125);

   qwkMaxMsgsPerArea = upb.qwkMaxMsgsPerArea;
   qwkMaxMsgs        = upb.qwkMaxMsgs;
   qwkArchiver       = upb.qwkArchiver;

   checkNewFiles     = upb.checkNewFiles;
   checkMail         = upb.checkMail;

   ripFont           = upb.ripFont;

   CLEAR_OBJECT(u);
   CLEAR_OBJECT(uxi);
   CLEAR_OBJECT(upb);
}

void
_User::toRa( RA2_UsersBBS &u , RA2_UsersXiBBS &uxi , RA2_UsersPbBBS &upb)
{
   CLEAR_OBJECT(u);
   CLEAR_OBJECT(uxi);
   CLEAR_OBJECT(upb);

   strcpy(u.Name,name             );
   strcpy(u.Location,city         );
   strcpy(u.Organisation,company  );
   strcpy(u.Address1,address1     );
   strcpy(u.Address2,address2     );
   strcpy(u.Address3,address3     );
   strcpy(u.Handle,alias          );
   strcpy(u.Comment,comment       );
   strcpy(u.DataPhone,dataPhone   );
   strcpy(u.VoicePhone,voicePhone );
   strcpy(u.ForwardTo,forwardTo   );

   strcpy(upb.passWord,passWord  );
   strcpy(upb.name    ,name      );

   if(passWord[0])
      u.PasswordCRC = RaCrc(passWord);
   else
      u.PasswordCRC = passWordCRC;

   PBtime2RAtime(u.LastTime , lastTime );

   PBdate2RAdate(u.LastDate , lastDate );
   PBdate2RAdate(u.FirstDate, firstDate);
   PBdate2RAdate(u.BirthDate, birthDate);
   PBdate2RAdate(u.SubDate  , expDate  );

   c2pas(u.Name);
   c2pas(u.Location);
   c2pas(u.Organisation);
   c2pas(u.Address1);
   c2pas(u.Address2);
   c2pas(u.Address3);
   c2pas(u.Handle);
   c2pas(u.Comment);
   c2pas(u.DataPhone);
   c2pas(u.VoicePhone);
   c2pas(u.LastTime);
   c2pas(u.LastDate);
   c2pas(u.FirstDate);
   c2pas(u.BirthDate);
   c2pas(u.SubDate);
   c2pas(u.ForwardTo);

   u.Security    = level       ;

   u.Flags = swapbits(*((dword *)(&aFlags)));

   if(uFlags & UFLAG_DELETED)    u.Attribute |= RA_UFLAG_DELETED;
   if(uFlags & UFLAG_CLEAR)      u.Attribute |= RA_UFLAG_CLRSCR;
   if(uFlags & UFLAG_PAUSE)      u.Attribute |= RA_UFLAG_MORE;
   if(uFlags & UFLAG_ANSI)       u.Attribute |= RA_UFLAG_ANSI;
   if(uFlags & UFLAG_NOKILL)     u.Attribute |= RA_UFLAG_NOKILL;
   if(uFlags & UFLAG_IGNORE)     u.Attribute |= RA_UFLAG_IGNORE;
   if(uFlags & UFLAG_FSED)       u.Attribute |= RA_UFLAG_FSE;
   if(uFlags & UFLAG_QUIET)      u.Attribute |= RA_UFLAG_QUIET;
   if(uFlags & UFLAG_HOTKEYS)    u.Attribute2 |= RA_UFLAG2_HOTKEYS;
   if(uFlags & UFLAG_AVATAR)     u.Attribute2 |= RA_UFLAG2_AVATAR;
   if(uFlags & UFLAG_HIDDEN)     u.Attribute2 |= RA_UFLAG2_HIDDEN;
   if(uFlags & UFLAG_PAGEPRI)    u.Attribute2 |= RA_UFLAG2_PAGEPRI;
   if(uFlags & UFLAG_GUEST)      u.Attribute2 |= RA_UFLAG2_GUEST;
   if(uFlags & UFLAG_NOIBM)      upb.uFlags |= RA_UFLAG3_NOIBM;
   if(uFlags & UFLAG_NOTOPS)     upb.uFlags |= RA_UFLAG3_NOTOPS;
   if(uFlags & UFLAG_AVTPLUS)    upb.uFlags |= RA_UFLAG3_AVTPLUS;
   if(uFlags & UFLAG_ATTEN)      upb.uFlags |= RA_UFLAG3_ATTEN;
   if(uFlags & UFLAG_NORIP)      upb.uFlags |= RA_UFLAG3_NORIP;
   if(uFlags & UFLAG_MULTILOGIN) upb.uFlags |= RA_UFLAG3_MULTILOGIN;
   if(uFlags & UFLAG_FREECHAT)   upb.uFlags |= RA_UFLAG3_FREECHAT;
   if(uFlags & UFLAG_LOCALONLY)  upb.uFlags |= RA_UFLAG3_LOCALONLY;

 //if(u.Attribute2 & RA_UFLAG2_FSMSG)
 //if(u.Attribute2 & RA_UFLAG2_NOECHOSCAN)
 //if(u.Attribute2 & RA_UFLAG2_POSTBILL)

   u.LastRead      = highMsgRead    ;
   u.NoCalls       = timesCalled    ;
   u.Downloads     = numDownloads   ;
   u.DownloadsK    = kbDownloaded   ;
   u.Uploads       = numUploads     ;
   u.UploadsK      = kbUploaded     ;
   u.TodayK        = kbToday        ;
   u.MsgsPosted    = msgsPosted     ;
   u.Elapsed       = timeUsed       ;
   u.ScreenLength  = screenLength   ;
   u.ScreenWidth   = screenWidth    ;
   u.Credit        = credit         ;
   u.Pending       = pending        ;
   u.DateFormat    = dateFormat     ;
   u.MsgArea       = msgArea        ;
   u.FileArea      = fileArea       ;
   u.DefaultProtocol = defaultProtocol;
   u.Sex           = sex;
   u.MsgGroup      = msgGroup;
   u.FileGroup     = fileGroup;

   if(u.DateFormat > 1)
      u.DateFormat = 3;

   strcpy(upb.country , country);
   strcpy(upb.state   , state);
   strcpy(upb.faxPhone, faxPhone);
   strcpy(upb.language, language);

   upb.lastPwdChange       = lastPwdChange     ;
   upb.lastNewFilesCheck   = lastNewFilesCheck ;
   upb.logLevel            = logLevel          ;
   upb.tbTimeBalance       = tbTimeBalance     ;
   upb.tbKbBalance         = tbKbBalance       ;
   upb.tbTimeWithdrawn     = tbTimeWithdrawn   ;
   upb.tbKbWithdrawn       = tbKbWithdrawn     ;
   upb.tbTimeDeposited     = tbTimeDeposited   ;
   upb.tbKbDeposited       = tbKbDeposited     ;
   upb.tbTimeLoaned        = tbTimeLoaned      ;
   upb.tbKbLoaned          = tbKbLoaned        ;
   upb.tbTimePayback       = tbTimePayback     ;
   upb.tbKbPayback         = tbKbPayback       ;
   upb.tbLastUsed          = tbLastUsed        ;
   upb.expLevel            = expLevel          ;
   upb.expFlagsOn          = expFlagsOn        ;
   upb.expFlagsOff         = expFlagsOff       ;
   upb.totalTimeUsed       = totalTimeUsed     ;

   memcpy(u.CombinedInfo       , &combinedBoards , 125);
   memcpy(&upb.mailCheckBoards , &mailCheckBoards, 125);

   upb.qwkMaxMsgsPerArea = qwkMaxMsgsPerArea;
   upb.qwkMaxMsgs        = qwkMaxMsgs;
   upb.qwkArchiver       = qwkArchiver;
   upb.checkNewFiles     = checkNewFiles;
   upb.checkMail         = checkMail;

   upb.ripFont           = ripFont;
}


bool
_User::read( long rec )
{
   FileName fn_u  (sysDir , "USERS.BBS");
   FileName fn_upb(sysDir , "USERSPB.BBS");

   File     f_u,
            f_upb;

   if(!f_u.open(fn_u) || !f_upb.open(fn_upb))
   {
      return FALSE;
   }

   RA2_UsersBBS    *u    = new RA2_UsersBBS;
   RA2_UsersXiBBS  *uxi  = new RA2_UsersXiBBS;
   RA2_UsersPbBBS  *upb  = new RA2_UsersPbBBS;

   int error = 0;

   f_u.seek(rec * sizeof(*u));
   if(f_u.read(u,sizeof(*u)) != sizeof(*u)) error++;

   f_upb.seek(rec * sizeof(*upb));
   if(f_upb.read(upb,sizeof(*upb)) != sizeof(*upb)) error++;

   if(!error) fromRa(*u,*uxi,*upb);

   record = rec;

   delete u;
   delete uxi;
   delete upb;

   strip_all(alias);

   if(alias[0] == '\0')
      strcpy(alias,name);

   if(screenLength < 5)
      screenLength = 24;

   return !error;
}

long
_User::numUsers()
{
   FileName fn(sysDir , "USERS.BBS");

   File     f;

   if(!f.open(fn))
   {
      return 0;
   }

   return f.len() / sizeof(RA2_UsersBBS);
}

/*
String
_User::findAlias(char *name)
{
   RA2_UsersBBS u;
   String empty_s;
   FileName fn(sysDir , "USERS.BBS");

   File     f;

   if(!f.open(fn))
   {
      return empty_s;
   }

   for(;;)
   {
      if(f.read(&u,sizeof(u)) != sizeof(u))
         break;

      pas2c(u.Name);
      pas2c(u.Handle);

      if(!stricmp(u.Name,name))
         return u.Handle;
   }

   return empty_s;
}
*/

bool
_User::write( bool append )
{
   if(!strcmp(alias,name))
      alias[0] = '\0';

   FileName fn_u  (sysDir , "USERS.BBS");
   FileName fn_uxi(sysDir , "USERSXI.BBS");
   FileName fn_upb(sysDir , "USERSPB.BBS");
   FileName fn_uidx(sysDir, "USERSIDX.BBS");

   File     f_u,
            f_uxi,
            f_upb,
            f_uidx;

   if(   !f_u.open   (fn_u   ,fmode_rw | fmode_copen)
      || !f_uxi.open (fn_uxi ,fmode_rw | fmode_copen)
      || !f_upb.open (fn_upb ,fmode_rw | fmode_copen)
      || !f_uidx.open(fn_uidx,fmode_rw | fmode_copen))
   {
      return FALSE;
   }

   RA2_UsersBBS     *u    = new RA2_UsersBBS;
   RA2_UsersXiBBS   *uxi  = new RA2_UsersXiBBS;
   RA2_UsersPbBBS   *upb  = new RA2_UsersPbBBS;
   RA2_UsersIdxBBS  *uidx = new RA2_UsersIdxBBS;

   uidx->NameCRC   = RaCrc(name);
   uidx->HandleCRC = RaCrc(alias);

   toRa(*u,*uxi,*upb);

   if(append)
      record = f_u.len() / sizeof(*u);

   u->XIrecord = record;

   int error = 0;

   f_u.seek(record * sizeof(*u));
   if(f_u.write(u,sizeof(*u)) != sizeof(*u)) error++;

   f_uxi.seek(record * sizeof(*uxi));
   if(f_uxi.write(uxi,sizeof(*uxi)) != sizeof(*uxi)) error++;

   f_uidx.seek(record * sizeof(*uidx));
   if(f_uidx.write(uidx,sizeof(*uidx)) != sizeof(*uidx)) error++;

   f_upb.seek(record * sizeof(*upb));
   if(f_upb.write(upb,sizeof(*upb)) != sizeof(*upb)) error++;

   delete u;
   delete uxi;
   delete upb;

   return !error;
}

bool
_User::search()
{
   RA2_UsersBBS *u       = new RA2_UsersBBS;
   RA2_UsersIdxBBS *uidx = new RA2_UsersIdxBBS;

   dword crc = RaCrc(lastSearchSpec);

   File  f;

   bool found = FALSE;

   if(lastSearchExact)
   {
      if(f.open(FileName( sysDir , "USERSIDX.BBS" ), fmode_read , 2048))
      {
         f.seek((++lastSearchRecord) * sizeof(*uidx));

         for(;;)
         {
            if(f.read( uidx , sizeof(*uidx) ) != sizeof(*uidx)) break;

            if(lastSearchAlias)
            {
               if(crc == uidx->HandleCRC)
               {
                  found = TRUE;
                  break;
               }
            }
            else
            {
               if(crc == uidx->NameCRC)
               {
                  found = TRUE;
                  break;
               }
            }

            lastSearchRecord++;
         }

         if(!found)
            lastSearchRecord = f.len() / sizeof(*u);
      }
   }
   else
   {
      if(f.open(FileName( sysDir , "USERS.BBS" ), fmode_read , 4096))
      {
         f.seek((++lastSearchRecord) * sizeof(*u));

         for(;;)
         {
            if(f.read( u , sizeof(*u) ) != sizeof(*u)) break;

            pas2c(u->Name);
            pas2c(u->Handle);

            String sName(u->Name);
            String sHandle(u->Handle);

            if(lastSearchAlias)
            {
               if(lastSearchFuzzy)
               {
                  if(sHandle.fuzzySearch(lastSearchSpec,75) > 0)
                  {
                     found = TRUE;
                     break;
                  }
               }
               else
               {
                  if(!stricmp( u->Handle , lastSearchSpec))
                  {
                     found = TRUE;
                     break;
                  }
               }
            }
            else
            {
               if(lastSearchFuzzy)
               {
                  if(sName.fuzzySearch(lastSearchSpec,75) > 0)
                  {
                     found = TRUE;
                     break;
                  }
               }
               else
               {
                  if(!stricmp( u->Name , lastSearchSpec))
                  {
                     found = TRUE;
                     break;
                  }
               }
            }

            lastSearchRecord++;
         }

         if(!found)
            lastSearchRecord = f.len() / sizeof(*u);
      }
   }

   f.close();

   delete u;
   delete uidx;

   if(found)
   {
      read(lastSearchRecord);
   }

   return found;
}

bool
_User::search( char *spec , bool exact , bool fuzzy , bool findalias )
{
   lastSearchSpec   = spec;
   lastSearchExact  = exact;
   lastSearchFuzzy  = fuzzy;
   lastSearchAlias  = findalias;
   lastSearchRecord = -1;

   return search();
}

bool
_User::searchNext()
{
   return search();
}

void
_User::newUser()
{
   CLEAR_OBJECT(*this);

   defaultProtocol = 'Z';

   strcpy(language, "ENGLISH");

   screenWidth       = 80;
   screenLength      = 24;
   logLevel          = 1;
   uFlags            = UFLAG_HOTKEYS;
   qwkMaxMsgsPerArea = 100;
   qwkMaxMsgs        = 500;
   fileArea          = 1;
   msgArea           = 1;

   memset(&mailCheckBoards,0xFF,125);
   memset(&combinedBoards ,0xFF,125);
}

String
_User::firstName()
{
   char s[36];

   strcpy(s,name);

   if(strchr(s,' '))
      *strchr(s,' ') = '\0';

   return String(s);
}

