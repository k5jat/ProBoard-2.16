
/* ------------------------------------------------------------
 * Filename ............... Date.Hpp
 *
 * General Purpose ........ Date class
 * ------------------------------------------------------------
 * First date ............. 12-20-94 03:42pm
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

#ifndef __DATE_HPP
#  define __DATE_HPP

/*--] Classes [----------------------------------] Date [------------------*/

   _ClassType(Date)
   class Date
   {
   protected:

   public:
      IntT        _d;
      IntT        _m;
      IntT        _y;

   public:
                  Date           (                                      );
                  Date           (  IntT d, IntT m, IntT y              );

      void        set            (  IntT day, IntT month, IntT year     );
      IntT        compare        (  DateR dt                            );
      FlgT        isValid        (                                      );
      FlgT        isLeap         (                                      );
      IntT        weekDay        (                                      );
      IntT        daysInMonth    (  IntT m = 0                          );
      IntT        daysInYear     (  IntT y = 0                          );
      IntT        dayNum         (                                      );
      WrdT        pack           (                                      );
      void        unpack         (  WrdT packed                         );

                  operator LngT  (                                      );

      IntT        operator <     (  DateR d                             );
      IntT        operator >     (  DateR d                             );
      IntT        operator ==    (  DateR d                             );
      IntT        operator !=    (  DateR d                             );
      IntT        operator >=    (  DateR d                             );
      IntT        operator <=    (  DateR d                             );
      IntT        operator -     (  DateR d                             );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __DATE_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
