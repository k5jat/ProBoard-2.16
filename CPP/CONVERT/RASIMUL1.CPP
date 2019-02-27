#define NO_RA1

#include <stdio.h>
#include <tslib.hpp>
#include "convert.hpp"
#include "ra1.hpp"
#include "pb.hpp"

bool
simul_files_ra(bool unlimited)
{
 RA1_FileArea ra_fa;
 _FileArea pb_fa;

 File fi,fo;

 if(!fi.open(FileName(syspath,"FILECFG.PRO")))
   {
    printf("Can't open FILECFG.PRO");
    return FALSE;
   }

 fo.open("FILES.RA",fmode_create);

 for(int rec=0; rec < 200 || unlimited;rec++)
   {
    if(fi.read(&pb_fa,sizeof(pb_fa)) != sizeof(pb_fa)) break;

    CLEAR_OBJECT(ra_fa);

    pb_fa.name[30]     = '\0';
    pb_fa.filepath[40] = '\0';

    strcpy(ra_fa.Name,pb_fa.name);
    strcpy(ra_fa.FilePath,pb_fa.filepath);

    c2pas(ra_fa.Name);
    c2pas(ra_fa.FilePath);

    ra_fa.Flags = swapbits(pb_fa.flags);
    ra_fa.Level = pb_fa.level;

    fo.write(&ra_fa,sizeof(ra_fa));
   }

for(;rec<200;rec++)
  {
   CLEAR_OBJECT(ra_fa);
   fo.write(&ra_fa,sizeof(ra_fa));
  }

return TRUE;
}

bool
simul_messages_ra()
{
 RA1_MsgArea ra_ma;
 _MsgArea pb_ma;

 File fi,fo;

 if(!fi.open(FileName(syspath,"MESSAGES.PB")))
   {
    printf("Can't open MESSAGES.PB");
    return FALSE;
   }

 fo.open("MESSAGES.RA",fmode_create);

 for(int rec=0;rec<200;rec++)
   {
    if(fi.read(&pb_ma,sizeof(pb_ma)) != sizeof(pb_ma)) break;

    CLEAR_OBJECT(ra_ma);

    if(!pb_ma.msgBaseType)
      {
       strcpy(ra_ma.Name,pb_ma.name);
       strcpy(ra_ma.OriginLine,pb_ma.origin);

       c2pas(ra_ma.Name);
       c2pas(ra_ma.OriginLine);

       ra_ma.ReadFlags  = swapbits(*((long *)&pb_ma.readFlags)  );
       ra_ma.WriteFlags = swapbits(*((long *)&pb_ma.writeFlags) );
       ra_ma.SysopFlags = swapbits(*((long *)&pb_ma.sysopFlags) );
       ra_ma.ReadLevel  = pb_ma.readLevel;
       ra_ma.WriteLevel = pb_ma.writeLevel;
       ra_ma.SysopLevel = pb_ma.sysopLevel;

       ra_ma.RecvKill  = pb_ma.rcvKillDays;
       ra_ma.DaysKill  = pb_ma.msgKillDays;
       ra_ma.CountKill = pb_ma.maxMsgs;

       ra_ma.Kind      = pb_ma.msgType;
       ra_ma.Type      = pb_ma.msgKind;

       ra_ma.AkaAddress = pb_ma.aka;
      }

    ra_ma.Attribute = 2;
    if(ra_ma.Type>=2) ra_ma.Attribute |= 1;

    fo.write(&ra_ma,sizeof(ra_ma));
   }

for(;rec<200;rec++)
  {
   CLEAR_OBJECT(ra_ma);
   fo.write(&ra_ma,sizeof(ra_ma));
  }

return TRUE;
}

bool
simul_config_ra()
{
 File f;

 static _ModemConfig modem_cfg;

/*
struct
  {
  unsigned baud;
  int port;
  int quiet;
  int blanktime;
  char initstring[70];
  char busystring[70];
  char initok[40];
  char busyok[40];
  char connect300[40];
  char connect1200[40];
  char connect2400[40];
  char connect4800[40];
  char connect9600[40];
  char connect19200[40];
  char connect38400[40];
  char ring[40];
  char answer[40];
  char connect7200[40];
  char connect12000[40];
  int forceanswer;
  char extra2[4];
  int delay;
  } modem_cfg;
*/

 if(!f.open(FileName(syspath,"MODEM.PB")))
   {
    printf("Can't open MODEM.PB");
    return FALSE;
   }

 if(f.read(&modem_cfg,sizeof(modem_cfg)) != sizeof(modem_cfg))
   {
    printf("MODEM.PB read error");
    return FALSE;
   }

 f.close();

 RA1_Config *racfg = new RA1_Config;
 CLEAR_OBJECT(*racfg);

 if(f.open(FileName(syspath,"AKA.PRO")))
   {
    for(int i=0;;i++) if(f.read(&racfg->Address[i],8) != 8) break;
    f.close();
   }

 f.open("CONFIG.RA",fmode_create);

 racfg->VersionID = 0x110;
 racfg->CommPort = modem_cfg.port;
 racfg->Baud = modem_cfg.maxBps;
 racfg->InitTries = 3;
 racfg->AnswerPhone = !!(modem_cfg.flags & MODEM_MANUAL_ANSWER);
 racfg->FlushBuffer = TRUE;
 racfg->MinimumBaud = 300;
 racfg->GraphicsBaud = 300;
 racfg->TransferBaud = 300;
 racfg->NewSecurity = 5;
 racfg->DirectWrite = TRUE;
 racfg->UserTimeOut = 240;
 racfg->LogonTime = 10;
 racfg->PasswordTries = 3;
 racfg->MaxPage = 3;
 racfg->PageLength = 23;
 racfg->CheckMail = TRUE;
 racfg->AskVoicePhone = TRUE;
 racfg->AskDataPhone = TRUE;
 racfg->ShowFileDates = TRUE;
 racfg->ANSI = TRUE;
 racfg->ClearScreen = TRUE;
 racfg->MorePrompt = 2;
 racfg->KillSent = TRUE;
 racfg->CrashAskSec = 32000;
 racfg->CrashSec = 32000;
 racfg->FAttachSec = 32000;
 racfg->StatFore  = 7;
 racfg->HiFore    = 7;
 racfg->WindFore  = 7;
 racfg->MinPwdLen = 3;
 racfg->HotKeys = TRUE;
 racfg->BorderFore = 7;
 racfg->BarFore = 7;
 racfg->PwdBoard = 1;
 racfg->BufferSize = 0;
 racfg->ShowMissingFiles = TRUE;
 racfg->RenumThreshold = 20000;
 racfg->LeftBracket = '(';
 racfg->RightBracket = ')';
 racfg->MultiLine = (cfg.multiline || cfg.numnodes>1) ? TRUE:FALSE;

 strcpy(racfg->MissingString,"MISSING");               c2pas(racfg->MissingString);
 strcpy(racfg->LogonPrompt,"Enter your full name: ");  c2pas(racfg->LogonPrompt);
 strcpy(racfg->ReplyHeader,"");                        c2pas(racfg->ReplyHeader);
 strcpy(racfg->ErrorFreeString,"ARQ");                 c2pas(racfg->ErrorFreeString);
 strcpy(racfg->InitStr,modem_cfg.cmdInit1);            c2pas(racfg->InitStr);
 strcpy(racfg->BusyStr,modem_cfg.cmdOffHook);          c2pas(racfg->BusyStr);
 strcpy(racfg->InitResp,modem_cfg.msgOk);              c2pas(racfg->InitResp);
 strcpy(racfg->BusyResp,modem_cfg.msgOk);              c2pas(racfg->BusyResp);
 strcpy(racfg->Connect300,modem_cfg.msgCon300);        c2pas(racfg->Connect300);
 strcpy(racfg->Connect1200,modem_cfg.msgCon1200);      c2pas(racfg->Connect1200);
 strcpy(racfg->Connect2400,modem_cfg.msgCon2400);      c2pas(racfg->Connect2400);
 strcpy(racfg->Connect4800,modem_cfg.msgCon4800);      c2pas(racfg->Connect4800);
 strcpy(racfg->Connect9600,modem_cfg.msgCon9600);      c2pas(racfg->Connect9600);
 strcpy(racfg->Connect19k,modem_cfg.msgCon19200);      c2pas(racfg->Connect19k);
 strcpy(racfg->Connect38k,modem_cfg.msgCon38400);      c2pas(racfg->Connect38k);
 strcpy(racfg->Ring,modem_cfg.msgRing);                c2pas(racfg->Ring);
 strcpy(racfg->AnswerStr,modem_cfg.cmdAnswer);         c2pas(racfg->AnswerStr);

 strcpy(racfg->MenuPath,cfg.mnupath);                  c2pas(racfg->MenuPath);
 strcpy(racfg->TextPath,cfg.txtpath);                  c2pas(racfg->TextPath);
 strcpy(racfg->AttachPath,cfg.pvtuploadpath);          c2pas(racfg->AttachPath);
 strcpy(racfg->NodelistPath,cfg.nodelistdir);          c2pas(racfg->NodelistPath);
 strcpy(racfg->MsgBasePath,cfg.msgpath);               c2pas(racfg->MsgBasePath);
 strcpy(racfg->SysPath,syspath);                       c2pas(racfg->SysPath);
 strcpy(racfg->ExternalEdCmd,cfg.editorname);          c2pas(racfg->ExternalEdCmd);

 strcpy(racfg->OriginLine,cfg.originline);             c2pas(racfg->OriginLine);
 strcpy(racfg->QuoteString," > ");                     c2pas(racfg->QuoteString);
 strcpy(racfg->Sysop,cfg.sysopname);                   c2pas(racfg->Sysop);
 strcpy(racfg->LogFileName,FileName(syspath,"PROBOARD.LOG")); c2pas(racfg->LogFileName);

 strcpy(racfg->SystemName,cfg.bbsname);                c2pas(racfg->SystemName);
 /*
    byte ProtocolAttrib[6];
    byte DefaultCombined[25];

    char LocationPrompt[61];
    char PagePrompt[61];
    char UserfilePrompt[41];
    char Location[41];
    char CRprompt[41];
    char ContinuePrompt[41];
    char ListPath[61];

   char SlowBaudTimeStart[6];
   char SlowBaudTimeEnd[6];
   char DownloadTimeStart[6];
   char DownloadTimeEnd[6];
   char PagingTimeStart[6];
   char PagingTimeEnd[6];
   char LoadingMsg[71];
   char ListPrompt[71];

   int Address[40];
   char SystemName[31];
 */

 f.write(racfg,sizeof(*racfg));
 f.close();

 return TRUE;
}
