#ifdef PEX
   #include <pb_sdk.h>
#else
   #include <io.h>
   #include <string.h>
   #include <ctype.h>
   #include <dir.h>
   #include <dos.h>
#endif

#include <tslib.hpp>

static char *
ts_DelLastChar( char *text, char chr )
{
   int len = strlen (text) - 1;

   if(text[len] == chr)
      text[len] = '\0';

   return text;
}


bool
ts_MakeFullDir(char *path)
{
   char str[128];
   char *p = path;

   str[0] = '\0';

   strcpy(str,path);

   p = strrchr(str,':');

   if(p)
   {
      *(p+1) = '\0';

      p = strrchr(path,':') + 1;
   }
   else
   {
      str[0] = '\0';

      p = path;
   }

   /* Vanaf hier wijst p naar de pathname zonder drive, en str[] bevat */
   /* de drive (als er een is)                                         */

   if(*p == '\\')
   {
      str[strlen(str)+1] = '\0';
      str[strlen(str)]   = *p++;
   }

   for(;;p++)
   {
      if(*p == '\\' || *p == '\0')
      {
         ts_DelLastChar(str,'\\');

         if(!ts_DirExists(str))
            if(mkdir(str) < 0)
               return FALSE;
      }

      str[strlen(str)+1] = '\0';
      str[strlen(str)  ] = *p;

      if(*p == '\0') break;
   }

   return TRUE;
}

bool
ts_DirExists(char *path)
{
   String s(path);

   s.delLast('\\');

   if(s[1]==':' && s.len() <= 2)
      return TRUE;

   DirScan scan(s,DIR_DIREC);

   return bool(int(scan) && (scan.flags() & DIR_DIREC));
}

String
ts_ResolveDir(char *dir)
{
   char cur_drive = '\0';
   char cur_dir[MAXDIR];
   char drive = '\0';
   FileName tmp_dir;

   cur_drive = drive = char(getdisk());
   getcurdir(cur_drive+1,cur_dir);

   if(strchr(dir,':'))
   {
      if(dir[1] != ':')
         return "";

      drive = char(toupper(dir[0]) - 'A');
      getcurdir(drive+1,cur_dir);

      tmp_dir = &dir[2];
   }
   else
   {
      tmp_dir = dir;
   }

   if(tmp_dir[0] != '\\')
   {
      tmp_dir.insert(0,"\\");
      tmp_dir.insert(0,cur_dir);
      tmp_dir.insert(0,"\\");
   }

   tmp_dir.appendBS();           // tmp_dir contains full directory

   String dir_stack[30];
   int stack_p = 0;

   char *s = strtok(tmp_dir,"\\");

   while(s)
   {
      String tmp = s;

      if(tmp == "..")
      {
         if(stack_p)
            stack_p--;
      }
      else
      {
         if(tmp != ".")
            dir_stack[stack_p++] = tmp;
      }

      s = strtok(NULL,"\\");
   }

   tmp_dir = char(drive+'A');

   tmp_dir << ":\\";

   for(int i=0;i<stack_p;i++)
   {
      tmp_dir << dir_stack[i] << '\\';
   }

   return tmp_dir;
}


bool
ts_IsFullPath(char *s)
{
   if(strlen(s) < 3 || strncmp(&s[1],":\\",2))
      return FALSE;

   return !!isalpha(s[0]);
}

void
ts_MakeInputPath(String& s)
{
   s.delLast('\\');
   if(s.lastChar() == ':')
      s << '\\';
}

void
ts_MakeInputPath(char *s)
{
   if(strlen(s) >= 3)
   {
      ts_DelLastChar(s,'\\');

      if(s[strlen(s)-1] == ':')
         strcat(s,"\\");
   }
}

bool
ts_SyncFileDate(char *fn1,char *fn2)
{
   File f1,f2;

   if(!f1.open(fn1) || !f2.open(fn2,fmode_rw))
      return FALSE;

   unsigned fdate,ftime;

   if(!_dos_getftime(f1.dosHandle(),&fdate,&ftime))
      _dos_setftime(f2.dosHandle(),fdate,ftime);

   return TRUE;
}
