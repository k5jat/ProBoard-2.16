
/* ------------------------------------------------------------
 * Filename ............... Th_File.Cpp
 *
 * General Purpose ........ Some misc. file functions
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
 * Name       : isUpToDate()
 * Purpose    : check if the destionation file is up-to-date
 * -------------------------------------------------------------------------
 * Parameters : source file, destination file
 * Return     : TRUE if up to date, FALSE if not
 */

FlgT isUpToDate(ChrP src, ChrP dst)
{
   DirScan d_src;
   DirScan d_dst;

   if(d_src.first(src) == FALSE)       // src not found, so dst is up to date
       return(TRUE);

   if(d_dst.first(dst) == FALSE)       // dst not found, so dst isn't ip to date
       return(FALSE);

   Date d1=d_src.date();
   Date d2=d_dst.date();

   LngT diff = d1 - d2;
   if(diff <  0) return(TRUE );        // src older, dst is up to date!
   if(diff >  0) return(FALSE);        // dst older, dst isn't up to date!
   if(diff == 0)
   {
      Time t1=d_src.time();
      Time t2=d_dst.time();

       diff = t1 - t2;
       if(diff <  0) return(TRUE );    // src older, dst is up to date!
       if(diff >  0) return(FALSE);    // dst older, dst isn't up to date!
   }

   return(TRUE );                      // same date & time, dst is up to date
}

/*
 * Name       : copyFile
 * Purpose    : copy a source file to destionation file
 * -------------------------------------------------------------------------
 * Parameters : two file handles
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT copyFile(File& src, File& dst)
{
   FlgT ret_val = TRUE;

   ChrP buffer = new ChrT[defaultBufferSize];
   for(;;)
   {
       IntT bytes;

       bytes = src.read(buffer, defaultBufferSize);
       if(bytes > 0)
           if(dst.write(buffer, bytes) < bytes)
           {
               ret_val = FALSE;
               break;
           }

       if(bytes < defaultBufferSize)
           break;
   }

   delete buffer;

   return(ret_val);
}

/*
 * Name       : copyBlock
 * Purpose    : copy a part of the source file to destionation file
 * -------------------------------------------------------------------------
 * Parameters : two file handles, block size
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT copyBlock(File& src, File& dst, LngT length)
{
   FlgT ret_val = TRUE;
   LngT bytesLeft = length;

   ChrP buffer = new ChrT[defaultBufferSize];
   for(;;)
   {
       IntT bytes;
       IntT bytesToRead = int(min(bytesLeft, LngT(defaultBufferSize)));

       bytes = src.read(buffer, bytesToRead);
       if(bytes == 0)
         break;

       if(dst.write(buffer, bytes) < bytes)
       {
           ret_val = FALSE;
           break;
       }

       bytesLeft -= bytes;
       if(bytes < bytesToRead)
           break;
   }

   delete buffer;

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
