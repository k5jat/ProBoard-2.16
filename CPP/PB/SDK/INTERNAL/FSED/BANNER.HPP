
/* ------------------------------------------------------------
 * Filename ............... Banner.Hpp
 *
 * General Purpose ........ Banner class
 * ------------------------------------------------------------
 * First date ............. 12-20-94 11:09am
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

#ifndef __BANNER_HPP
#  define __BANNER_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __TIMER_HPP
#     include "timer.hpp"
#  endif

/*--] Classes [----------------------------------] Banner [----------------*/

   _ClassType(Banner)
   class Banner
   {
   protected:
      IntT        counter;
      FlgT        memWarning;
      Timer       timer;
      ChrP        message;
      IntT        waitTime;

   public:
                  Banner         (                                      );

      void        update         (                                      );
      void        redraw         (                                      );
      void        toggle         (                                      );
      void        unregistered   (                                      );
      void        putS           (  ChrP str                            );
   };

/*-------------------------------------------------------------------------*/

   extern BannerP TBanner;

/*-------------------------------------------------------------------------*/

#endif                                 //

/*---------------------------] END OF THE CODE [---------------------------*/
