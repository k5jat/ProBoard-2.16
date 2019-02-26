
/* ------------------------------------------------------------
 * Filename ............... Header.Hpp
 *
 * General Purpose ........ header class
 * ------------------------------------------------------------
 * First date ............. 12-19-94 09:01pm
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

#ifndef __HEADER_HPP
#  define __HEADER_HPP

/*-------------------------------------------------------------------------*/

/*--] Classes [----------------------------------] Header [----------------*/

   _ClassType(Header)
   class Header
   {
   protected:
      FlgT        insertMode;
      FlgT        rawMode;
      FlgT        abbrevMode;
      ChrP        path;

   public:
                  Header         (  ChrP p                              );

      void        redraw         (                                      );
      void        draw           (                                      );
      void        update         (                                      );
   };

/*-------------------------------------------------------------------------*/

   extern HeaderP THeader;

/*-------------------------------------------------------------------------*/

#endif                                 // __HEADER__HPP

/*---------------------------] END OF THE CODE [---------------------------*/
