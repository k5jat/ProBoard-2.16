#include <string.h>
#include <stdio.h>
#include <tslib.hpp>
#include "convert.hpp"

unsigned _stacksize = 16000;

Config cfg;
char syspath[70];

main(int argc,char *argv[])
{
 printf("\nProBoard v2.2 Conversion Utility\n" );
 printf( "Copyright (c) 1990-1999 TeleGrafix Communications, Inc.  All Rights Reserved\n\n");

 getsyspath(syspath,argv[0]);

 if(argc>1)
   {
    if(!strcmpl(argv[1],"RA"))
      {
       if(argc<3)
         {
          printf("You have to specify the directory where RA-files are located!\n");
          return 1;
         }

       strcpy(ra_path,argv[2]);
       append_backspace(ra_path);

       if(!read_config()) return 1;

       printf("Converting RA message areas...");
       if(convert_ra_msgs()<0) printf("\n");
                         else  printf("Done.\n");

       printf("Converting RA file areas......");
       if(convert_ra_files()<0) printf("\n");
                          else  printf("Done.\n");

       printf("Converting RA menus...........");
       if(convert_ra_menus()<0) printf("\n");
                          else  printf("Done.\n");

       return 0;
      }

    if(!strcmpl(argv[1],"PB"))
      {
       printf("ProBoard v2.01 upgrade\n\n");

       read_config();

       //printf("Converting user files........");
       //if(!convert_userfile()) printf("\n");
       //                  else  printf("Done.\n");
       //printf("Converting menus.............");
       //if(!convert_menus()) printf("\n");
       //               else  printf("Done.\n");

       printf("Converting message areas..........");
       if(!convert_msgareas()) printf("\n");
                         else  printf("Done.\n");

       printf("Converting modem config...........");
       if(!convert_modem()) printf("\n");
                      else  printf("Done.\n");

       printf("Converting personal file list.....");
       if(!convert_pvtfiles()) printf("\n");
                      else     printf("Done.\n");

       printf("Upgrading CONFIG.PRO..............");
       if(!convert_config())   printf("\n");
                      else     printf("Done.\n");

       printf("Upgrading Language files..........");
       if(!convert_languages()) printf("");
                      else      printf("");

       return 0;
      }

 /*
    if(!strcmpl(argv[1],"MAX"))
      {
       if(argc < 3)
         {
          printf("You have to specify the directory where the Maximus user file is located!\n");
          return 1;
         }

       strcpy(maximus_path,argv[2]);
       append_backspace(maximus_path);

       if(!read_config()) return 1;

       printf("Converting Maximus User file...");
       if(convert_maximus() < 0) printf("\n");
                        else     printf("Done.\n");

       return 0;
      }
 */

    if(!strcmpl(argv[1],"SIMUL1"))
      {
       bool unlimited = FALSE;

       if(!read_config()) return 1;

       if(argc > 2 && !strcmpl(argv[2],"-U")) unlimited = TRUE;

       printf("Creating FILES.RA......");
       if(!simul_files_ra(unlimited)) printf("\n");
                                 else printf("Done.\n");
       printf("Creating MESSAGES.RA...");
       if(!simul_messages_ra()) printf("\n");
                        else printf("Done.\n");
       printf("Creating CONFIG.RA.....");
       if(!simul_config_ra()) printf("\n");
                         else printf("Done.\n");
       return 0;
      }

    if(!strcmpl(argv[1],"SIMUL2"))
      {
       bool unlimited = FALSE;

       if(!read_config()) return 1;

       if(argc > 2 && !strcmpl(argv[2],"-U")) unlimited = TRUE;

       printf("Creating FILES.RA......");
       if(!simul_files_ra2(unlimited)) printf("\n");
                                  else printf("Done.\n");
       printf("Creating MESSAGES.RA...");
       if(!simul_messages_ra2()) printf("\n");
                         else printf("Done.\n");
       printf("Creating CONFIG.RA.....");
       if(!simul_config_ra2()) printf("\n");
                          else printf("Done.\n");
       return 0;
      }

    strupr(argv[1]);
    printf("Unknown option: %s\n",argv[1]);

    return 0;
   }


 printf("Usage: CONVERT <mode> [ra-dir]\n\n"
        "     mode:\n"
        "              PB = Convert ProBoard 2.01/2.1x files to ProBoard 2.2\n\n"
        "              RA = Convert RA 2.0x files to ProBoard 2.20\n\n"
        "          SIMUL1 = Create RemoteAccess 1.11 system files\n"
        "          SIMUL2 = Create RemoteAccess 2.01 system files\n");

 return 1;
}

bool
read_config()
{
 File fi;

 if(!fi.open(FileName(syspath,"CONFIG.PRO")))
   {
    printf("Can't open CONFIG.PRO\n");
    return FALSE;
   }

 fi.read(&cfg,sizeof(cfg));

 append_backspace(cfg.msgpath);

 return TRUE;
}
