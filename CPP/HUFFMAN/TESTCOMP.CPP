#include <stdio.h>
#include <stdlib.h>
#include <tslib.hpp>

bool   compress_file(char *ifile,char *ofile,unsigned long &insize,unsigned long &outsize,void (*errf)(char *) = NULL);
bool decompress_file(char *ifile,char *ofile,void (*errf)(char *) = NULL);

void
main(int argc,char*argv[])
{
   unsigned long insize,outsize;

   if(argc != 4)
   {
      printf("\nUsage: TESTCOMP <C|D> <infile> <outfile>\n");
      exit(1);
   }

   if(argv[1][0] == 'C')
   {
      printf("Compressing <%s> to <%s>...",argv[2],argv[3]);

      compress_file(argv[2],argv[3],insize,outsize);

      printf("\n\nInput  Size = %lu\n",insize);
      printf(    "Output Size = %lu\n\n",outsize);

      printf("Compression ratio = %lu%%\n",100L * (insize-outsize)/insize);
   }

   if(argv[1][0] == 'D')
   {
      printf("Decompressing <%s> to <%s>...",argv[2],argv[3]);
      decompress_file(argv[2],argv[3]);
   }

}
