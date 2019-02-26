#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <process.h>
#include <io.h>

#include "install.hpp"

#define VERSION "2.2"

LinkedList<tExtConvert>      ll_ext_conv;
LinkedList<tFileDestination> ll_file_dest;

FileName source_dir;  // Full path of where INSTALL.EXE is located
FileName dest_dir;    // Full path of where ProBoard should be installed

FileName doc_dir;     // Full path of where the documentation should be copied
FileName sdk_dir;     // Full path of where the sdk should be copied

char     boot_drive;  // (uppercase 'A'-'Z')
bool     do_update;   // Perform update instead of full install?

static char proboard_env_buf[80];

_Config  cfg;

static bool ProBoardInstalled(FileName dir);
static bool InitConfig(_Config&);
static bool CopyFiles();

void
show_msg(char *s)
{
     Window w( 0,
               0,
               70,
               2,
               0x5F,
               EXPLODE | SHADOW | CENTERED_X | CENTERED_Y,
               CHISEL_BORDER,
               0x5D,
               NULL,
               0x50 );

     w.open();

     w.centerLine(0, s, 0x5E);
     KB.get();
}

void
show_error(char *s1,char *s2)
{
     Window w( 0,
               0,
               70,
               6,
               0x4F,
               EXPLODE | SHADOW | CENTERED_X | CENTERED_Y | BRACKETS,
               CHISEL_BORDER,
               0x4C,
               NULL,
               0x40 );
     w.open();
     w.title( "ERROR", 0xCF);
     w.centerLine(2,s1,0x4E);
     w.centerLine(4,s2,0x4E);
     KB.get();
}

void
main(int ,char *argv[])
{
   char inp_s[80];
   ScreenRestore restore(1);

   tsw_cursoroff();

   boot_drive = 'C';

   if(getenv("COMSPEC"))
      boot_drive = *getenv("COMSPEC");

   source_dir = argv[0];
   source_dir.stripName();

   dest_dir = "C:\\PB";

   if(getenv("PROBOARD"))
      dest_dir = getenv("PROBOARD");

   ts_MakeInputPath(dest_dir);

   tsw_fillscreen( '±', 0x19 );

   Window titlew;

   titlew.open( 1,
                1,
                tsw_hsize,
                3,
                0x70,
                CENTERED_X,
                CHISEL_BORDER,
                0x7F,
                NULL,
                0x78 );

   titlew.centerLine( 0,"ProBoard v" VERSION " Installation Utility", 0x71);

   tsw_showfooter( "Copyright (c) 1990-1999  TeleGrafix Communications, Inc.  All Rights Reserved",
                   BAR_HIGH );

   for(;;)
   {
      do_update = FALSE;

      Window inputw;
      
      inputw.open( tsw_hsize / 2 + 1 - 35,
                   11,
                   tsw_hsize/2 + 35,
                   15,
                   0x3F,
                   SHADOW,
                   CHISEL_BORDER,
                   0x3B,
                   NULL,
                   0x31 );

      inputw << "\n Install ProBoard in: ";

      inputw.attrib(0x31);
      inputw.setFieldAttr(0x4E);

      strcpy(inp_s,dest_dir);

      tsw_cursoron();

      if(inputw.scan(inp_s,60,40,SCAN_UPPER|SCAN_FIELD) == SF_ESC)
         return;

      tsw_cursoroff();

      dest_dir = inp_s;

      ts_MakeInputPath(dest_dir);

      inputw.close();

      if(ts_DirExists(dest_dir))
      {
         if(ProBoardInstalled(dest_dir))
         {
            Window w( 0,
                      10,
                      60,
                      17,
                      0x3F,
                      SHADOW | EXPLODE | CENTERED_X,
                      CHISEL_BORDER,
                      0x3B,
                      NULL,
                      0x31 );
            w.open();

            w.centerLine(2,"ProBoard is already installed in this directory",0x3F);
            w.centerLine(3,"Update it to version " VERSION " ?",0x3F);
            w.centerLine(5,"[Y/N]",0x3E);

            if(!KB.ask())
               return;

            do_update = TRUE;
         }
      }
      else
      {
         Window w( 0,
                   10,
                   60,
                   17,
                   0x3F,
                   SHADOW | EXPLODE | CENTERED_X,
                   CHISEL_BORDER,
                   0x3B,
                   NULL,
                   0x31 );
         w.open();

         w.centerLine(2,"The specified directory does not exist",0x3F);
         w.centerLine(3,"Create it?",0x3F);
         w.centerLine(5,"[Y/N]",0x3E);

         if(!KB.ask())
            continue;

         if(!ts_MakeFullDir(dest_dir))
         {
            show_error("Error creating destination directory","Press any key to continue");
            continue;
         }
      }

      Window w;

      if(do_update)
      {
         w.open( 0,
                 0,
                 72,
                 7,
                 0x3F,
                 EXPLODE | SHADOW | CENTERED_X | CENTERED_Y,
                 CHISEL_BORDER,
                 0x3B,
                 NULL,
                 0x31 );

         w.centerLine(2,"Your current ProBoard setup will now be updated to v" VERSION,0x3F);
         w.centerLine(4,"Continue?",0x3F);
         w.centerLine(5,"[Y/N]",0x3E);
      }
      else
      {
         w.open( 0,
                 0,
                 72,
                 10,
                 0x3F,
                 EXPLODE | SHADOW | CENTERED_X | CENTERED_Y,
                 CHISEL_BORDER,
                 0x3B,
                 NULL,
                 0x31 );

         w.centerLine(2,"ProBoard v" VERSION " will now be installed in the following directory:",0x3F);
         w.centerLine(4,dest_dir,0x3F);
         w.centerLine(7,"Continue?",0x3F);
         w.centerLine(8,"[Y/N]",0x3E);
      }

      if(KB.ask())
         break;
   }

   if(!InitConfig(cfg))
      return;

   doc_dir = dest_dir;
   doc_dir.appendBS();
   doc_dir << "DOC";

   sdk_dir = dest_dir;
   sdk_dir.appendBS();
   sdk_dir << "SDK";

   if(!do_update)
   {
      for(;;)
      {
         if(!EditDirectories())
         {
            Window w( 0,
                      10,
                      60,
                      17,
                      0x4F,
                      SHADOW | EXPLODE | CENTERED_X,
                      CHISEL_BORDER,
                      0x4C,
                      NULL,
                      0x40 );
            w.open();

            w.centerLine(2,"Are you sure you want to quit?",0x4F);
            w.centerLine(4,"[Y/N]",0x4E);

            if(KB.ask())
               return;
            else
               continue;
         }

         bool error = FALSE;

         if(!ts_DirExists(cfg.txtpath      ) && !ts_MakeFullDir(cfg.txtpath      ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",cfg.txtpath      ),"Press any key to continue");
         }
         if(!ts_DirExists(cfg.RIPpath      ) && !ts_MakeFullDir(cfg.RIPpath      ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",cfg.RIPpath      ),"Press any key to continue");
         }
         if(!ts_DirExists(cfg.iconspath    ) && !ts_MakeFullDir(cfg.iconspath    ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",cfg.iconspath    ),"Press any key to continue");
         }
         if(!ts_DirExists(cfg.mnupath      ) && !ts_MakeFullDir(cfg.mnupath      ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",cfg.mnupath      ),"Press any key to continue");
         }
         if(!ts_DirExists(cfg.msgpath      ) && !ts_MakeFullDir(cfg.msgpath      ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",cfg.msgpath      ),"Press any key to continue");
         }
         if(!ts_DirExists(cfg.uploadpath   ) && !ts_MakeFullDir(cfg.uploadpath   ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",cfg.uploadpath   ),"Press any key to continue");
         }
         if(!ts_DirExists(cfg.pvtuploadpath) && !ts_MakeFullDir(cfg.pvtuploadpath))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",cfg.pvtuploadpath),"Press any key to continue");
         }
         if(!ts_DirExists(cfg.nodelistdir  ) && !ts_MakeFullDir(cfg.nodelistdir  ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",cfg.nodelistdir  ),"Press any key to continue");
         }
         if(!ts_DirExists(cfg.pexpath      ) && !ts_MakeFullDir(cfg.pexpath      ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",cfg.pexpath      ),"Press any key to continue");
         }
         if(!ts_DirExists(doc_dir          ) && !ts_MakeFullDir(doc_dir          ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",(char *)doc_dir  ),"Press any key to continue");
         }
         if(!ts_DirExists(sdk_dir          ) && !ts_MakeFullDir(sdk_dir          ))
         {
            error = TRUE;
            show_error(form("Unable to create directory %s",(char *)sdk_dir  ),"Press any key to continue");
         }

         if(!error)
            break;
      }
   }
   else
   {
      bool error = FALSE;

      if(!ts_DirExists(doc_dir          ) && !ts_MakeFullDir(doc_dir          ))
      {
         error = TRUE;
         show_error(form("Unable to create directory %s",(char *)doc_dir  ),"Press any key to continue");
      }
      if(!ts_DirExists(sdk_dir          ) && !ts_MakeFullDir(sdk_dir          ))
      {
         error = TRUE;
         show_error(form("Unable to create directory %s",(char *)sdk_dir  ),"Press any key to continue");
      }

      if(error)
         return;
   }

   if(!ReadInstallCfg())
   {
      show_error("Error reading INSTALL.CFG", "Press any key to exit");
      return;
   }

   if(!CopyFiles())
      return;

   sprintf(proboard_env_buf,"PROBOARD=%s",(char *)dest_dir);

   putenv(proboard_env_buf);

   if(!do_update)
   {
      File f;

      if(!f.open(FileName(dest_dir , "CONFIG.PRO"),fmode_create))
         show_error("Error writing CONFIG.PRO","Press any key to continue");
      else
         f.write(&cfg,sizeof(cfg));
   }
   else
   {
      Window w( 1,
                1,
                tsw_hsize,
                tsw_vsize,
                0x7,
                NOBORDER | EXPLODE );
      w.open();

      w.attrib(0x8F);
      w << "Running conversion program...\n\n";
      w.attrib(7);

      FileName convert_exe("CONVERT.EXE");
      convert_exe.changePath(dest_dir);

      spawnl(P_WAIT,convert_exe,convert_exe,"PB",NULL);

      show_msg("Conversion complete - Press any key to continue");
   }


   ts_MakeInputPath(dest_dir);

   titlew.close();

   Window w;

   w.open( 1,
           1,
           70,
           (do_update ? 11:17),
           0x1F,
           CENTERED_X | CENTERED_Y | SHADOW | BRACKETS,
           CHISEL_BORDER,
           0x19,
           NULL,
           0x10 );

   w.title( "ProBoard v" VERSION, 0x1E );

   if(do_update)
   {
      w.centerLine(2,"Update complete",0x1F);
   }
   else
   {
      w.centerLine(2,"Installation complete",0x1F);

      w.centerLine(5,"Please add the following line to your AUTOEXEC.BAT file",0x1F);
      w.centerLine(7,form("SET PROBOARD=%s",(char *)dest_dir),0x1E);
   }

   w.centerLine( do_update ? 4:10, "Remember to run the REGISTER program to unlock your software",0x1F );

   w.centerLine( do_update ? 6:12, "Be sure to review the terms of DOC\\!LICENSE.TXT",0x1F );

   w.centerLine(do_update ? 8:14,"Press any key to exit",0x1B);

   KB.clear();
   KB.get();
}

static
bool
CopyFiles()
{
   Window w(1,10,60,22,0x3F,EXPLODE|SHADOW|CENTERED_X);
   w.open();
   w.title(" Installing ",0x3E);

   bool compressed = FALSE;

   FileName fn(source_dir,"PROBOARD.EX_");

   if(!access(fn,0))
      compressed = TRUE;

   FileName pbexe("PROBOARD.EXE");
   pbexe.changePath(dest_dir);

   int attr = 0;

   if(do_update)
   {
      attr = _chmod(pbexe,0);

      if(attr < 0)
         show_error("Error accessing PROBOARD.EXE","Press any key to continue");
      else
      {
         attr &= ~int(FA_RDONLY);

         _chmod(pbexe,1,attr);
      }
   }

   attr |= FA_RDONLY|FA_ARCH;

   int filecount = 0;
   int totalfiles = ll_file_dest.count();

   w.gauge(2,8,0x70,54,filecount,totalfiles,TRUE);

   for( ll_file_dest.rewind() ; !ll_file_dest.eol() ; ll_file_dest++)
   {
      FileName sname(ll_file_dest.get().file);
      FileName dname(ll_file_dest.get().file);
      FileName ext(sname);

      if(ext.find(".") >= 0)
         ext.del(0,ext.find(".")+1);

      if(compressed)
      {
         for( ll_ext_conv.rewind() ; !ll_ext_conv.eol() ; ll_ext_conv++)
         {
            if(ext == ll_ext_conv.get().uncompressed)
               sname.changeExt(ll_ext_conv.get().compressed);
         }
      }

      dname.changePath(ll_file_dest.get().dir);
      sname.changePath(source_dir);

      if(dname.find(".PBL")>=0 && do_update)
      {
         FileName fn(dname);

         fn.stripPath();

         Window w( 0,
                   13,
                   60,
                   20,
                   0x3F,
                   SHADOW | EXPLODE | CENTERED_X,
                   CHISEL_BORDER,
                   0x3B,
                   NULL,
                   0x31 );
         w.open();

         w.centerLine(2,form("Overwrite language file \"%s\"",(char *)fn),0x3F);
         w.centerLine(3,"with newer version ?",0x3F);
         w.centerLine(5,"[Y/N]",0x3E);

         if(!KB.ask())
         {
            filecount++;
            continue;
         }
      }

      w.centerLine(4,form("%-54s",""));
      w.centerLine(6,form("%-54s",""));
      w.centerLine(2,"Copying");
      w.centerLine(4,(char *)sname,0x3E);
      w.centerLine(5,"to");
      w.centerLine(6,(char *)dname,0x3E);

      if(compressed)
      {
         if(!decompress_file(sname,dname))
         {
            bool error = TRUE;

            sname.changeExt(ext);

            if(ts_CopyFile(sname,dname,8192) > 0)
               error = FALSE;

            if(error)
            {
               show_error(form("Error copying file %s!",(char*)sname),"Press any key to quit");
               return FALSE;
            }
         }

         ts_SyncFileDate(sname,dname);
      }
      else
      {
         if(ts_CopyFile(sname,dname,8192) < 0)
         {
            show_error("Error copying file!","Press any key to quit");
            return FALSE;
         }
      }

      filecount++;

      w.gauge(2,8,0x70,54,filecount,totalfiles,FALSE);
   }

   if(_chmod(pbexe,1,attr) < 0)
      show_error("Error setting read-only attribute for PROBOARD.EXE","Press any key to continue");



   return TRUE;
}


bool
InitConfig(_Config& cfg)
{
   CLEAR_OBJECT(cfg);

   dest_dir.appendBS();

   File f;

   if(!f.open(FileName(do_update ? dest_dir:source_dir ,do_update ? "CONFIG.PRO":"CONFIG.TPL")) || f.read(&cfg,sizeof(cfg)) != sizeof(cfg))
   {
      show_error(do_update ? "Unable to read existing CONFIG.PRO":"Unable to read config file CONFIG.TPL","Press any key to exit");

      return FALSE;
   }

   if(!do_update)
   {
      sprintf(cfg.txtpath      ,"%sTXTFILES"       , (char *) dest_dir);
      sprintf(cfg.mnupath      ,"%sMENUS"          , (char *) dest_dir);
      sprintf(cfg.msgpath      ,"%sMSGBASE"        , (char *) dest_dir);
      sprintf(cfg.uploadpath   ,"%sFILES\\UPLOADS" , (char *) dest_dir);
      sprintf(cfg.pexpath      ,"%sPEX"            , (char *) dest_dir);
      sprintf(cfg.nodelistdir  ,"%sNODELIST"       , (char *) dest_dir);
      sprintf(cfg.pvtuploadpath,"%sPVTFILES"       , (char *) dest_dir);
      sprintf(cfg.RIPpath      ,"%sRIP\\SCRIPTS"   , (char *) dest_dir);
      sprintf(cfg.iconspath    ,"%sRIP\\ICONS"     , (char *) dest_dir);
   }

   return TRUE;
}


bool
ProBoardInstalled(FileName dir)
{
   dir.appendBS();

   FileName ConfigPro(dir,"CONFIG.PRO");

   return !access(ConfigPro,0);
}
