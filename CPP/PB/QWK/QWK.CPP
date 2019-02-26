#define Use_LinkedList

#include <pb_sdk.h>
#include <tslib.hpp>
#include "qwk.hpp"
#include "qwk_arch.hpp"

const word MSGTEXT_BUFSIZE = 8192;
const word IO_BUFSIZE      = 16384;

struct PROTOCOL
  {
   char name[50];       /* Name of protocol                                  */
   char key;            /* Hotkey for Protocol                               */
   char flags;          /* Protocol behavior                                 */
   char logfile[60];    /* Logfile from protocol                             */
   char ctlfile[60];    /* Control file (redirection file)                   */
   char dlcom[128];     /* Download command line                             */
   char ulcom[128];     /* Upload command line                               */
   char dlstring[80];   /* String to place in download control file          */
   char ulstring[80];   /* String to place in upload control file            */
   char dlkeyword[40];  /* Download keyword to search for in logfile         */
   char ulkeyword[40];  /* Upload keyword to search for in logfile           */
   int wordnr;          /* File name word nr after keyword (1-..)            */
  };


/* Flags: */

#define PROT_BATCH   1    /* Batch protocol        */
#define PROT_ENABLED 2    /* Enabled               */
#define PROT_BOTH    4    /* Full-duplex protocol  */
#define PROT_BIM     8    /* Bimodem-type ctl-file */
#define PROT_LOCAL  16    /* Local protocol        */

struct LastRead
   {
      int  area;
      long lr;

      LastRead() {}
      LastRead( int a, long l )
         {
            area = a;
            lr   = l;
         }
   };

LinkedList<LastRead> NewLastRead;
FileName WorkDir;
int totalmsgs;

bool batch = FALSE;

void AddScannedArea(MSGAREA &ma);
void ShowBusy();
void SelectAreas();
bool DownloadMailPacket();
void UploadMailPacket();
void UnZipMailPacket();
bool ZipMailPacket();
bool MakeDirectory( char *name );
bool RemoveDirectory( char *name );
void DeleteMailPacket();
bool CreateMessagesDat();
void CreateControlDat();
void CreateDoorId();
void ProcessReplies();
long BasicReal2Long(long InValue);
long Long2BasicReal(long InValue);
void ChangeConfiguration();
void ShowMenu();
bool CreateMailPacket();
void ChangeConfiguration();
String ProtocolName(char k);
void SelectProtocol();
void UpdateLastRead();

extern "C" int HangupFunction();

void
Capitalize(char *s)
{
   for(bool first = TRUE ; *s ; s++ , first = FALSE)
   {
      if(first || *(s-1) == ' ' || *(s-1) == '.')
         *s = toupper(*s);
      else
         *s = tolower(*s);
   }
}

void
ShowBusy()
{
   static int x = 0;
   static char s[] = "|/-\\";

   putchar('\b');
   putchar(s[x]);
   putchar(char(255));   // Flush output buffer

   x = ++x % 4;
}

void
SelectProtocol()
{
   PROTOCOL p;
   File f;
   String keys;

   printf("\n\f\n\2Select Default Protocol:\n\n");

   if(!f.open(FileName((char *)SysPath,"PROTOCOL.PRO")))
      return;

   for(;;)
   {
      if(f.read(&p,sizeof(p)) != sizeof(p))
         break;

      if(p.flags & PROT_ENABLED && (!(p.flags & PROT_LOCAL) || !BaudRate))
      {
         keys << p.key;
         printf("  \7<\3%c\7> \6%s\n",p.key,p.name);
      }
   }
   f.close();

   keys << '\r';

   printf("\n\n\7Select Protocol: ");

   char k = WaitKeys(keys);

   if(k != '\r')
      {
         CurUser->defaultProtocol = k;

         Log( LOG_NORMAL , "Protocol changed to '%c'" , k);

         printf("\3%s\n\n\7Press [\3Enter\7] to continue.\t",(char *)ProtocolName(k));
      }
}

String
ProtocolName(char k)
{
   PROTOCOL p;
   File f;

   if(!f.open(FileName((char *)SysPath,"PROTOCOL.PRO")))
      return "";

   for(;;)
   {
      if(f.read(&p,sizeof(p)) != sizeof(p))
         break;

      if(p.flags & PROT_ENABLED)
      {
         if(k == p.key)
            return p.name;
      }
   }

   return "";
}

void
SelectAreas()
{
   MenuFunction( MENU_COMBINED_SELECT , "");
}

bool
MailPacketExists()
{
   return !access(FileName((char *)WorkDir,(char *)Config->QWKid,".QWK"),0);
}

bool
DownloadMailPacket()
{
   if(!MailPacketExists())
   {
      printf("\n\n\1No mail packet has been created yet!\7\n\n"
             "Press [\3Enter\7] to continue.\t");

      return FALSE;
   }

   printf("\7\n\nStarting download...");

   MenuFunction(32,form("/F=%s%s.QWK /N /I /T /D /K=%c /Q /L=%s_QWK.LOG" , (char *)WorkDir,Config->QWKid,CurUser->defaultProtocol,(char *)WorkDir));

   File f;

   if(!f.open(FileName((char *)WorkDir,"_QWK.LOG")))
      return FALSE;

   if(f.readByte() == 'D')   // Quick & Dirty
   {
      f.close();

      Log(LOG_NORMAL , "QWK: Packet downloaded (%u msgs)" , totalmsgs);

      UpdateLastRead();

      return TRUE;
   }
   else
   {
      f.close();

      printf("\n\n\6Packet not downloaded."
             "\n\n\7Press [\3Enter\7] to continue.\t");

      return FALSE;
   }
}

void
UploadMailPacket()
{
   DeleteMailPacket();

   printf("\n\3Begin Upload Now...");

   MenuFunction(33,form("%s /N /I /T /D /Q /K=%c" , (char *)WorkDir , CurUser->defaultProtocol));

   if(access(FileName((char *)WorkDir,(char *)Config->QWKid,".REP"),0))
   {
      printf("\n\n\1No valid mail packet has been received!\7\n\n"
             "Press [\3Enter\7] to continue.\t");

      return;
   }

   UnZipMailPacket();

   ProcessReplies();
}

int
ArchiveType(char *fname)
{
   File f;
   char buf[10];

   if(f.open(fname , fmode_read))
   {
      for(int a = 0; arc[a].desc ; a++)
      {
         f.seek( arc[a].ident_start);
         f.read( buf , strlen(arc[a].ident) );
         buf[strlen(arc[a].ident)] = '\0';

         if(!strcmp(buf,arc[a].ident)) return a;
      }
   }

   return -1;
}

void
UnZipMailPacket()
{
   FileName zipfn((char *)WorkDir,(char *)Config->QWKid,".REP");

   int archiver = ArchiveType(zipfn);

   if(archiver < 0)
   {
      Log(LOG_NORMAL , "QWK: Unknown archive type uploaded");

      printf("\n\6Unknown archive type!\n\n\7Press [\3Enter\7] to continue.\t");

      return;
   }

   String unzipcmd = arc[archiver].decompr_cmd;

   unzipcmd.replace( "%a" , form("%s%s.REP" , (char *)WorkDir,Config->QWKid) );

   printf("\n\6Decompressing mail packet...");

   char drive = WorkDir[0];
   String dir = &WorkDir[2];

   dir.delLast('\\');

   File batch(FileName((char *)StartupPath,"_UNPACK.BAT") , fmode_create | fmode_text);
   batch.printf("@echo off\n");
   batch.printf("%c:\n",drive);
   batch.printf("cd %s\n",(char *)dir);
   batch.printf("%s\n",(char *)unzipcmd);
   batch.close();

   MenuFunction( MENU_SHELL , form("*W*Q*N*Z%s_UNPACK.BAT" , (char *)StartupPath) );

   unlink(FileName((char *)StartupPath,"_UNPACK.BAT"));

   printf("\3Done.");
}

bool
MakeDirectory( char *name )
{
   union REGS r;
   struct SREGS sr;

   r.x.ax = 0x3900;
   r.x.dx = FP_OFF(name);
   sr.ds  = FP_SEG(name);

   intdosx(&r,&r,&sr);

   return !(r.x.cflag);
}

bool
RemoveDirectory( char *name )
{
   union REGS r;
   struct SREGS sr;

   r.x.ax = 0x3A00;
   sr.ds  = FP_SEG(name);
   r.x.dx = FP_OFF(name);

   intdosx(&r,&r,&sr);

   return !(r.x.cflag);
}

void
DeleteMailPacket()
{
   FileName wc(WorkDir,"*.*");

   DirScan scan(wc);

   while(int(scan))
   {
      unlink(FileName(WorkDir,scan.name()));
      scan++;
   }
}

bool
CreateMessagesDat()
{
   String msgtext;
   MESSAGE msg;
   QWK_Header qwk;
   QWK_Index qwkidx;
   File f,fi,fp;
   long rec_no = 1;
   char *sparkware = "Produced by Qmail...Copyright (c) 1987 by Sparkware.  All Rights Reserved";

   while(NewLastRead.count())
   {
      NewLastRead.rewind();
      NewLastRead.remove();
   }

   if(!f.open(FileName(WorkDir,"MESSAGES.DAT") , fmode_create , IO_BUFSIZE))
      return FALSE;

   memset( &qwk , ' ' , sizeof(qwk) );
   memcpy( &qwk , sparkware , strlen(sparkware));

   f.write(&qwk , sizeof(qwk));
   rec_no++;

   if(batch)
      printf("\n\7");
   else
      printf("\n\f\n\7");

   printf("\3  Message Area                        Total   New   To You\n");
   printf("\7ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\n");

   char *buf    = new char[MSGTEXT_BUFSIZE+1];
   char *msgbuf = new char[MSGTEXT_BUFSIZE+1];

   bool getout = FALSE;

   totalmsgs = 0;

   for(int areanum = 1 ; areanum <= 1000 && !getout; areanum++)
   {
      if(CurUser->combinedBoards[(areanum-1) / 8] & (1 << ((areanum-1)%8)))
      {
         MSGAREA ma;

         if(!ReadMsgArea( areanum , &ma))
            continue;

         if(!CheckAccess(ma.readLevel,ma.readFlags))
            continue;

         printf("\5%-35.35s  ", ma.name);

         long lr     = GetLastRead( areanum , UserRecNr );
         long new_lr = 0;
         long high   = HighMsg( areanum );
         long total  = NumMsgs( areanum );
         long numnew = 0;
         long numtoyou = 0;

         printf("\6%5ld  \1*",total);

         if(lr >= high)
         {
            printf("\b\7    0      0\n");
            continue;
         }

         bool ok = FirstMessage(&msg,areanum,1,lr+1);

         while(ok)
         {
            if((msg.attr & MSGATTR_PRIVATE) && !CheckAccess(ma.sysopLevel,ma.sysopFlags))
            {
               if(stricmp(msg.to , CurUser->name))
               {
                  ShowBusy();

                  if(toupper(PeekChar()) == 'S')
                  {
                     getout = TRUE;
                     break;
                  }

                  ok = NextMessage(&msg,areanum,1);

                  continue;
               }
            }

            memset(&qwk,' ',128);

            if(msg.attr & MSGATTR_PRIVATE)
            {
               if(msg.attr & MSGATTR_RECEIVED)
                  qwk.status = '+';
               else
                  qwk.status = '*';
            }
            else
            {
               if(msg.attr & MSGATTR_RECEIVED)
                  qwk.status = '-';
            }

            strncpy(qwk.from,strupr(msg.from),25);
            strncpy(qwk.to,strupr(msg.to),25);
            strncpy(qwk.subj,msg.subj,25);

            strncpy(qwk.num , form("%-7ld",msg.num) , 7);

            strncpy(qwk.date , Date(msg.postDate[0],msg.postDate[1],msg.postDate[2]).format("MM-DD-YY") , 8);
            strncpy(qwk.time , Time(msg.postTime[0],msg.postTime[1],0).format("HH:MM") , 5);

            memset(qwk.pwd ,' ',12);
            memset(qwk.rnum,' ',8);

            if(msg.next)
               strncpy(qwk.rnum , form("%-8d",msg.next) , 8);

            qwk.active = byte(0xE1);
            qwk.area   = areanum;

            CreateMessageTextString(&msg,msgbuf,MSGTEXT_BUFSIZE);

            msgtext.clear();

            char *s = msgbuf;
            char *d = buf;

            for(s = msgbuf ; *s ; s++)
            {
               if(*s == '\r')
               {
                  *d++ = char(227);
               }
               else
               {
                  if(*s != '\n')
                     *d++ = *s;
               }
            }

            int len = int(d - buf);

            for(int i=0; i < ((128 - (len % 128)) % 128) ; i++) *d++ = ' ';
            *d = '\0';

            int num_blks = (len+255) / 128;

            strncpy(qwk.blks , form("%-6d" , num_blks) , 6);

            num_blks--;

            qwkidx.record = Long2BasicReal(rec_no);
            qwkidx.dummy  = 0;

            if(fi.opened() || fi.open(form("%s%03d.NDX",(char *)WorkDir,areanum),fmode_create , 1024))
            {
               fi.write(&qwkidx,sizeof(qwkidx));
            }

            if(!stricmp(msg.to , CurUser->name))
            {
               if(fp.opened() || fp.open(form("%sPERSONAL.NDX",(char *)WorkDir),fmode_create , 512))
               {
                  fp.write(&qwkidx,sizeof(qwkidx));
               }

               numtoyou++;
            }

            qwk.logicalnum = ++totalmsgs;

            f.write(&qwk,sizeof(qwk));
            rec_no++;

            s = buf;

            for(i=0;i<num_blks;i++ , s += 128 , rec_no++)
            {
               f.write(s,128);
            }

            numnew++;
            new_lr = msg.id;

            ShowBusy();

            if(toupper(PeekChar()) == 'S')
            {
               getout = TRUE;
               break;
            }

            if(numnew >= CurUser->qwkMaxMsgsPerArea)
               break;

            ok = NextMessage(&msg,areanum,1);
         }

         if(numnew)
         {
            NewLastRead.add(LastRead(areanum,new_lr));
         }

         fi.close();

         printf("\b\7%5ld  %c%5ld\n", numnew , (numtoyou? '\1':'\7') , numtoyou);
      }
   }

   fp.close();

   delete [] msgbuf;
   delete [] buf;

   return TRUE;
}

void
CreateControlDat()
{
   TextFile f;
   String username = CurUser->name;
   username.upperCase();

   if(!f.open(FileName(WorkDir,"CONTROL.DAT"), fmode_create)) return;

   f << (char *)Config->bbsname << '\n';
   f << (char *)Config->location << '\n';
   f << (char *)Config->phone << '\n';
   f << (char *)Config->sysopname << '\n';
   f << "00000," << (char *)Config->QWKid << '\n';
   f << Date(TODAY).format("MM-DD-CCYY") << ',';
   f << Time(NOW).format("HH:MM:00") << '\n';
   f << username << '\n';
   f << '\n';
   f << "0\n";
   f << form("%u\n",totalmsgs);

   String areas;

   for(int a = 1 , num_areas = 0; a <= 1000 && a <= NumMsgAreas() ; a++)
   {
      MSGAREA ma;

      if(!ReadMsgArea(a,&ma))
         continue;

      if(!CheckAccess(ma.readLevel,ma.readFlags))
         continue;

      String name;

      if(ma.qwkTag[0])
         name = ma.qwkTag;
      else
      if(ma.echoTag[0])
         name = ma.echoTag;
      else
      if(strlen(ma.name) <= 13)
         name = ma.name;
      else
         name = form("%13.13s",ma.name);

      areas << form("%d\n%s\n" , a , (char *)name);
      num_areas++;
   }

   f.printf("%d\n",num_areas-1);
   f << areas;
   f << "\n\n\n";
}

void
CreateDoorId()
{
   TextFile f;

   if(!f.open(FileName(WorkDir,"DOOR.ID"),fmode_create)) return;

   f << "DOOR = ProBoard\n"
        "VERSION = 2.00\n"
        "SYSTEM = ProBoard\n"
        "CONTROLNAME = PROBOARD\n"
        "CONTROLTYPE = ADD\n"
        "CONTROLTYPE = DROP\n"
        "MIXEDCASE = YES\n"
        "FIDOTAG = YES\n";
}

void
ProcessReplies()
{
   QWK_Header qwk;

   printf("\n\n\7Unpacking reply packet...\n\n");

   File f(FileName(WorkDir , (char *)Config->QWKid , ".MSG"),fmode_read, 2048);

   if(!f.opened())
   {
      printf("\1Mail packet not found!\n\n\7Press [\3Enter\7] to continue.\t");
      return;
   }

   char id[9];
   id[strlen(Config->QWKid)] = '\0';

   f.read(id,strlen(Config->QWKid));

   if(String(id) != String(Config->QWKid))
   {
      Log(LOG_NORMAL , "QWK: Invalid mail packet uploaded");

      printf("\1Invalid mail packet!\n\n\7Press [\3Enter\7] to continue.\t");
      return;
   }

   f.seek(128);

   for(int msgnum = 1;;msgnum++)
   {
      MSGAREA ma;
      String from,to,subj,text;
      int area;
      bool pvt;
      int num_blks;

      if(f.read(&qwk , 128) != 128) break;

      if(qwk.time[2] != ':' || qwk.date[2] != '-')
      {
         Log(LOG_NORMAL , "QWK: Bad (scrambled) reply packet");

         printf("\1Bad reply packet!\n");
         break;
      }

      qwk.active = '\0';

      num_blks = atoi(qwk.blks) - 1;

      if(num_blks < 1 || num_blks > 256) break;  // Something wrong with packet

      qwk.pwd [0] = '\0';  subj = qwk.subj;
      qwk.subj[0] = '\0';  from = qwk.from;
      qwk.from[0] = '\0';  to   = qwk.to;

      from.trim();
      to  .trim();
      subj.trim();

      area = qwk.area;

      for(int i=0;i<num_blks;i++)
      {
         char s[129];
         s[128] = '\0';

         if(f.read(s,128) != 128) break;

         text << s;
      }

      text.rtrim();
      text.replace("ã","\n");

      printf("\6Msg #\3%-3d\6 : ",msgnum);

      if(!ReadMsgArea( area , &ma))
      {
         printf("\1Unknown area %d!\n");

         Log(0,"QWK: Unknown area %d in reply packet" , area);

         continue;
      }

      if(!CheckAccess(ma.writeLevel,ma.writeFlags))
      {
         printf("\1No write access in area %d (%s) !\n", area, ma.name);

         continue;
      }

      if(ma.msgType == MSG_PVT)
         pvt = TRUE;
      else
         pvt = FALSE;

      if(ma.msgType == MSG_BOTH)
         pvt = (qwk.status == '*' || qwk.status == '+');

      WriteMSGTMP( text );

      Capitalize(from);
      Capitalize(to);

      if(to == "ProBoard")
      {
         printf("\1Control message (not supported yet)\n");
         continue;
      }

      PostMessage( from , to , subj , area , pvt );

      printf("\7To '\3%s\7' (Area \3%s\7)%s\n",(char *)to , ma.name , pvt ? " (\2PVT\7)":"");

      Log( 0 , "QWK: Msg to '%s' in area %d%s",(char *)to,area,pvt ? " (PVT)":"");
   }

   printf("\n\7Press [\3Enter\7] to continue.\t");
}


long
BasicReal2Long(long InValue)
{
   long Temp;
   int  Expon;
   long RetVal;

   Expon = int(((InValue >> 24) & 0x000000ffL) - 152);

   Temp  = (InValue & 0x007FFFFFL) | 0x00800000L;

   if(Expon < 0)
      Temp = Temp >> (-Expon);
   else
      Temp = Temp << Expon;

   if(InValue & 0x00800000L)
      RetVal = -Temp;
   else
      RetVal = Temp;

   if(!Expon) RetVal = 0;

   return RetVal;
}

long
Long2BasicReal(long InValue)
{
   bool Negative;
   long Expon;

   if(!InValue) return 0;

   if(InValue < 0)
   {
      Negative = TRUE;
      InValue  = -InValue;
   }
   else
   {
      Negative = FALSE;
   }

   Expon = 152;

   if(InValue < 0x007FFFFFL)
   {
      while(!(InValue & 0x00800000L))
      {
         InValue = InValue << 1;
         Expon--;
      }
   }
   else
   {
      while(InValue & 0xFF000000L)
      {
         InValue = InValue >> 1;
         Expon++;
      }
   }

   InValue = InValue & 0x007FFFFFL;

   if(Negative) InValue = InValue | 0x00800000L;

   return  (InValue + (Expon << 24));
}

bool
ZipMailPacket()
{
   String zipcmd = arc[CurUser->qwkArchiver-1].compr_cmd;

   zipcmd.replace( "%a" , form("%s.QWK" , Config->QWKid) );
   zipcmd.replace( "%f" , "*.*");

   printf("\n\6Compressing mail packet...");

   char drive = WorkDir[0];
   String dir = &WorkDir[2];

   dir.delLast('\\');

   File batch(FileName((char *)StartupPath,"_PACKUP.BAT") , fmode_create | fmode_text);
   batch.printf("@echo off\n");
   batch.printf("%c:\n",drive);
   batch.printf("cd %s\n",(char *)dir);
   batch.printf("%s\n",(char *)zipcmd);
   batch.close();

   MenuFunction( MENU_SHELL , form("*W*Q*N*Z%s_PACKUP.BAT" , (char *)StartupPath) );

   unlink(FileName((char *)StartupPath,"_PACKUP.BAT"));

   if(!MailPacketExists())
   {
      Log(LOG_FRIEND,"QWK: Error compressing mail!",(char *)zipcmd);

      printf("\1Error!");
      printf("\n\n\7Press [\3Enter\7] to continue.\t");

      return FALSE;
   }
   else
   {
      printf("\3Done.\n\n");

      return TRUE;
   }
}

void
UpdateLastRead()
{
   printf("\7\n\nUpdating lastread pointers...");

   NewLastRead.rewind();

   for( ; !NewLastRead.eol() ; NewLastRead++)
   {
      SetLastRead( NewLastRead.get().area , UserRecNr , NewLastRead.get().lr);
   }

   printf("\2Done\n\n");
}

void
ShowTitle()
{
   printf("\n\f\n\7");
   printf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n");
   printf("³\7 °°°°°°°°°°°°°°°°  \5ProBoard QWK Offline Mail Packer v2.10\7  °°°°°°°°°°°°°°°° \7³\n");
   printf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n");
   printf("\n");
}

void
ShowMenu()
{
   ShowTitle();

   if(MailPacketExists())
      printf("                           \7(\2Mail Packet Created\7)\n\n");

   printf("                         \7[\3P\7] \6Create Mail Packet\n");
   printf("                         \7[\3K\7] \6Kill Mail Packet\n");
   printf("\n");
   printf("                         \7[\3D\7] \6Download Mail Packet\n");
   printf("                         \7[\3U\7] \6Upload Reply Packet\n");
   printf("\n");
   printf("                         \7[\3C\7] \6Configuration / Select Areas\n");
   printf("\n");
   printf("                         \7[\3X\7] \1EXIT\n");

   printf("\n\n\7Make Your Choice: ");
}

bool
CreateMailPacket()
{
   if(CreateMessagesDat())
   {
      CreateDoorId();
      CreateControlDat();
      if(ZipMailPacket())
         return TRUE;
   }

   return FALSE;
}

void
ChangeConfiguration()
{
   char k = '\0';

   while( k != '-')
   {
      printf("\n\f\n\7");
      printf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n");
      printf("³\7 °°°°°°°°°°°°°°°°°°°°°°  \5ProBoard QWK Configuration\7  °°°°°°°°°°°°°°°°°°°°°° \7³\n");
      printf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n");
      printf("\n");
      printf("                       \7[\3A\7] \6Archiver (\3%s\6)\n" , arc[CurUser->qwkArchiver-1].desc);
      printf("                       \7[\3P\7] \6Protocol (\3%s\6)\n" , (char *)ProtocolName(CurUser->defaultProtocol));
      printf("                       \7[\3S\7] \6Select Areas\n" );
      printf("                       \7[\3G\7] \6Select/Unselect All Areas\n");
      printf("                       \7[\3M\7] \6Maximum # messages per area (\3%u\6)\n",CurUser->qwkMaxMsgsPerArea);
      printf("\n");
      printf("                       \7[\3-\7] \1MAIN MENU\n");

      printf("\n\n\7Select: ");

      k = WaitKeys("\r-APGSM");

      if(k != '\r') printf("\6%c\n",k);

      if(k == 'P')
         SelectProtocol();

      if(k == 'S')
         SelectAreas();

      if(k == 'G')
         MenuFunction( MENU_COMBINED_CLEAR , "");

      if(k == 'A')
      {
         printf("\n\f\n\2Available Archivers:\n\n");

         for(int a = 0 ; arc[a].desc ; a++)
            printf("    \7[\3%d\7] \6%s\n",a+1,arc[a].desc);

         printf("\n\7Select Archiver: ");

         char new_a = WaitKeys("123\r\x1b");

         printf("\3%c",new_a);

         if(isdigit(new_a))
            CurUser->qwkArchiver = new_a - '0';
      }

      if(k == 'M')
      {
         char buf[5];

         printf("\n\n\2Maximum # messages per area: ");

         Input(buf,4,INPUT_DIGITS);

         if(buf[0] && atoi(buf) > 0)
         {
            CurUser->qwkMaxMsgsPerArea = atoi(buf);
         }
      }
   }
}

void
main(int argc,char *argv[])
{
   bool download_mode = FALSE,
        upload_mode   = FALSE,
        success       = FALSE;

   if(CurUser->qwkArchiver == 0)
      CurUser->qwkArchiver = 1;

   if(CurUser->defaultProtocol == 0)
      CurUser->defaultProtocol = 'Z';

   if(CurUser->qwkMaxMsgsPerArea == 0)
      CurUser->qwkMaxMsgsPerArea = 100;

   if(CurUser->qwkMaxMsgs == 0)
      CurUser->qwkMaxMsgs = 500;

   if(argc > 1)
   {
      if(toupper(argv[1][0]) == 'D')
      {
         download_mode = TRUE;
         batch         = TRUE;
      }

      if(toupper(argv[1][0]) == 'U')
      {
         upload_mode = TRUE;
         batch       = TRUE;
      }
   }

   WorkDir = form("%s%s", StartupPath , "QWK_WORK") ;

   if(batch)
      ShowTitle();
   else
      ShowMenu();

   MakeDirectory( WorkDir );

   WorkDir.appendBS();

   DeleteMailPacket();

   InstallHandler(HANDLER_HANGUP , HangupFunction);

   if(download_mode)
   {
      if(CreateMailPacket() == TRUE)
         if(DownloadMailPacket() == TRUE)
            success = TRUE;
   }

   while(!download_mode && !upload_mode)
   {
      byte key = WaitKeys("SPKDUCX\r");

      putchar( key );

      switch(key)
      {
         case 'K': DeleteMailPacket();
                   break;
         case 'P': if(CreateMailPacket())
                   {
                     printf("\7Download now? \6[Y/n] \3");
                     char k = WaitKeys("YN\r");
                     switch(k)
                     {
                        case '\r':
                        case 'Y' : printf("Yes");
                                   success = DownloadMailPacket();
                                   break;
                        case 'N' : printf("No");
                                   break;
                     }
                   }
                   break;
         case 'D': if(DownloadMailPacket())
                      success = TRUE;
                   break;
         case 'U': UploadMailPacket();
                   break;
         case 'C': ChangeConfiguration();
                   break;
      }

      if(key == 'X') break;

      ShowMenu();
   }

   DeleteMailPacket();

   WorkDir.delLast('\\');

   RemoveDirectory( WorkDir );

   //if(success)
   //   UpdateLastRead();
}

extern "C"
int
HangupFunction()
{
   DeleteMailPacket();
   WorkDir.delLast('\\');
   RemoveDirectory( WorkDir );

   return HANDLED;
}

