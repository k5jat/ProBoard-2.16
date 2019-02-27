
/* ------------------------------------------------------------
 * Filename ............... Rows.Hpp
 *
 * General Purpose ........ Rows class
 * ------------------------------------------------------------
 * First date ............. 12-06-94 03:12pm
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

#ifndef __ROWS_HPP
#  define __ROWS_HPP

/*--] Structures [-------------------------------] Rows [------------------*/

   _StructType(Rows)
   struct Rows
   {
   public:
      WrdP        lines;
      IntT        used;

   public:
                  Rows           (                                      );
                  ~Rows          (                                      );

      void        clear          (                                      );
      FlgT        exist          (  IntT lineNo                         );
      FlgT        set            (  IntT lineNo, WrdT offset            );
      WrdT        get            (  IntT lineNo                         );
      IntT        last           (                                      );
      WrdT        llen           (  IntT lineNo, WrdT textLen           );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __ROWS_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
