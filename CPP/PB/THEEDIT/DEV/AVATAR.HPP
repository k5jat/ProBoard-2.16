
/* ------------------------------------------------------------
 * Filename ............... Avatar.Hpp
 *
 * General Purpose ........ Avatar class
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

#ifndef __AVATAR_HPP
#  define __AVATAR_HPP

/*-------------------------------------------------------------------------*/

#  ifndef _ANSI_HPP
#     include "ansi.hpp"
#  endif

/*--] Classes [----------------------------------] Avatar [----------------*/

   _ClassType(Avatar)
   class Avatar : public Ansi
   {
   public:
      virtual FlgT   gotoXY      (  BytT x, BytT y                      );
      virtual FlgT   color       (  BytT attr                           );
      virtual FlgT   clrEol      (                                      );
      virtual FlgT   scrollUp    (  BytT start, BytT end, BytT lines    );
      virtual FlgT   scrollDown  (  BytT start, BytT end, BytT lines    );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __AVATAR_HPP

/*---------------------------] END OF THE CODE [---------------------------*/







