#include <stdio.h>
#include <string.h>
#include <tslib.hpp>

struct reg
   {
      int beta;
      int dist;
      char name[36];
      char bbs[36];
   };

static char *cryptext;
static int   crypt_ptr = 0;
static int   crypt_length;

inline void crypt(char *buf)
{
 *buf ^= cryptext[crypt_ptr] ^ (cryptext[0] * crypt_ptr);

 cryptext[crypt_ptr] += ((crypt_ptr < (crypt_length - 1)) ?
                         cryptext[crypt_ptr + 1] : cryptext[0]);

 if(!cryptext[crypt_ptr]) cryptext[crypt_ptr] += 1;

 if(++crypt_ptr >= crypt_length) crypt_ptr = 0;
}

static void
encrypt(char *text,int len,char *key)
{
 char x[101];
 strcpy(x,key);

 crypt_length = strlen(key);
 crypt_ptr = 0;
 cryptext = x;
 for(int i=0;i<len;i++) crypt(text++);
}

bool
read_key(File &f , reg &regis)
{
   int i;
   byte xor_data[100]=
   {
      0x79,0xDE,0xAA,0x71,0x3A,0x68,0x5D,0xBA,0xE7,0x2E,
      0x6D,0x4A,0xF2,0x2D,0x47,0x2A,0x12,0x0E,0xDC,0x0A,
      0xCB,0xF4,0xC5,0xC3,0x2D,0x99,0xE7,0x5E,0xE6,0xFA,
      0x8D,0x26,0xC5,0x52,0x16,0x56,0x2F,0xCB,0x8A,0x7F,
      0x59,0xB6,0x6C,0xFE,0x26,0x94,0x3D,0x48,0x35,0xEF,
      0x4F,0x99,0x1D,0x76,0x5B,0xD7,0x44,0x9F,0x4E,0x6D,
      0xBD,0x83,0x44,0xC1,0xAE,0xD7,0xA6,0x20,0x26,0xDC,
      0x69,0x90,0x4D,0x7C,0x6D,0x49,0xB7,0x66,0x51,0x57,
      0x49,0xAB,0x6F,0xF2,0x1E,0x3F,0x23,0x5E,0x7D,0x67,
      0xE7,0x43,0xA4,0x48,0x8B,0xDF,0x73,0xD7,0x20,0x29
   };

   if(f.len() != 1620)
   {
      return FALSE;
   }

   reg *kd = new reg[20];
   char key[101];

   f.read(key,100);
   key[100] = 0;
   f.read(kd,1520);
   f.close();

   for(i=0;i<100;i++) key[i] ^= xor_data[i];
   for(i=0;i<1520;i++) ((char *)kd)[i] ^= xor_data[i%100];

   encrypt((char *)kd,1520,key);

   regis = kd[0];

   for(i=1;i<20;i++)
      if(memcmp(&kd[i],&regis,sizeof(reg)))
      {
         delete [] kd;

         return FALSE;
      }

   delete [] kd;

   return TRUE;
}

main(int argc,char *argv[])
{
   reg key;
   File f;
   String fn("PROBOARD.KEY");

   if(argc > 1) fn = argv[1];

   printf("\n");

   if(!f.open(fn))
   {
      printf("File not found!\n");

      return 1;
   }

   if(!read_key(f,key))
   {
      printf("Illegal key!\n");
      return 1;
   }

   printf("Sysop Name  : %s\n"
          "BBS Name    : %s\n"
          "RegType     : %s %s\n",key.name,key.bbs,key.beta ? "[BETA]":"",key.dist ? "[DIST]":""
         );

   return 0;
}
