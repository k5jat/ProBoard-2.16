#include <string.h>
#include "install.hpp"

static
void
EditDirHelp(int field)
{
   char *helpText[]=
   {
      "Drive+Directory for ANS/ASC files",
      "Drive+Directory for RIP sequences",
      "Drive+Directory for RIP icons used in RIP screens",
      "Drive+Directory for menu files (*.MNU)",
      "Drive+Directory for message base files and user files",
      "Drive+Directory where uploads should be placed",
      "Drive+Directory where private uploads (user-to-user) should be placed",
      "Drive+Directory where raw nodelist is located",
      "Drive+Directory from where .PEX files should be loaded",
      "Drive+Directory where the documentation files should be copied to",
      "Drive+Directory where the SDK files should be copied to"
   };

   tsw_showfooter(helpText[field], BAR_HIGH );
}


bool
EditDirectories()
{
   static char s_doc_dir[80],
               s_sdk_dir[80];

   KEY hotkeys[] = { KEY_F3 , KEY_F10 , 0 };

   Field frm[]=
   {
      /* TYPE           dataPtr                 choices     hookFunc    flags       len      width    x     y     */
      /* -------------------------------------------------------------------------------------------------------  */
      { FRM_STRING      ,cfg.txtpath            ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,2    },
      { FRM_STRING      ,cfg.RIPpath            ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,3    },
      { FRM_STRING      ,cfg.iconspath          ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,4    },
      { FRM_STRING      ,cfg.mnupath            ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,5    },
      { FRM_STRING      ,cfg.msgpath            ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,6    },
      { FRM_STRING      ,cfg.uploadpath         ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,7    },
      { FRM_STRING      ,cfg.pvtuploadpath      ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,8    },
      { FRM_STRING      ,cfg.nodelistdir        ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,9    },
      { FRM_STRING      ,cfg.pexpath            ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,10   },
      { FRM_STRING      ,s_doc_dir              ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,11   },
      { FRM_STRING      ,s_sdk_dir              ,0          ,0          ,FRM_UPPER  ,60     ,33       ,25   ,12   }
   };

   FormWindow w( 1, 
                 1, 
                 62, 
                 15, 
                 0x3F, 
                 SHADOW | EXPLODE | CENTERED_X | CENTERED_Y | BRACKETS,
                 CHISEL_BORDER,
                 0x3B,
                 NULL,
                 0x31 );
   w.open();
   w.title( "Enter directories to install ProBoard in", 0x3E );

   w << "\n            Text files:\n"
          "             RIP files:\n"
          "             RIP icons:\n"
          "                 Menus:\n"
          " Messages & User files:\n"
          "          User uploads:\n"
          "       Private uploads:\n"
          "              Nodelist:\n"
          "             PEX files:\n"
          "         Documentation:\n"
          "          ProBoard SDK:";

   w.direct( 5,
             14,
             0x3E,
             "[ F3 - Exit Install ]" );
             
   w.direct( 33,
             14,
             0x3E,
             "[ F10/Esc - Continue ]" );

   w.define( frm, 
             11, 
             0x31, 
             0x4E, 
             hotkeys, 
             EditDirHelp );

   strcpy(s_doc_dir,doc_dir);
   strcpy(s_sdk_dir,sdk_dir);

   for(;;)
   {
      tsw_cursoron();
      int ret = w.process();
      tsw_cursoroff();

      tsw_fillrect(1,tsw_vsize,tsw_hsize,tsw_vsize,'°',0x70);

      if(ret == ED_HOT && w.scanHotKey==KEY_F3)
         return FALSE;

      doc_dir = s_doc_dir;
      sdk_dir = s_sdk_dir;

      if(strncmp(&cfg.txtpath[1],":\\",2))
      {
         tsw_beep();
         w.setField(0);
         continue;
      }
      if(strncmp(&cfg.RIPpath[1],":\\",2))
      {
         tsw_beep();
         w.setField(1);
         continue;
      }
      if(strncmp(&cfg.iconspath[1],":\\",2))
      {
         tsw_beep();
         w.setField(2);
         continue;
      }
      if(strncmp(&cfg.mnupath[1],":\\",2))
      {
         tsw_beep();
         w.setField(3);
         continue;
      }
      if(strncmp(&cfg.msgpath[1],":\\",2))
      {
         tsw_beep();
         w.setField(4);
         continue;
      }
      if(strncmp(&cfg.uploadpath[1],":\\",2))
      {
         tsw_beep();
         w.setField(5);
         continue;
      }
      if(strncmp(&cfg.pvtuploadpath[1],":\\",2))
      {
         tsw_beep();
         w.setField(6);
         continue;
      }
      if(strncmp(&cfg.nodelistdir[1],":\\",2))
      {
         tsw_beep();
         w.setField(7);
         continue;
      }
      if(strncmp(&cfg.pexpath[1],":\\",2))
      {
         tsw_beep();
         w.setField(8);
         continue;
      }
      if(strncmp(&s_doc_dir[1],":\\",2))
      {
         tsw_beep();
         w.setField(9);
         continue;
      }
      if(strncmp(&s_sdk_dir[1],":\\",2))
      {
         tsw_beep();
         w.setField(10);
         continue;
      }

      break;
   }

   doc_dir.appendBS();
   sdk_dir.appendBS();

   return TRUE;
}


