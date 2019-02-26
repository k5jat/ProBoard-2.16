
/* ------------------------------------------------------------
 * Filename ............... Screen.Hpp
 *
 * General Purpose ........ Screen class
 * ------------------------------------------------------------
 * First date ............. 12-08-94 09:47am
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

#ifndef __SCREEN_HPP
#  define __SCREEN_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __TTERMINL_HPP
#     include "terminal.hpp"
#  endif

#  ifndef __TVIDEO_HPP
#     include "video.hpp"
#  endif

/*--] Classes [----------------------------------] Screen [----------------*/

   _ClassType(Screen)
   class Screen
   {
   private:
      VideoP      local;
      TerminalP   remote;
      FlgT        remoteEnabled;

   public:
      BytT        height;
      BytT        width;
      BytT        mode;

   public:
                  Screen         (                                      );
                  ~Screen        (                                      );

      void        remoteOn       (                                      )  {  remoteEnabled = True;   }
      void        remoteOff      (                                      )  {  remoteEnabled = False;  }

      void        cls            (                                      );
      void        color          (  BytT attr                           );
      void        gotoXY         (  BytT x, BytT y                      );
      FlgT        scrollUp       (  BytT start, BytT end, BytT lines    );
      FlgT        scrollDown     (  BytT start, BytT end, BytT lines    );
      void        clrEol         (                                      );
      void        putC           (  ChrT c                              );
      void        putS           (  ChrP s                              );
      void        printf         (  ChrP fs, ...                        );
      void        cursorOff      (                                      );
      void        cursorOn       (                                      );
      BytT        attrib         (                                      );
      void        centerLine     (  BytT lineNo, BytT attr, ChrP fs, ...);
   };

/*-------------------------------------------------------------------------*/

   extern ScreenP TScreen;

/*-------------------------------------------------------------------------*/

#endif                                 // __SCREEN_HPP

/*---------------------------] END OF THE CODE [---------------------------*/

