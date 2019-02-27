#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tswin.hpp>
#include "..\pbkey.hpp"

int   compress_data(byte *input , byte *output , int isize);
dword calc_security(KeyData *kd , byte *data1 , byte *data2);

void editkey(KeyData *);

#if defined(EVL)
   #define REGBY "Eddie Van Loon"
#elif defined(CS)
   #define REGBY "Constantin Spiess"
#elif defined(CK)
   #define REGBY "Chuck King"
#elif defined(WD)
   #define REGBY "Warren Davis"
#elif defined(KG)
   #define REGBY "Ken Givens"
#elif defined(SP)
   #define REGBY "Sarah Pearson"
#else
   #define REGBY "Philippe Leybaert"
#endif

main(int argc,char *argv[])
{
   File f;
   KeyFile *kf = new KeyFile;
   RandomTable *random_table = new RandomTable[32];

   FileName fn(argv[0]);
   fn.stripName();
   fn << "GENKEY.RND";

   if(!f.open(fn,fmode_read))
   {
      printf("Error reading random file\n");
      return 1;
   }

   f.read(random_table, 2048);
   f.close();

   byte *output_data = new byte[5000];

   dword r = *((unsigned long _far *)0x46C);

   memset(kf,0,sizeof(*kf));

   for(int i=0;i<100;i++)
   {
      kf->xorData[i] = char(r%0xFF);
      if(!kf->xorData[i]) i--;
      r = (r * 1103515245L + 12948L);
   }
   kf->xorData[100] = 0;

   String pBBSName;
   String pSysOpName;
   bool pAlpha         = FALSE;
   bool pBeta          = FALSE;
   bool pGamma         = FALSE;
   bool pCommercial    = FALSE;
   bool pDistribution  = FALSE;
   bool pRegSite       = FALSE;
   char pExpDate[20];
   char pExpVersion[20];

   strcpy(pExpDate,"00-00-00");
   strcpy(pExpVersion,"0.00");

   for(i=1;i<argc;i++)
   {
      if(argv[i][0] == '?')
      {
         printf("\nUsage:  GENKEY [options]\n\n"
                "Options:\n");
         printf("         /BBS=bbsname          (required)\n");
         printf("         /SYSOP=sysopname      (required)\n");
         printf("         /ALPHA\n");
         printf("         /BETA\n");
         printf("         /GAMMA\n");
         printf("         /COMMERCIAL\n");
         printf("         /DISTRIBUTION\n");
         printf("         /REGSITE\n");
         printf("         /ED=DD-MM-YY\n");
         printf("         /EV=xx.xx\n\n");
         printf("  (only the 3 first characters of the option tag are significant)\n");

         return 1;
      }

      if(argv[i][0] == '/')
      {
         if(!strnicmp("BBS",&argv[i][1],3))
         {
            char *p = strchr(argv[i],'=');

            if(p)
               pBBSName = p+1;
         }

         if(!strnicmp("SYS",&argv[i][1],3))
         {
            char *p = strchr(argv[i],'=');

            if(p)
               pSysOpName = p+1;
         }

         if(!strnicmp("BET",&argv[i][1],3))
            pBeta = TRUE;
         if(!strnicmp("ALP",&argv[i][1],3))
            pAlpha = TRUE;
         if(!strnicmp("GAM",&argv[i][1],3))
            pGamma = TRUE;
         if(!strnicmp("COM",&argv[i][1],3))
            pCommercial = TRUE;
         if(!strnicmp("DIS",&argv[i][1],3))
            pDistribution = TRUE;
         if(!strnicmp("REG",&argv[i][1],3))
            pRegSite = TRUE;
         if(!strnicmp("ED",&argv[i][1],2))
            strncpy(pExpDate,&argv[i][4],19);
         if(!strnicmp("EV",&argv[i][1],2))
            strncpy(pExpVersion,&argv[i][4],19);
      }
   }


   if(pBBSName.len() && pSysOpName.len())
   {
      strcpy(kf->keyData[0].sysopName , pSysOpName);
      strcpy(kf->keyData[0].bbsName   , pBBSName);

      int day = atoi(strtok(pExpDate,"/-."));
      int mon = atoi(strtok(NULL   ,"/-."));
      int yr  = atoi(strtok(NULL   ,"/-."));

      if(yr > 1900) yr -= 1900;

      kf->keyData[0].expirDate.set(day,mon,yr);

      int major_ver , minor_ver;

      if(strchr(pExpVersion,'.'))
      {
         sscanf(strchr(pExpVersion,'.')+1,"%x",&minor_ver);
         *strchr(pExpVersion,'.') = '\0';
         sscanf(pExpVersion,"%x",&major_ver);
      }

      kf->keyData[0].expirVersion = ((major_ver & 0xFF) << 8) | (minor_ver & 0xFF);

      if(pAlpha)
         kf->keyData[0].alpha = 1;
      if(pBeta)
         kf->keyData[0].beta = 1;
      if(pGamma)
         kf->keyData[0].gamma = 1;
      if(pCommercial)
         kf->keyData[0].commercial = 1;
      if(pDistribution)
         kf->keyData[0].distSite = 1;
      if(pRegSite)
         kf->keyData[0].regSite = 1;
   }
   else
   {
      editkey(&kf->keyData[0]);
   }

   kf->keyData[0].createDate.today();
   kf->keyData[0].createTime.now();

   strcpy(kf->keyData[0].createdBy,REGBY);

   for(i=0;i<32;i++)
      kf->securityCheck[i] = calc_security(&kf->keyData[0] , random_table[i].data1, random_table[i].data2);

   for(i = 1 ; i < 20 ; i++) kf->keyData[i] = kf->keyData[0];

   kf->keyCrc = crc32(kf->keyData , sizeof(kf->keyData));
   kf->xorCrc = crc32(kf->xorData , sizeof(kf->xorData));

   encrypt((char *)kf->keyData,sizeof(kf->keyData),kf->xorData);

   int output_size = compress_data((byte *)kf,output_data,sizeof(*kf));

   for(i = 0 ; i < output_size ; i++)
   {
      output_data[i] ^= fixed_xor_data[i % 100];
   }

   f.open("REGKEY.PB",fmode_create);
   f << "ProBoard Registration Key File\r\n\x1a";
   f.write(output_data,output_size);
   f.close();

   return 0;
}

void
editkey(KeyData *key)
{
   ScreenRestore restore(1);
   tsw_fillscreen('°',0x70);

   tsw_clearrect(1,1,tsw_hsize,1,0x1E);
   tsw_centerline(1,"ProBoard Key Generator for " REGBY);

   FormWindow w(11,5,70,21,0x1F,EXPLODE|SHADOW);
   w.open();

   Field fld[] = {
                  { FRM_STRING  , 0 , 0,0 , 0 , 35 , 36, 19,2  },
                  { FRM_STRING  , 0 , 0,0 , 0 , 35 , 36, 19,3  },
                  { FRM_YESNO   , 0 , 0,0 , 0 , 3  , 3 , 19,5  },
                  { FRM_YESNO   , 0 , 0,0 , 0 , 3  , 3 , 19,6  },
                  { FRM_YESNO   , 0 , 0,0 , 0 , 3  , 3 , 19,7  },
                  { FRM_YESNO   , 0 , 0,0 , 0 , 3  , 3 , 19,8  },
                  { FRM_YESNO   , 0 , 0,0 , 0 , 3  , 3 , 19,9  },
                  { FRM_YESNO   , 0 , 0,0 , 0 , 3  , 3 , 19,10 },
                  { FRM_STRING  , 0 , 0,0 , 0 , 10 , 11, 19,12 },
                  { FRM_STRING  , 0 , 0,0 , 0 ,  5 ,  6, 19,14 }
                 };

   char expdate[11] = "00/00/0000";
   char expver[6]   = "0.00";

   fld[0].value = key->sysopName;
   fld[1].value = key->bbsName;
   fld[2].value = &key->alpha;
   fld[3].value = &key->beta;
   fld[4].value = &key->gamma;
   fld[5].value = &key->distSite;
   fld[6].value = &key->regSite;
   fld[7].value = &key->commercial;
   fld[8].value = expdate;
   fld[9].value = expver;

   w << "\n Sysop Name ....:"
        "\n BBS Name ......:\n"
        "\n Alpha Test ....:"
        "\n Beta  Test ....:"
        "\n Gamma Test ....:"
        "\n Distr. Site ...:"
        "\n Reg.   Site ...:"
        "\n Commercial ....:\n"
        "\n Expir. Date ...:\n"
        "\n Expir. Version :";

   w.define(fld,10,0x1E,0x70);
   w.process();

   int day = atoi(strtok(expdate,"/-."));
   int mon = atoi(strtok(NULL   ,"/-."));
   int yr  = atoi(strtok(NULL   ,"/-."));

   if(yr > 1900) yr -= 1900;

   key->expirDate.set(day,mon,yr);

   int major_ver , minor_ver;

   if(strchr(expver,'.'))
   {
      sscanf(strchr(expver,'.')+1,"%x",&minor_ver);
      *strchr(expver,'.') = '\0';
      sscanf(expver,"%x",&major_ver);
   }

   key->expirVersion = ((major_ver & 0xFF) << 8) | (minor_ver & 0xFF);
}
