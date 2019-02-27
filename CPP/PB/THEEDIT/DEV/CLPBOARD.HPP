
/* ------------------------------------------------------------
 * Filename ............... ClpBoard.Hpp
 *
 * General Purpose ........ Clipboard class
 * ------------------------------------------------------------
 * First date ............. 12-12-94 06:36pm
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

#ifndef __CLPBOARD_HPP
#  define __CLPBOARD_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __CONSOLE_HPP
#     include "console.hpp"
#  endif

/*--] Classes [----------------------------------] Clipboard [------------*/

   _ClassType(Clipboard)
   class Clipboard
   {
   private:
      ConsoleP    console;
      ChrI        lines;
      IntT        used;
      IntT        size;
      BytT        topLine;
      BytT        cursorY;

   public:
      BytT        attrib;
      BytT        upperY;
      BytT        height;
      FlgT        isOpen;
      FlgT        isActive;
      FlgT        needsUpdate;

   public:
                  Clipboard      (  ConsoleP con                        );
                  ~Clipboard     (                                      );

      void        release        (                                      );
      void        set            (  ChrP data                           );
      ChrP        get            (  IntT lineNo                         );
      FlgT        get            (  ChrP buffer                         );

      IntT        len            (                                      );
      IntT        curLine        (                                      );
      void        scroll         (                                      );
      FlgT        down           (                                      );
      FlgT        up             (                                      );
      FlgT        pageDown       (                                      );
      FlgT        pageUp         (                                      );
      FlgT        import         (  ChrP fname                          );
      void        update         (                                      );
      void        drawCursor     (                                      );
      WrdT        handle         (  WrdT key                            );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __CLPBOARD_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
