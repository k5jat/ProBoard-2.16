
/* ------------------------------------------------------------
 * Filename ............... Message.Hpp
 *
 * General Purpose ........ Message class
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

#ifndef __MESSAGE_HPP
#  define __MESSAGE_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __TEXT_HPP
#     include "text.hpp"
#  endif

/*--] Class [------------------------------------] Message [---------------*/

   _ClassType(Message)
   class Message : public Text
   {
   public:
      BytT        qAttrib;

   public:
                  Message        (  ConsoleP con                        )
                  : Text(con)
                  {
                  }

      FlgT        getQuote       (  IntT lineNo, ChrP buffer            );
      FlgT        addChar        (  ChrT c                              );
      void        update         (                                      );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __MESSAGE_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
