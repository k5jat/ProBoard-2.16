
/* ------------------------------------------------------------
 * Filename ............... Ansi.Hpp
 *
 * General Purpose ........ Ansi class
 * ------------------------------------------------------------
 * First date ............. 12-05-94 03:02pm
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

#ifndef __ANSI_HPP
#  define __ANSI_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __TERMINAL_HPP
#     include "terminal.hpp"
#  endif

/*--] Classes [----------------------------------] Ansi [------------------*/

   _ClassType(Ansi)
   class Ansi : public Terminal
   {
   public:
      virtual FlgT   cls         (                                      );
      virtual FlgT   gotoXY      (  BytT x, BytT y                      );
      virtual FlgT   color       (  BytT attr                           );
      virtual FlgT   clrEol      (                                      );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __ANSI_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
