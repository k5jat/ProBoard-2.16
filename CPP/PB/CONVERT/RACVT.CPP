
/* ------------------------------------------------------------
 * Filename ............... RaCvt.Cpp
 *
 * General Purpose ........ Convert RA files
 * ------------------------------------------------------------
 * First date ............. 23 jan 1994
 *
 * First in version ....... 2.00
 *
 * Written by ............. P. Leybaert / A. Schellinck
 * ------------------------------------------------------------
 * Revisions:
 * ----------
 *
 *   Date   | By |                  Purpose                   |
 * ---------+----+--------------------------------------------+
 *          |    |                                            |
 *          |    |                                            |
 *          |    |                                            |
 */

/*-------------------------------------------------------------------------*/

#define NO_RA1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <tslib.hpp>
#include "convert.hpp"
#include "ra1.hpp"
#include "ra2.hpp"
#include "pb.hpp"

/*-------------------------------------------------------------------------*/

char ra_mnu_path[61];
char ra_msg_path[61];
char ra_path    [61];

/*-------------------------------------------------------------------------*/

void     parse_data     (  char *dataline                               );
void     check_func     (  struct menuitem *t                           );

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : read_ra_paths()
 * Purpose   : Read the RA path's from the RA configuration record
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : -1 on ERROR, 0 on OK
 *
 */

static int read_ra_paths()
{
   File f;
   FileName fn(ra_path,"CONFIG.RA");

   if(!f.open(fn))
   {
      printf("Can't open %s\n",(char *)fn);
      return -1;
   }

   f.seek(751);
   f.read(ra_mnu_path,61);

   f.seek(183,SEEK_CUR);
   f.read(ra_msg_path,61);

   f.close();

   pas2c(ra_mnu_path);
   pas2c(ra_msg_path);

   append_backspace(ra_msg_path);
   append_backspace(ra_mnu_path);

   return 0;
}


/*
 * Routine   : swapbits()
 * Purpose   :
 * ------------------------------------------------------------------------
 * Parameters: original value
 * Return    : value with bits swapped
 *
 */

long swapbits(long x)
{
   long y = 0;

   for(int i= 0; i < 32; i++)
      if(TESTBIT(x,long(i)))
         SETBIT(y,long(31-i));

   return y;
}

/*
 * Routine   : cvtcolor()
 * Purpose   : convert the RA color to PB color
 * ------------------------------------------------------------------------
 * Parameters: foreground and background colors
 * Return    : byte with the FG and BG color merged
 *
 */

byte cvtcolor(byte fg,byte bg)
{
   char colors[] =
   {
      0,4,2,6,1,5,3,7
   };

   return ((fg&8) | colors[fg&7] | (colors[bg&7] << 4)) ^ 8;
}


/*
 * Routine   : convert_ra_msgs()
 * Purpose   : convert the RA message file
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : -1 on ERROR, 0 on OK
 *
 */

int convert_ra_msgs()
{
   RA2_MsgArea ra_ma;
   _MsgArea pb_ma;

   File fi,fo;

   FileName fn(ra_path,"MESSAGES.RA");
   if(!fi.open(fn))
   {
      printf("Can't open %s",(char *)fn);
      return -1;
   }

   fo.open(FileName(syspath,"MESSAGES.PB"),fmode_create);

   for(int num = 1;;num++)
   {
      if(fi.read(&ra_ma,sizeof(ra_ma)) != sizeof(ra_ma))
         break;

      pas2c(ra_ma.Name);
      pas2c(ra_ma.OriginLine);
      pas2c(ra_ma.JAMbase);

      memset(&pb_ma,0,sizeof(pb_ma));

      pb_ma.areaNum = num;

      ra_ma.Name[30] = '\0';

      strcpy(pb_ma.name,ra_ma.Name);
      strcpy(pb_ma.origin,ra_ma.OriginLine);

      *((long *)&pb_ma.readFlags)  = swapbits(ra_ma.ReadFlags);
      *((long *)&pb_ma.writeFlags) = swapbits(ra_ma.WriteFlags);
      *((long *)&pb_ma.sysopFlags) = swapbits(ra_ma.SysopFlags);

      *((long *)&pb_ma.readFlagsNot)  = swapbits(ra_ma.ReadNotFlags);
      *((long *)&pb_ma.writeFlagsNot) = swapbits(ra_ma.WriteNotFlags);
      *((long *)&pb_ma.sysopFlagsNot) = swapbits(ra_ma.SysopNotFlags);

      pb_ma.readLevel  = ra_ma.ReadSecurity;
      pb_ma.writeLevel = ra_ma.WriteSecurity;
      pb_ma.sysopLevel = ra_ma.SysopSecurity;

      pb_ma.rcvKillDays = ra_ma.RecvKill;
      pb_ma.msgKillDays = ra_ma.DaysKill;
      pb_ma.maxMsgs     = ra_ma.CountKill;

      pb_ma.msgType       = ra_ma.MsgKinds;
      pb_ma.msgKind       = (ra_ma.Typ > 2) ? 2 : ra_ma.Typ;

      pb_ma.msgBaseType = (ra_ma.Attribute & byte(0x80)) ? MSGBASE_JAM:MSGBASE_HUDSON;
      strcpy(pb_ma.path,ra_ma.JAMbase);
      strupr(pb_ma.path);

      pb_ma.groups[0] = ra_ma.Group;
      pb_ma.groups[1] = ra_ma.AltGroup[0];
      pb_ma.groups[2] = ra_ma.AltGroup[1];
      pb_ma.groups[3] = ra_ma.AltGroup[2];

      pb_ma.allGroups = !!(ra_ma.Attribute2 & 1);

      pb_ma.minAge = ra_ma.Age;

      pb_ma.aka    = ra_ma.AkaAddress;

      fo.write(&pb_ma,sizeof(pb_ma));
   }

   return 0;
}


/*
 * Routine   : convert_ra_files()
 * Purpose   : convert the RA files
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

int
convert_ra_files()
{
   RA2_FileArea ra_ma;
   _FileArea pb_ma;

   File fi,fo;

   FileName fn(ra_path,"FILES.RA");
   if(!fi.open(fn))
   {
      printf("Can't open %s",(char *)fn);
      return -1;
   }

   fo.open(FileName(syspath,"FILECFG.PRO"),fmode_create);

   for(;;)
   {
      if(fi.read(&ra_ma,sizeof(ra_ma)) != sizeof(ra_ma))
         break;

      pas2c(ra_ma.Name);
      pas2c(ra_ma.FilePath);

      memset(&pb_ma,0,sizeof(pb_ma));

      strcpy(pb_ma.name,ra_ma.Name);
      strcpy(pb_ma.filepath,ra_ma.FilePath);

      append_backspace(pb_ma.filepath);

      strcpy(pb_ma.listpath,pb_ma.filepath);
      strcat(pb_ma.listpath,"FILES.BBS");

      pb_ma.flags    = swapbits(ra_ma.Flags);
      pb_ma.flagsNot = swapbits(ra_ma.NotFlags);

      pb_ma.level = ra_ma.Security;

      if(ra_ma.Attrib & 0x10)
      pb_ma.free = TRUE;

      if(ra_ma.Attrib & 0x08)
      pb_ma.cdrom = TRUE;

      pb_ma.allGroups = !!(ra_ma.Attrib2 & 1);

      fo.write(&pb_ma,sizeof(pb_ma));
   }

   return 0;
}


/*
 * Routine   : checkData()
 * Purpose   : check the dataline for each function
 * ------------------------------------------------------------------------
 * Parameters: RA function, new PB function, dataline
 * Return    : new dataline
 *
 */

String checkData(int raFunc, int pbFunc, char *data)
{
   static char newData[256];

   CLEAR_OBJECT(newData);
   strcpy(newData, data);

   switch(pbFunc)
   {
      case 1:  case 2:  case 3:  case 4:     // menu functions
      {
         char *tmp;                          // /P bijvoegen voor paswoord

         tmp = strtok(newData, " ");
         tmp = strtok(NULL   , " ");

         if(tmp != NULL)
         {
               tmp[strlen(tmp)] = ' ';
               if(strnicmp(tmp, "/F", 2) && strnicmp(tmp, "/M", 2))
               {
                  memmove(tmp + 3, tmp, strlen(tmp) + 1);
                  strncpy(tmp - 1, " /P=", 4);
               }
         }

         for(int cnt = 0; newData[cnt] != 0; cnt++)
         {
            if(newData[cnt] == '<')
               newData[cnt] = '-';

            if(newData[cnt] == '>')
               newData[cnt] = '+';
         }
      }
      break;

      case 5: case 12: case 15: case 40: case 45:
      {                                // remove all '/xxx' parameters
         for(int cnt = 0; newData[cnt] != 0; cnt++)
         {
            if(newData[cnt] == '/')
               newData[cnt] = '\0';
         }
      }
      break;

      case 7:
      {
         String line("");
         int size = strlen(newData);
         int cnt = 0;
         while(cnt < size)
         {
            if(newData[cnt] == '*')
            {
               char t = 0;

               switch(toupper(newData[cnt + 1]))
               {
                  case 'D':   case 'U':   case '#':
                  {
                     t = 0;
                     cnt++;
                  }
                  break;

                  case 'O': case 'S':
                  {
                     while(newData[cnt + 1] != '*' && newData[cnt + 1] != ' ' && newData[cnt + 1] != 0)
                        cnt++;
                  }
                  break;

                  case 'M':
                  {
                     t = 'X';
                     cnt++;
                  }
                  break;

                  case 'N':
                  {
                     t = '#';
                     cnt++;
                  }
                  break;

                  case '0':
                  {
                     t = 0;
                     cnt++;
                     line << "@<CURFILEAREADIR>@ ";
                  }
                  break;

                  case '1':
                  {
                     t = 0;
                     cnt++;
                     line << "@<CURMSGAREA#>@ ";
                  }
                  break;

                  default:
                  {
                     t = newData[cnt + 1];
                     cnt++;
                  }
                  break;
               }

               if(t != 0)
                  line << '*' << t;
            }
            else
               line << newData[cnt];

            cnt++;
         }

         strcpy(newData, (char *)line);
      }
      break;

      case 22:
      {
         strcpy(newData, "/M");
      }
      break;

      case 23: case 24: case 25:
      {
         String line(newData);
         line.replace("/M","*");

         strcpy(newData, (char *)line);
      }
      break;

      case 27:
      {
         String line;

         for( char *tmp = strtok( newData, " ") ; tmp ; tmp = strtok(NULL," "))
         {
            String part = tmp;

            if(!strnicmp(part,"/T",2))
            {
               part.replace("/T=","/T=\"");
               part << '\"';
               part.replace("_"," ");
            }

            if(part == "/U" || part == "/H")
               continue;

            if(!strnicmp(part,"/N",2))
               continue;

            line << part << ' ';
         }

         line.trim();

         line.replace("/M","*");

         strcpy(newData,line);
      }
      break;

      case 35: case 36: case 37: case 38:
      {
         String line(newData);

         line.replace("/T","");
         line.trim();
         if(line.find("/FG") < 0)
            if(line.find("/F") < 0)
               line = "*";
            else
               line = "X";

         strcpy(newData, line);
      }

      case 31: case 34:
      {
         String line(newData);
         line.replace("/T","");
         line.trim();
         line.replace("/F","X");

         strcpy(newData, (char *)line);
      }
      break;

      case 32:
      {
         String line(newData);
         line.replace("/F","X");

         strcpy(newData, (char *)line);

         switch(raFunc)
         {
            case 48:
            {
               String line("/F=");
               line << newData;
               strcpy(newData, (char *)line);
            }
            break;

            case 55:
            {
               strcpy(newData,"/A");
            }
            break;

            case 59:
            {
               strcat(newData," *");
            }
            break;

         }
      }
      break;

      case 49:
      {
         String line;

         line << "* " << newData;
         line.replace("/1","");

         strcpy(newData,line);
      }
      break;

      case 51:
      {
         strcpy(newData,"10");
      }
      break;

      case 54:
      {
         String line;

         line << "* " << newData;
         line.replace("/1","");

         strcpy(newData,line);
      }
      break;

      case 39: case 61:
      {
      }
      break;

      default:
      {                                // no parameters for these functions
         CLEAR_OBJECT(newData);
      }
   }

   String tmp(newData);

   tmp.trim();

   return tmp;
}

/*
 * Routine   : convert_ra_menus()
 * Purpose   : convert the remote access menus
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : -1 on ERROR, 0 on OK
 *
 */

int convert_ra_menus()
{
   char pbFunction[] =
   {
       0,    1,    2,    3,    4,    5,   -1,    7,    8,    9,
      10,   11,   12,   13,   14,   -1,   16,   17,   18,   19,
      20,   21,   22,   23,   24,   25,   -1,   27,   28,   -1,
      30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
      40,   41,   42,   -1,   -1,   45,   -1,   47,   32,   49,
      54,   51,   50,   -1,   -1,   32,   -1,   57,   58,   32,
      59,   62,   41,   69,   70,   46,   -1,   -1,   -1,   -1,
      -1,   68,   -1,   -1,   71
   };

   struct menuitem
   {
      byte  type;             // function to execute
      word  minSec;           // minimum required security level
      word  maxSec;           // maximum allowed security level
      long  offFlags;         // flags wich must be disabled
      long  onFlags;          // flags wich must be enabled
      word  timeLeft;         // amount of time required
      word  timeUsed;         // minimum online time before execute
      byte  age;              // minimum age
      byte  termAttrib;       // terminal attributes (bit 0 = ANSI / bit 1 = AVT)
      long  minSpeed;         // minimum logon speed
      long  maxSpeed;         // maximum logon speed
      long  credit;           // required credit
      long  optionCost;       // cost to select this option
      long  perMinCost;       // cost / minute to use this option
      byte  node[32];         // on wich node should it execute (1 bit / node)
      byte  group[32];        // wich group can use it (1 bit / group)
      word  startTime[7];     // start time to enable this option (1 / day)
      word  stopTime[7];      // end time to enable this option ( 1 /day)
      char  display[136];     // text to display
      char  hotkey [9];       // hotkey: hotkey[1] = key or ^A for autoexec
      char  data[136];        // optional data
      byte  fgColor;          // foreground color
      byte  bgColor;          // background color
      byte  reserved[50];     // reserved for RA written in Visual Basic ;-)
   };

   if(read_ra_paths() < 0)
      return(-1);

   FileName mask(ra_mnu_path,"*.Mnu");
   DirScan  find;

   bool status = find.first(mask);
   while(status == TRUE)
   {
      FileName fnIn (ra_mnu_path,find.name()); fnIn .mixedCase();
      FileName fnOut(cfg.mnupath,find.name()); fnOut.mixedCase(); fnOut.changeExt("Pbm");

      //printf("\n   þ Converting %s to %s...", (char *)fnIn, (char *)fnOut);
      File in;
      File out;

      if(in.open(fnIn, fmode_read, 4096) == FALSE)
         return(-1);

      if(out.open(fnOut, fmode_create, 4096) == FALSE)
         return(-1);

      _MenuFile pbHeader;  CLEAR_OBJECT(pbHeader);
      _MenuItem pbItem;    CLEAR_OBJECT(pbItem  );
      menuitem  raItem;    CLEAR_OBJECT(raItem  );


      if(in.read(&raItem, sizeof(menuitem)) != sizeof(menuitem))
         return(-1);

      pas2c(raItem.display);
      strcpy(pbHeader.prompt, raItem.display);
      pbHeader.highlight = cvtcolor(raItem.type    , raItem.bgColor);
      pbHeader.color     = cvtcolor(raItem.fgColor , raItem.bgColor);

      if(out.write(&pbHeader, sizeof(_MenuFile)) != sizeof(_MenuFile))
         return(-1);

      for(;;)
      {
         if(in.read(&raItem, sizeof(menuitem)) != sizeof(menuitem))
            break;

         if(pbFunction[raItem.type] != -1)      // if supported function...
         {
            pas2c(raItem.display );
            pas2c(raItem.data    );

            CLEAR_OBJECT(pbItem);

            strcpy(pbItem.text, raItem.display);
            strcpy(pbItem.data, raItem.data   );
            pbItem.attr                   = 0;
            pbItem.color                  = cvtcolor(raItem.fgColor, raItem.bgColor);
            pbItem.hotKey                 = raItem.hotkey[1];
            pbItem.function               = pbFunction[raItem.type];
            pbItem.access.minLevel        = raItem.minSec;
            pbItem.access.maxLevel        = raItem.maxSec;
            pbItem.access.flags           = swapbits(raItem.onFlags );
            pbItem.access.flagsNot        = swapbits(raItem.offFlags);
            pbItem.access.minAge          = raItem.age;
            pbItem.access.minTimeLeft     = raItem.timeLeft;
            pbItem.access.minTimeOnline   = raItem.timeUsed;
            pbItem.access.terminals       = 0xFF;        // geen omschrijving gevonden...
            pbItem.access.minSpeed        = raItem.minSpeed;
            pbItem.access.maxSpeed        = raItem.maxSpeed;

            for(int cnt = 0; cnt < 7; cnt++)
            {
               int start = (raItem.startTime[cnt] / 30);
               int stop  = (raItem.stopTime [cnt] / 30);

               for(int period = start; period <= stop; period++)
                  pbItem.access.timeFrame.enable(cnt, period);
            }

            strcpy(pbItem.data, checkData(raItem.type, pbFunction[raItem.type], pbItem.data));

            if(out.write(&pbItem, sizeof(struct _MenuItem)) != sizeof(struct _MenuItem))
               return(-1);
         }
      }

      status = find.next();
   }

   return(0);
}

/*---------------------------] END OF THE CODE [---------------------------*/
