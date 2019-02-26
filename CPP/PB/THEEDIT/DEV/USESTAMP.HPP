
/* ------------------------------------------------------------
 * Filename ............... UseStamp.Hpp
 *
 * General Purpose ........ UseStamp class
 * ------------------------------------------------------------
 * First date ............. 12-20-94 02:25pm
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

#ifndef __USESTAMP_HPP
#  define __USESTAMP_HPP

/*--] Classes [----------------------------------] UseStamp [--------------*/

   _ClassType(UseStamp)
   class UseStamp
   {
   protected:
      ChrT        fname[81];
      WrdT        value;

   protected:
      FlgT        read           (                                      );
      FlgT        write          (                                      );

   public:
      WrdT        days;
      FlgT        firstTime;
      FlgT        tampered;

   public:
                  UseStamp       (  ChrP path, ChrP project             );
   };

/*-------------------------------------------------------------------------*/

extern UseStampP   TUseStamp;

/*-------------------------------------------------------------------------*/

#endif                                 // __USESTAMP_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
