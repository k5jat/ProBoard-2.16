
/* ------------------------------------------------------------
 * Filename ............... ConLine.Hpp
 *
 * General Purpose ........ ConLine class
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

#ifndef __CONLINE_HPP
#  define __CONLINE_HPP

/*--] Classes [----------------------------------] ConLine [---------------*/

   _ClassType(ConLine)
   class ConLine
   {
   private:
      ChrP        origText;
      BytT        origAttr;
      ChrP        copyText;
      BytT        copyAttr;
      BytT        width;
      FlgT        empty;

   public:
                  ConLine        (  BytT w, BytT attr                   );
                  ~ConLine       (                                      );

      void        clear          (                                      );
      void        clearCopy      (                                      );
      void        set            (  ChrP t                              );
      void        set            (  BytT a                              );
      ChrP        get            (                                      );
      IntT        get            (  ChrP buffer                         );
      BytT        attrib         (                                      );
      FlgT        isEmpty        (                                      )  { return empty;   }
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __CONLINE_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
