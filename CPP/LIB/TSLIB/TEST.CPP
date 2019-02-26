#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>
#endif

#include <tslib.hpp>

main(int,char *argv[])
{
   char *lname = "jim biggs";
   char *uname = "JIM BIGGS";

   printf(" crc32(lname) = %08lX\n", crc32(lname));
   printf("~crc32(lname) = %08lX\n",~crc32(lname));
   printf(" crc32(lname) = %08lX\n", crc32(uname));
   printf("~crc32(lname) = %08lX\n",~crc32(uname));

   printf(" crc16(lname) = %04X\n", crc(lname));
   printf("~crc16(lname) = %04X\n",~crc(lname));
   printf(" crc16(lname) = %04X\n", crc(uname));
   printf("~crc16(lname) = %04X\n",~crc(uname));
}

