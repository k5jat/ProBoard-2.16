#include <string.h>

#include "install.hpp"

bool
ReadInstallCfg()
{
   File f;
   FileName fn(source_dir,"INSTALL.CFG");

   if(!f.open(fn,fmode_read|fmode_text))
      return FALSE;

   for(;;)
   {
      char *s_kw = NULL,
           *s_p1 = NULL,
           *s_p2 = NULL;
      String kw,p1,p2;

      String s = f.readLine();

      if(!s.len())
         break;

      s.delLast('\n');

      s_kw = strtok(s," ");
      if(s_kw)
      {
         kw = s_kw;
         s_p1 = strtok(NULL," ");

         if(s_p1)
         {
            p1 = s_p1;
            s_p2 = strtok(NULL," ");

            if(s_p2)
               p2 = s_p2;
         }
      }

      if(kw == "EXT")
      {
         if(!p2.len())
            continue;

         tExtConvert cvt;

         cvt.uncompressed = p1;
         cvt.compressed   = p2;

         ll_ext_conv.add(cvt);

         continue;
      }

      if(kw == "PEX" || kw == "DOC" || kw == "ICN" || kw == "MNU" ||
         kw == "RIP" || kw == "SDK" || kw == "SYS" || kw == "TXT")
      {
         if(!p1.len())
            continue;

         tFileDestination dst;

         if(kw == "PEX")
            dst.dir = cfg.pexpath;
         if(kw == "DOC")
            dst.dir = doc_dir;
         if(kw == "ICN")
            dst.dir = cfg.iconspath;
         if(kw == "MNU")
            dst.dir = cfg.mnupath;
         if(kw == "RIP")
            dst.dir = cfg.RIPpath;
         if(kw == "SDK")
            dst.dir = sdk_dir;
         if(kw == "SYS")
            dst.dir = dest_dir;
         if(kw == "TXT")
            dst.dir = cfg.txtpath;

         dst.file = p1;

         if(!do_update || p2 == "U")
            ll_file_dest.add(dst);
      }
   }

   return TRUE;
}
