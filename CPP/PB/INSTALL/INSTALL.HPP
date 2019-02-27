#define Use_LinkedList

#include <tswin.hpp>
#include <pb_lib.hpp>

#define BAR_NORM 0x4F
#define BAR_HIGH 0x4E

struct tExtConvert
   {
      String uncompressed;
      String compressed;
   };

struct tFileDestination
   {
      String dir;
      String file;
   };

bool EditDirectories();
bool ReadInstallCfg();
void show_error(char *s1,char *s2);
void show_message(char *s);

bool   compress_file(char *ifile,char *ofile,unsigned long &insize,unsigned long &outsize,void (*errf)(char *) = NULL);
bool decompress_file(char *ifile,char *ofile,void (*errf)(char *) = NULL);

extern _Config                      cfg;
extern LinkedList<tExtConvert>      ll_ext_conv;
extern LinkedList<tFileDestination> ll_file_dest;
extern FileName                     source_dir;  // Full path of where INSTALL.EXE is located
extern FileName                     dest_dir;    // Full path of where ProBoard should be installed
extern FileName                     doc_dir;     // Full path of where the documentation should be copied
extern FileName                     sdk_dir;     // Full path of where the sdk should be copied

extern char                         boot_drive;  // (uppercase 'A'-'Z')
extern bool                         do_update;   // Perform update instead of full install?

