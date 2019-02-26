
/* ------------------------------------------------------------
 * Filename ............... Terminal.Hpp
 *
 * General Purpose ........ Terminal class
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

#ifndef __TERMINAL_HPP
#  define __TERMINAL_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __CURSOR_HPP
#     include "cursor.hpp"
#  endif

/*--] Classes [----------------------------------] Terminal [--------------*/

   _ClassType(Terminal)
   class Terminal
   {
   protected:
      Cursor      cursor;
      BytT        attr;

   protected:
      void        send           (  ChrP fs, ...                        );

   public:
                  Terminal       (                                      );

      void        putC           (  ChrT c                              );
      void        putS           (  ChrP str                            );
      void        printf         (  ChrP fs, ...                        );
      BytT        attrib         (                                      );

      virtual FlgT   cls         (                                      )  { return False;   }
      virtual FlgT   gotoXY      (  BytT x, BytT y                      )  { return False;   }
      virtual FlgT   color       (  BytT attr                           )  { return False;   }
      virtual FlgT   clrEol      (                                      )  { return False;   }
      virtual FlgT   scrollUp    (  BytT start, BytT end, BytT lines    )  { return False;   }
      virtual FlgT   scrollDown  (  BytT start, BytT end, BytT lines    )  { return False;   }
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __TERMINAL_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
