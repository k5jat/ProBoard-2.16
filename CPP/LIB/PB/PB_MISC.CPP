
/* ------------------------------------------------------------
 * Filename ............... Pb_Misc.Cpp
 *
 * General Purpose ........ Some misc ProBoard routines
 * ------------------------------------------------------------
 * First date ............. 14 sep 1993
 *
 * First in version ....... 1.40
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
#  include <string.h>
#endif

#include <ProBoard.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Routine   : getSystemPath()
 * Purpose   : get the ProBoard system path
 * ------------------------------------------------------------------------
 * Parameters: ARGV[0] parameter
 * Return    : Filename class with system path
 *
 */

FileName getSystemPath(ChrP argv)
{
   FileName sysPath(argv);
   ChrP p;

   if( (p = getenv("PROBOARD")) == NULL)   // try PROBOARD env var
      if( (p = getenv("PB")) == NULL)     // try PB env var
         if( (p = getenv("RA")) == NULL) // try RA env var
         {
            sysPath.stripName();
         }

   if(p != NULL)
      sysPath = p;

   sysPath.appendBS();
   sysPath.mixedCase();
   return(sysPath);
}

/*
 * Routine   : raCRC()
 * Purpose   : calculate the CRC code of a RA string
 * ------------------------------------------------------------------------
 * Parameters: string
 * Return    : CRC code
 *
 */

DwdT raCRC(ChrP str)
{
   DwdT ret_val = 0;

   String buffer(str);
   buffer.upperCase();

   if(buffer.len() != 0)
      ret_val = ~crc32(ChrP(buffer));

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
