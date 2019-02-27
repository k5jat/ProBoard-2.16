#include <stdio.h>
#include <string.h>
#include <tslib.hpp>
#include "..\pbkey.hpp"

int   decompress_data(byte *input , byte *output , int isize , int outsize);
void  write_readme(char *fname,KeyData *key);

main(int argc,char *argv[])
{
   int i , output_size;
   File f;
   KeyFile *kf = new KeyFile;
   RandomTable *random_table = new RandomTable[32];

   if(argc != 3)
   {
      printf("\nUsage: MKREADME <keyfile> <readme>\n");

      return 1;
   }

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

   if(!f.open(argv[1]))
   {
      printf("Key file not found\n");
      return 1;
   }

   output_size = int(f.len() - 33);

   byte *output_data = new byte[output_size];

   f.seek(33);
   f.read(output_data , output_size);
   f.close();

   for(i = 0 ; i < output_size ; i++)
   {
      output_data[i] ^= fixed_xor_data[i % 100];
   }

   decompress_data(output_data , (byte *)kf , output_size , sizeof(*kf));

   encrypt((char *)kf->keyData,sizeof(kf->keyData),kf->xorData);

   bool valid = TRUE;

   if(
         kf->keyCrc != crc32(kf->keyData , sizeof(kf->keyData))
      || kf->xorCrc != crc32(kf->xorData , sizeof(kf->xorData))
     )
   {
      printf("Illegal CRC!\n");

      valid = FALSE;
   }

   for(i=0;i<32;i++)
      if(kf->securityCheck[i] != calc_security(&kf->keyData[0] , random_table[i].data1, random_table[i].data2))
      {
         valid = FALSE;
      }

   if(!valid)
   {
      printf("\nINVALID KEY!\n");
      return 1;
   }

   write_readme(argv[2],kf->keyData);

   return 0;
}


void
write_readme(char *fname,KeyData *key)
{
   FILE *fp = fopen(fname,"wt");

   if(fp == NULL)
   {
      printf("Error creating output file!\n");
      return;
   }

   fprintf(fp,"\n"
              "Thank you for registering ProBoard.  To install your key file, copy\n"
              "REGKEY.PB to your ProBoard system directory and make sure the information\n"
              "entered in ProCFG/Options/Site Info matches this:\n\n");

   fprintf(fp,"SysOp Name    : `%s'\n",key->sysopName);
   fprintf(fp,"System Name   : `%s'\n",key->bbsName);

   fprintf(fp,"\n(don't add the quotes!)\n\n");

   if(key->commercial) fprintf(fp,"The supplied key file is valid for commercial use\n\n");

   fclose(fp);
}
