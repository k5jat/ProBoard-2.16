
struct Archiver
   {
      char *desc;
      int ident_start;
      char *ident;
      char *compr_cmd;
      char *decompr_cmd;
   };

Archiver arc[] = {
                  { "Phil Katz's PkZip 2.04g" , 0, "\x50\x4b\x03\x04" , "pkzip -m %a %f" , "pkunzip -o %a" },
                  { "Robert Jung's ARJ"       , 0, "\x60\xea"         , "arj m %a %f"    , "arj e -y %a"   },
                  { "Phil Katz's PkPak (ARC)" , 0, "\x1A"             , "pkpak -m %a %f" , "pkunpak -r %a" },
                  { NULL , 0, NULL               , NULL             , NULL            }
                 };
