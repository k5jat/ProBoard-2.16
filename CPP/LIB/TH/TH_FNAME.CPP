
/* ------------------------------------------------------------
 * Filename ............... Th_Fname.Cpp
 *
 * General Purpose ........ Some misc. filename functions
 * ------------------------------------------------------------
 * First date ............. 14 sep 1993
 *
 * First in version ....... 2.00
 *
 * Written by ............. Alain Schellinck
 * ------------------------------------------------------------
 * Revisions:
 * ----------
 *
 *   Date   | By |                  Purpose                   |
 * ---------+----+--------------------------------------------+
 *          |    |                                            |
 *          |    |                                            |
 *          |    |                                            |
 */

/*-------------------------------------------------------------------------*/

#ifndef PEX
#  include <stdlib.h>
#  include <stdio.h>
#  include <dos.h>
#  include <string.h>
#endif

#include <TheHouse.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Name       : getUniqueName()
 * Purpose    : get a unique name in a directory
 * -------------------------------------------------------------------------
 * Parameters : directory, extension
 * Return     : Filename (without extension and path)
 */

FileName getUniqueName(FileName dir, String ext)
{
   static FileName file;

   Time time(NOW);
   Date date(TODAY);

   for(;;)
   {
      file = "";                          // build DDHHMMSS filename
      file << date.format("DD") << time.format("HHMMSS");

      FileName full(dir);                     // build complete path
      full.appendBS();
      full << file << ext;
      full.mixedCase();

      DirScan find;
      if(find.first(full) == FALSE)
      {
         sleep(1);
         break;
      }
   }

   return(file);
}

/*---------------------------] END OF THE CODE [---------------------------*/
