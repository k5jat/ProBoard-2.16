
/* ------------------------------------------------------------
 * Filename ............... Timer.Hpp
 *
 * General Purpose ........ Timer class
 * ------------------------------------------------------------
 * First date ............. 12-04-94 08:59pm
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

#ifndef __TIMER_HPP
#  define __TIMER_HPP

/*--] Classes [----------------------------------] Timer [-----------------*/

   class Timer
   {
   protected:
      FlgT     stopped;
      DwdT     startVal;
      DwdT     stopVal;

   protected:
      DwdT        get            (                                      );

   public:
                  Timer          (                                      );

      void        start          (                                      );
      void        stop           (                                      );
      DwdT        elapsed        (                                      );
      FlgT        isStopped      (                                      );
   };

/*-------------------------------------------------------------------------*/

   void           sleepClock     (  IntT ticks                          );

/*-------------------------------------------------------------------------*/

#endif                                 // __TIMER_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
