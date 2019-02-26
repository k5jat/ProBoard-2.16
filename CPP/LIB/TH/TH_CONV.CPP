
/* ------------------------------------------------------------
 * Filename ............... Th_Conv.Cpp
 *
 * General Purpose ........ Some misc. convertion functions
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
 * Name       : CToPas()
 * Purpose    : convert the specified string to a pascal string
 * -------------------------------------------------------------------------
 * Parameters : reference to the string
 * Return     : reference to the string
 */

ChrP CToPas(ChrP str)
{
   ShrtT len = strlen(str);
   memmove(str + 1, str, len);
   str[0] = len;
   return(str);
}

/*
 * Name       : PasToC()
 * Purpose    : convert the specified string to a C string
 * -------------------------------------------------------------------------
 * Parameters : reference to the string
 * Return     : reference to the string
 */

ChrP PasToC(ChrP str)
{
   ShrtT len = str[0];
   memmove(str, str + 1, len);
   str[len] = 0;
   return(str);
}

/*---------------------------] END OF THE CODE [---------------------------*/
