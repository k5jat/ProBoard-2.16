#include <stdio.h>
#include <string.h>
#include <tslib.hpp>
#include "..\pbkey.hpp"

int   decompress_data(byte *input , byte *output , int isize , int outsize);
void  showkey(KeyData *key);

main()
{
   int i , output_size;
   File f;
   KeyFile *kf = new KeyFile;
   RandomTable *random_table = new RandomTable[32];

   if(!f.open("GENKEY.RND",fmode_read))
   {
      printf("Error reading random file\n");
      return 1;
   }

   f.read(random_table, 2048);
   f.close();

   if(!f.open("REGKEY.PB"))
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

   printf("\nChecksums: ");

   for(i=0;i<32;i++)
      if(kf->securityCheck[i] != calc_security(&kf->keyData[0] , random_table[i].data1, random_table[i].data2))
      {
         printf("X");
         valid = FALSE;
      }
      else
      {
         printf("û");
      }

   printf("\n");

   showkey(kf->keyData);

   if(valid)
      printf("\n\nThis is a valid key.\n");
   else
      printf("\n\nThis is an illegal key!\n");

   return 0;
}


void
showkey(KeyData *key)
{
   printf("\nSysOp Name    : `%s'",key->sysopName);
   printf("\nBBS Name      : `%s'",key->bbsName);
   printf("\nCreated by    : %s",key->createdBy);
   printf("\nCreation date : %02d/%02d/%04d at %02d:%02d:%02d",
                                 key->createDate[0],
                                 key->createDate[1],
                                 key->createDate[2]+1900,
                                 key->createTime[0],
                                 key->createTime[1],
                                 key->createTime[2]);
   printf("\nExpir. date   : %02d/%02d/%04d",
                                 key->expirDate[0],
                                 key->expirDate[1],
                                 key->expirDate[2]+1900);
   printf("\nExpir. version: %X.%02X",key->expirVersion / 0xFF,
                                        key->expirVersion & 0xFF);

   printf("\nFlags         :");
   if(key->alpha)      printf(" [ALPHA]");
   if(key->beta )      printf(" [BETA]");
   if(key->gamma)      printf(" [GAMMA]");
   if(key->distSite)   printf(" [DIST]");
   if(key->regSite)    printf(" [REG]");
   if(key->commercial) printf(" [COMMERC]");
}
