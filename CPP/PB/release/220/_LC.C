/***************************************************************************/
/*** _LC.C                    Last caller list module for ProBoard v2.10 ***/
/*** ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ***/
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

char *months_short[] = { "???","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

void
main(int argc,char *argv[])
{
    bool aliases = FALSE;
    int lc_num  = 10 ,  /* # Entries in list         */
        lc_node =  0 ,  /* Node number to list       */
        i ,
        f ;             /* File handle for BINLOG.PB */

    char fname[80];
    BINLOG *bl;
    long cur_rec;

    strcpy(fname,SysPath);
    strcat(fname,"BINLOG.PB");

    for(i=1;i<argc;i++)
    {
        if(isdigit(argv[i][0])) lc_num = atoi(argv[i]);

        if(argv[i][0] == '/')
        {
            switch(argv[i][1])
            {
                case 'N': if(strlen(argv[i]) > 3) lc_node = atoi(&argv[i][3]);
                          break;
                case 'H': aliases = TRUE;
                          break;
            }
        }
    }

    f = open(fname,O_RDONLY | O_DENYNONE);

    if(f < 0) return;

    bl = malloc( lc_num * sizeof(BINLOG));

    cur_rec = l_div(filelength(f) , sizeof(BINLOG));

    if(cur_rec >= 1) cur_rec--;

    for(i = 0 ; i < lc_num ; cur_rec--)
    {
        lseek(f , l_mul(cur_rec,sizeof(BINLOG)) , SEEK_SET);

        if(read(f , &bl[i] , sizeof(BINLOG)) != sizeof(BINLOG)) break;

        if(!bl[i].baud) continue;

        if(bl[i].uflags & UFLAG_HIDDEN) continue;

        if(lc_node && lc_node != bl[i].node) continue;

        i++;
    }

    close(f);

    lc_num = i;

    EnableStop();

    InitLineCounter();

    printf("\n\f\7ÖÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÒÄÄÄÄÄÄÄÄÒÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÒÄÄÄÄÒÄÄÄÄÄÄÄ·\n"
           "º\6   Name                         \7º\6  Date  \7º\6        Online        \7º\6 N# \7º\6 Speed \7º\n"
           "ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ×ÄÄÄÄÄÄÄÄ×ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ×ÄÄÄÄ×ÄÄÄÄÄÄÄ¶\n");

    LineCounter();
    LineCounter();
    LineCounter();

    for(i = 0 ; i<lc_num ; i++)
    {
      int onlinetime = 60 * (bl[i].timeOut[0] - bl[i].timeIn[0])
                          + (bl[i].timeOut[1] - bl[i].timeIn[1]);

      if(onlinetime < 0) onlinetime += 60*24;

      if(Stopped()) break;

      if(bl[i].alias[0] == '\0')
         strcpy(bl[i].alias,bl[i].name);

      printf("º\3 %-31.31s\7º\5 %02d %3.3s \7º\1 %02d:%02d \7-\1 %02d:%02d \7(\2%3d'\7) º\1%3d \7º\3 %5ld \7º\n",
            (aliases ? bl[i].alias:bl[i].name),bl[i].date[0],months_short[bl[i].date[1]],
            bl[i].timeIn[0],bl[i].timeIn[1],bl[i].timeOut[0],bl[i].timeOut[1],onlinetime,bl[i].node,bl[i].baud);

      if(!LineCounter()) break;
    }

    if(!Stopped())
      {
       if(LineCounter() && LineCounter() && LineCounter())
         {
          printf("ÓÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÐÄÄÄÄÄÄÄÄÐÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÐÄÄÄÄÐÄÄÄÄÄÄÄ½\n\n"
                 " Press \3[Enter]\7 to continue.\t");
         }
      }

    free(bl);
}
