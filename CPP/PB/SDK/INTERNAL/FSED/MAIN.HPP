
/* ------------------------------------------------------------
 * Filename ............... Main.Hpp
 *
 * General Purpose ........ Main include file
 * ------------------------------------------------------------
 * First date ............. 12-18-94 10:50pm
 *
 * First in version ....... 2.00
 *
 * Written by ............. Alain Schellinck
 *
 * Copyright .............. Nephilim software development
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

#ifndef __MAIN_HPP
#  define __MAIN_HPP

/*-------------------------------------------------------------------------*/

#ifdef PEX
#  include <pb_sdk.h>
#else
#  include <bios.h>
#  include <ctype.h>
#  include <dir.h>
#  include <dos.h>
#  include <fcntl.h>
#  include <io.h>
#  include <stdarg.h>
#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  include <sys\types.h>
#  include <time.h>
#endif

/*-------------------------------------------------------------------------*/

#include "typedefs.hpp"
#include "version.hpp"

/*-------------------------------------------------------------------------*/

#  define   Black    0
#  define   Blue     1
#  define   Green    2
#  define   Cyan     3
#  define   Red      4
#  define   Magenta  5
#  define   Brown    6
#  define   White    7
#  define   HiLight  8
#  define   Blink    128

#  define   Color(fg, bg)  (fg | (bg << 4))

/*-------------------------------------------------------------------------*/

#endif

/*---------------------------] END OF THE CODE [---------------------------*/
