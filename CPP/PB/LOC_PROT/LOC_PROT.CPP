#define Use_LinkedList

#include <pb_sdk.h>
#include <tslib.hpp>

const byte MODE_UPLOAD   = 0;
const byte MODE_DOWNLOAD = 1;

bool ReadFileList(char *listfile, LinkedList<FileName>& files);

void
main(int argc,char *argv[])
{
   byte mode;
   LinkedList<FileName> filelist;

   if(argc < 2)
      return;

   if(BaudRate)
      return;

   String logfname = getenv("DSZLOG");

   switch(toupper(argv[1][0]))
   {
      case 'U': mode = MODE_UPLOAD;
                break;
      case 'D': mode = MODE_DOWNLOAD;
                break;
      default : return;
   }

   File logfile(logfname,fmode_create|fmode_text);

   if(mode == MODE_DOWNLOAD)
   {
      if(argc < 3)
         return;

      if(argv[2][0] == '@')
         ReadFileList(&argv[2][1],filelist);
      else
         filelist.add(FileName(argv[2]));

      printf("\n\n\7Local Download\n"
                   "--------------\n\n");

      String dir = Config->uploadpath;
      dir.delLast('\\');

      for(filelist.rewind();!filelist.eol();filelist++)
      {
         FileName fn_full = filelist.get();
         FileName fn      = fn_full;

         fn.stripPath();

         for(;;)
         {
            char s[61];

            printf("\6File \3%-12s\6 (Default destionation = %s)\n\n",(char *)fn,(char *)dir);
            printf("Copy to directory: \7");

            Input(s,60,INPUT_UPALL);

            if(s[0])
            {
               dir = s;
               dir.delLast('\\');
            }

            if(!ts_DirExists(dir))
            {
               printf("\n\n1\Directory does not exist!\n\n");
               continue;
            }

            printf("\n\n\7Copying file \3%s \7to \3%s\7...",(char *)fn, (char *)dir);

            if(ts_CopyFile(fn_full,dir,2048))
            {
               logfile << "D " << (char *)fn << '\n';
               printf("\2Done!\n\n\n");
            }
            else
            {
               printf("\1Error!!\n\n\n");
            }

            break;
         }
      }
   }

   if(mode == MODE_UPLOAD)
   {
      printf("\n\n\7Local Upload\n"
                   "------------\n\n");

      String dest_dir = Config->uploadpath;

      if(argc > 2)
         dest_dir = argv[2];

      if(!ts_DirExists(dest_dir))
      {
         printf("\1Upload directory '%s' does not exist!\n\n"
                "\7Press \3[Enter]\7 to continue\t",(char *)dest_dir);

         return;
      }


      for(;;)
      {
         char s[61];

         printf("\7Enter the complete path and filename of the file you want to upload\n\n");
         Input(s,60,INPUT_UPALL);

         if(!s[0])
            break;

         if(access(s,0))
         {
            printf("\n\n\1File not found!\n\n");
            continue;
         }

         printf("\n\n\6Copying file \3%s\6 to \3%s\6...",s,(char *)dest_dir);

         if(ts_CopyFile(s,dest_dir,2048))
         {
            printf("\7Done!\n\n");

            FileName fn(s);
            fn.stripPath();

            logfile << "U " << (char *)fn << '\n';
         }
         else
            printf("\1ERROR!\n\n");
      }
   }
}


bool
ReadFileList(char *listfile, LinkedList<FileName>& files)
{
   File f;

   files.kill();

   if(!f.open(listfile,fmode_text))
      return FALSE;

   for(;;)
   {
      FileName s;

      s = f.readLine();

      if(!s.len())
         break;

      s.delLast('\n');

      files.add(s);
   }

   return TRUE;
}
