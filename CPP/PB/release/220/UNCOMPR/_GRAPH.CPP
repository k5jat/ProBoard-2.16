/***************************************************************************/
/*** _GRAPH.CPP                    Usage graph module for ProBoard v2.10 ***/
/*** ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ ***/
/*** This source file is distributed together with ProBoard v2.10, and   ***/
/*** may be modified and redistributed, provided that this header is     ***/
/*** not modified. When distributing the modified source code, make sure ***/
/*** you add a comment block with your name and any changes you have     ***/
/*** made. It would be appreciated if you send a copy of the modified    ***/
/*** source code to the author (Philippe Leybaert)                       ***/
/***                                                                     ***/
/*** The author can be reached at FidoNet 2:291/1905                     ***/
/***                           or CompuServe 70314,2021                  ***/
/***                           or phl@innet.be                           ***/
/***                           or in the FidoNet PROBOARD echo           ***/
/***************************************************************************/

#include <pb_sdk.h>
#include "cppdate.hpp"

#define GRAPH_SPEED     0
#define GRAPH_LASTDAYS  1
#define GRAPH_HOURLY    2
#define GRAPH_WEEKS     3
#define GRAPH_AVGSPEED  4

void put_meganum( word num );

long num_calls;

char *weekdays[] = { "Mo" , "Tu" , "We" , "Th" , "Fr" , "Sa" , "Su" };

int
read_binlog(int mode,int ndays,long *data)
{
    FILE *fp;
    BINLOG bl;
    Date today(TODAY);
    char fname[80];
    int max_diff = 0;
    int i;
    long total = 0;

    long count[24];

    for(i = 0;i<24;i++) count[i] = 0;

    printf("\n\7\f\nReading data...");

    strcpy(fname,SysPath);
    strcat(fname,"BINLOG.PB");

    fp = fopen(fname,"rb");

    if(fp == NULL)
    {
        Log(LOG_FRIEND,"Unable to open BINLOG.PB");

        return -1;
    }

    setvbuf(fp,NULL,_IOFBF,16384);

    for(;;)
    {
        if(fread( &bl , sizeof(bl) , 1 , fp) != 1) break;

        Date bl_date(bl.date[0],bl.date[1],bl.date[2]);

        if(bl.timeIn[0] > 23 || !bl_date.ok())
        {
            Log(LOG_FRIEND,"BINLOG.PB corrupted!");

            fclose(fp);

            return -1;
        }

        if(!bl.baud) continue;

        switch(mode)
        {
            case GRAPH_HOURLY:
            {
                int h;
                int onlinetime;
                int diff = int(today - bl_date);

                if(ndays && diff > ndays) continue;

                num_calls++;

                if(diff > max_diff) max_diff = diff;


                onlinetime = 60 * (bl.timeOut[0] - bl.timeIn[0])
                                + (bl.timeOut[1] - bl.timeIn[1]);

                if(onlinetime < 0) onlinetime += 60*24;

                h = bl.timeIn[0] - 6;

                if(h < 0) h += 24;

                data[h] += onlinetime;
            }
            break;

            case GRAPH_LASTDAYS:
            {
                int diff = int(today - bl_date);

                if(diff < 24 && diff >= 0)
                {
                    int onlinetime = 60 * (bl.timeOut[0] - bl.timeIn[0])
                                        + (bl.timeOut[1] - bl.timeIn[1]);

                    num_calls++;

                    if(onlinetime < 0) onlinetime += 60*24;

                    data[23-diff] += onlinetime;

                    if(diff > max_diff) max_diff = diff;
                }
            }
            break;

            case GRAPH_WEEKS:
            {
                int week         = bl_date.weekNum();
                int weeks_to_add = today.weeksInYear(today.year()-1);
                int weeknow      = today.weekNum() + weeks_to_add;

                int diff = int(today - bl_date);

                if(diff < 0  ||  diff > 200) continue;

                if(week+weeks_to_add <= weeknow) week += weeks_to_add;

                if(weeknow-week < 24)
                {
                    int onlinetime = 60 * (bl.timeOut[0] - bl.timeIn[0])
                                        + (bl.timeOut[1] - bl.timeIn[1]);

                    if(onlinetime < 0) onlinetime += 60*24;

                    data[23 - (weeknow - week)] += onlinetime;

                    if(weeknow - week > max_diff) max_diff = weeknow - week;

                    num_calls++;
                }
            }
            break;

            case GRAPH_SPEED:
            {
                int speed;
                int diff = int(today - bl_date);

                if(!bl.baud) continue;

                if(ndays && diff > ndays) continue;

                num_calls++;

                if(diff > max_diff) max_diff = diff;

                switch(bl.baud)
                {
                    case 2400L : speed =  0;  break;
                    case 9600L : speed =  1;  break;
                    case 14400L: speed =  2;  break;
                    case 16800L: speed =  3;  break;
                    case 19200L: speed =  4;  break;
                    case 21600L: speed =  5;  break;
                    case 24000L: speed =  6;  break;
                    case 26400L: speed =  7;  break;
                    case 28800L: speed =  8;  break;
                    case 31200L: speed =  9;  break;
                    case 33600L: speed = 10; break;
                    default    : speed = 11;  break;
                }

                data[speed]++;
                total++;
            }
            break;

            case GRAPH_AVGSPEED:
            {
                int week         = bl_date.weekNum();
                int weeks_to_add = today.weeksInYear(today.year()-1);
                int weeknow      = today.weekNum() + weeks_to_add;

                if(int(today - bl_date) < 0) continue;

                if(week+weeks_to_add <= weeknow) week += weeks_to_add;

                if(weeknow-week < 24)
                {
                    count[23 - (weeknow - week)] ++;

                    data[23 - (weeknow - week)] += l_div(bl.baud ,100);

                    if(weeknow - week > max_diff) max_diff = weeknow - week;

                    num_calls++;
                }
            }
            break;
        }
    }

    switch(mode)
    {
        case GRAPH_HOURLY:
        {
            if(max_diff) for(i=0;i<24;i++) data[i] = l_div(l_mul(data[i],10) , (6 * max_diff));
        }
        break;

        case GRAPH_LASTDAYS:
        {
            for(i = 0 ; i < 24 ; i++) data[i] = l_div(l_mul(data[i] , 10) , 144);
        }
        break;

        case GRAPH_SPEED:
        {
            if(total) for(i = 0 ; i < 24 ; i++) data[i] = l_div(l_mul(data[i] , 100) , total);
        }
        break;

        case GRAPH_WEEKS:
        {
            for(i = 0 ; i < 24 ; i++) data[i] = l_div(l_mul(data[i] , 10) , 1008);
        }
        break;

        case GRAPH_AVGSPEED:
        {
            for(i = 0 ; i < 24 ; i++) if(count[i]) data[i] = l_div(data[i] , count[i]);
        }
    }

    fclose(fp);

    if(max_diff < 0)
    {
        printf("\1Error reading log file! Please inform sysop!\n\n"
               "\7Press \3[Enter]\7 to continue.\t");
    }
    else
    {
        printf("\n");
    }

    return max_diff;
}

static void
chart(long *data,int data_size,int width,char *str1,char *str2,char *str3)
{
    static long legal_max[] = { 4,8,12,20,40,60,80,100,120,160,200,240,300,500,0 };
    long max = 0;
    int i;

   for(i = 0 ; i < data_size ; i++) if(data[i] > max) max = data[i];

   for(i = 0 ; legal_max[i] ; i++)
      if(max <= legal_max[i])
         break;

   if(legal_max[i]) max = legal_max[i];

   if(RIP())
   {
      DisableStop();
      LocalDisplay(FALSE);

      printf("!|1K|*|w0000000000|W00|Y00000100|1B00000200LK040F000B010900000F07000000\n");
      printf("!|1U0A08H8880000<><>|1B000002007S040F000B010900000F07000000\n");
      printf("!|1U0U0OGO1C0000<>\\\n");
      printf("%s\\\n",str1);
      printf("<>\n");
      printf("!|1B000002007K040F000B010900000F07000000|1U0U1KGO7S0000<><>|c0F|=00000003\n");
      printf("!|LGE741474|L2874281S|Y02000400|1B00000200LC020F000F080700000F07000000\n");

      printf("!|=00000001\n");

      for(i=0;i<4;i++)
      {
         printf("!|c01|L");
         put_meganum(90);
         put_meganum(70 + i*45);
         put_meganum(570);
         put_meganum(70 + i*45);

         printf("\n!|c0E|@15");
         put_meganum(70 + i*45 - 5);
         printf("%5d\n",max - (max/4) * i);
      }

      printf("!|c0F\n");

      for(i=0;i<data_size;i++)
      {
         if(!data[i]) continue;

         printf("!|1U");
         put_meganum(90 + i * width * 6);
         put_meganum(250 - 180L*data[i] / max);
         put_meganum(90 + (i+1) * width * 6 - 6);
         put_meganum(250);
         printf("0000<><>\n");
      }

      printf("!|@2H79%s\n" , str3);
      printf("!|w0012271510|#|#|#\n");

      LocalDisplay(TRUE);
      RemoteDisplay(FALSE);
   }

   EnableStop();

   printf("\n\f\6 %-3s\7บ\6 %s\n" , str2, str1);

   if(Stopped()) return;

   printf("\7ออออฮอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n");

   if(Stopped()) return;

   for(i = 0 ; i < 16 ; i++)
   {
      int y;

      if(!(i%4)) printf("\1%3d\7 บ",(16-i) * (int)max / 16);
               else printf("    \7บ\3");

      if(!(i%4)) SetFullColor(7);

      if(Stopped()) return;

      for(y=0 ; y<data_size ; y++)
      {
            int w;

            putchar( !(i%4) ? 'ฤ' : ' ');

            if(data[y] >= l_div(l_mul(max,16-i),16) && data[y])
            {
               if(!(i%4)) SetColor(3);
               for(w = 1 ; w < width ; w++) putchar('Û');
               if(!(i%4)) SetFullColor(7);
            }
            else
            if(data[y] >= l_div(l_mul(max,31-2*i),32) && data[y])
            {
               if(!(i%4)) SetColor(3);
               for(w = 1 ; w < width ; w++) putchar('Ü');
               if(!(i%4)) SetFullColor(7);
            }
            else
            {
               for(w = 1 ; w < width ; w++) putchar(!(i%4) ? 'ฤ' : ' ');
            }

            if(Stopped()) return;
      }

      for(int w=72;w>=(data_size*width);w--)
         putchar(!(i%4) ? 'ฤ' : ' ');

      printf("\7บ\n");

      if(Stopped()) return;
   }

   printf("\7ออออฮอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออน\n"
            "    บ\6 %-72.72s\7บ\n",str3);

   RemoteDisplay(TRUE);

   printf("\n\7Press \3[Enter]\7 to continue.\t");

   if(RIP()) printf("\n!|1K|*\n");
}

void
last_days(int ndays,char *msg)
{
    int i;

    long *data = (long *)malloc( 24 * sizeof(long) );

    for(i = 0 ; i < 24 ; i++) data[i] = 0;

    if(read_binlog(GRAPH_LASTDAYS,ndays,data) >= 1)
    {
        char *p;
        int wd;
        Date today(TODAY);
        char str[80];

        wd = today.weekDay() - 2;

        str[0] = 0;

        for(i = 0 ; i < 24 ;wd++,i++)
        {
            if(wd > 6) wd -= 7;
            if(wd < 0) wd += 7;

            strcat(str , weekdays[wd]);
            strcat(str," ");
        }

        p = msg;

        if(p[0] == '\0') p = "Usage graph per day for the last %d days";

        chart(data,24,3,form(p,24),"%",str);
    }

    free(data);
}

void
speed_graph(int numdays , char *msg)
{
    int i;

    long *data = (long *)malloc( 24 * sizeof(long));

    for(i = 0 ; i < 24 ; i++) data[i] = 0;

    numdays = read_binlog(GRAPH_SPEED,numdays,data);

    if(numdays >= 1)
    {
        char *p = msg;

        if(p[0] == '\0') p = "Speed Statistics for the last %d days";

        chart(data,12,6,form(p,numdays),"%"," 2400  9600 14400 16800 19200 21600 24000 26400 28800 31200 33600 Other");
    }

    free(data);
}

void
avgspeed_graph(int nweeks,char *msg)
{
    int i;

    long *data = (long *)malloc( 24 * sizeof(long));

    for(i = 0 ; i < 24 ; i++) data[i] = 0;

    nweeks = read_binlog(GRAPH_AVGSPEED,24,data);

    if(nweeks >= 1)
    {
        int weeknr;
        char *p;
        Date today(TODAY);
        char str[80];

        weeknr = today.weekNum() + 53 - 24;

        str[0] = '\0';

        for(i = weeknr ; i < (weeknr+24) ; i++)
           strcat(str,form("%02d ",(i%53)+1));

        p = msg;

        if(p[0] == '\0') p = "Average connect speed for the last %d weeks";

        chart(data,24,3,form(p,nweeks),"BPS",str);
    }

    free(data);
}


void
hourly_graph(int numdays,char *msg)
{
    int i;

    long *data = (long *)malloc( 24 * sizeof(long));

    for(i=0;i<24;i++) data[i] = 0;

    num_calls = 0;

    numdays = read_binlog(GRAPH_HOURLY , numdays , data);

    if(numdays >= 1)
    {
        char *p = msg;

        if(p[0] == '\0') p = "Hourly Usage Graph for the last %d days (%d.%d calls/day)";

        chart(data,24,3,form(p,numdays,(int)l_div(num_calls,numdays),int(l_div(l_mul(num_calls,10),numdays))%10),"%","06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 00 01 02 03 04 05");
    }

    free(data);
}

void
weekly_graph(int nweeks,char *msg)
{
    int i;

    long *data = (long *)malloc( 24 * sizeof(long));

    for(i = 0 ; i < 24 ; i++) data[i] = 0;

    nweeks = read_binlog(GRAPH_WEEKS,24,data);

    if(nweeks >= 1)
    {
        int weeknr;
        char *p;
        Date today(TODAY);
        char str[80];

        nweeks++;

        weeknr = today.weekNum() + 53 - 24;

        str[0] = '\0';

        for(i = weeknr ; i < (weeknr+24) ; i++)
           strcat(str,form("%02d ",(i%53)+1));

        p = msg;

        if(p[0] == '\0') p = "Usage graph per week for the last %d weeks";

        chart(data,24,3,form(p,nweeks),"%",str);
    }

    free(data);
}



void
main(int argc,char *argv[])
{
    int report = 0;
    int i;
    int ndays = 0;

    char text[80];

    text[0] = 0;

    for(i=1;i<argc;i++)
    {
        if(isalpha(argv[i][0]))
        {
            if(!stricmp(argv[i] , "Speed"   )) report = 0;
            if(!stricmp(argv[i] , "LastDays")) report = 1;
            if(!stricmp(argv[i] , "Hourly"  )) report = 2;
            if(!stricmp(argv[i] , "Weekly"  )) report = 3;
            if(!stricmp(argv[i] , "AvgSpeed")) report = 4;
        }

        if(argv[i][0] == '/')
        {
            switch(argv[i][1])
            {
                case 'N': ndays = atoi(&argv[i][3]);
                          break;
                case 'T': strcpy(text,&argv[i][3]);
                          break;
            }
        }
    }

    switch(report)
    {
        case 0: speed_graph(ndays,text);
                break;
        case 1: last_days(ndays,text);
                break;
        case 2: hourly_graph(ndays,text);
                break;
        case 3: weekly_graph(ndays,text);
                break;
        case 4: avgspeed_graph(ndays,text);
                break;
    }
}

void
put_meganum( word num )
{
   int n[2];
   char s[3] = "00";

   n[0] = num / 36;
   n[1] = num % 36;

   for(int i = 0 ; i < 2 ; i++)
   {
      if(n[i] < 10)
         s[i] = '0' + n[i];
      else
         s[i] = 'A' + n[i] - 10;
   }

   printf(s);
}
