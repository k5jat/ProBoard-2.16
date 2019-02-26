
/* ------------------------------------------------------------
 * Filename ............... Queue.Hpp
 *
 * General Purpose ........ Queue class
 * ------------------------------------------------------------
 * First date ............. 12-07-94 10:51am
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

#ifndef __QUEUE_HPP
#  define __QUEUE_HPP

/*--] Classes [----------------------------------] Queue [-----------------*/

   _ClassType(Queue)
   class Queue
   {
   private:
      WrdP        queue;
      BytT        size;
      BytT        used;

   public:
                  Queue          (  BytT s                              );
                  ~Queue         (                                      );

      WrdT        peek           (                                      );
      FlgT        poke           (  WrdT val                            );
      WrdT        get            (                                      );
      void        purge          (                                      );
      FlgT        isEmpty        (                                      );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __QUEUE_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
