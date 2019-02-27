
/* ------------------------------------------------------------
 * Filename ............... Console.Hpp
 *
 * General Purpose ........ Console class
 * ------------------------------------------------------------
 * First date ............. 12-06-94 10:24am
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

#ifndef __CONSOLE_HPP
#  define __CONSOLE_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __CONLINE_HPP
#     include "conline.hpp"
#  endif

/*--] Classes [----------------------------------] Console [---------------*/

	_ClassType(Console)
	class Console
	{
	private:
		ConLineI    lines;
		BytT        defaultAttr;
		BytT        upperX, upperY;
		BytT        lowerX, lowerY;

   public:
      BytT        width, height;

   public:
                  Console        (  BytT ux, BytT uy, BytT lx, BytT ly,
                                    BytT defAttr                        );
                  ~Console       (                                      );

      BytT        attrib         (                                      );
      void        cls            (                                      );
      void        setLine        (  BytT lineNo, ChrP text, BytT attr   );
      void        clearLine      (  BytT lineNo                         );
      void        gotoXY         (  BytT x, BytT y                      );
      void        draw           (                                      );
      void        redraw         (                                      );
      void        scrollUp       (  BytT uy, BytT h                     );
      void        scrollDown     (  BytT uy, BytT h                     );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __CONSOLE_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
