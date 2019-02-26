
/* ------------------------------------------------------------
 * Filename ............... Menu.Hpp
 *
 * General Purpose ........ Menu class
 * ------------------------------------------------------------
 * First date ............. 12-20-94 11:37am
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

#ifndef __MENU_HPP
#  define __MENU_HPP

/*--] Classes [----------------------------------] Menu [------------------*/

   _ClassType(Menu)
   class Menu
   {
   protected:
      BytT        darkAttr;
      BytT        highAttr;
      BytT        normAttr;

   protected:
      ChrT        drawItem       (  WrdT item                           );

   public:
                  Menu           (                                      );

      IntT        get            (                                      );
      FlgT        save           (  ChrP name                           );
      FlgT        help           (  ChrP path                           );
      FlgT        upload         (  ChrP path                           );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __MENU_HPPx

/*---------------------------] END OF THE CODE [---------------------------*/
