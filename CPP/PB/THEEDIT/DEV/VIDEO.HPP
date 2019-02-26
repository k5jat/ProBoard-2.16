
/* ------------------------------------------------------------
 * Filename ............... Video.Hpp
 *
 * General Purpose ........ Video class
 * ------------------------------------------------------------
 * First date ............. 12-05-94 03:02pm
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

#ifndef __VIDEO_HPP
#  define __VIDEO_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __CURSOR_HPP
#     include "cursor.hpp"
#  endif

/*--] Classes [----------------------------------] Video [-----------------*/

   _ClassType(Video)
   class Video
   {
   private:
      Cursor      cursor;
      BytT        attr;
      ChrFP       videoAddr;

   protected:
      void        setVideoAddr   (                                      );

   public:
                  Video         (                                      );

      void        scrollUp       (  BytT start, BytT end, BytT lines    );
      void        scrollDown     (  BytT start, BytT end, BytT lines    );
      void        cls            (                                      );
      void        gotoXY         (  BytT x, BytT y                      );
      void        putC           (  ChrT c                              );
      void        putS           (  ChrP str                            );
      void        printf         (  ChrP fs, ...                        );
      void        color          (  BytT attr                           );
      void        clrEol         (                                      );
      BytT        attrib         (                                      );
      void        cursorOff      (                                      );
      void        cursorOn       (                                      );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __VIDEO_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
