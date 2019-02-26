
/* ------------------------------------------------------------
 * Filename ............... Cursor.Hpp
 *
 * General Purpose ........ Cursor class
 * ------------------------------------------------------------
 * First date ............. 12-07-94 01:08pm
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

#ifndef __CURSOR_HPP
#  define __CURSOR_HPP

/*--] Classes [----------------------------------] Cursor [----------------*/

   _ClassType(Cursor)
   class Cursor
   {
   public:
      BytT        x;
      BytT        y;

   public:
                  Cursor         (                                      );

      void        gotoXY         (  BytT x, BytT y                      );
      void        hide           (                                      );
      void        unhide         (                                      );
      void        get            (                                      );
      FlgT        left           (  BytT chars = 1                      );
      FlgT        right          (  BytT chars = 1                      );
      FlgT        up             (  BytT lines = 1                      );
      FlgT        down           (  BytT lines = 1                      );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __CURSOR_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
