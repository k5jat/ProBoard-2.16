
/* ------------------------------------------------------------
 * Filename ............... Editor.Hpp
 *
 * General Purpose ........ Editor class
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

#ifndef __EDITOR_HPP
#  define __EDITOR_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __CLPBOARD_HPP
#     include "clpboard.hpp"
#  endif

#  ifndef __MESSAGE_HPP
#     include "message.hpp"
#  endif

/*--] Classes [----------------------------------] Editor [----------------*/

   _ClassType(Editor)
   class Editor
   {
   protected:
      ClipboardP  clipboard;
      ConsoleP    console;
      ChrT        buffer[128];

   public:
      MessageP    message;
      ClipboardP  quote;

   protected:
      // -- window functions --------------------------------------------
      void        update         (                                      );
      IntT        getHeight      (                                      );
      FlgT        cutLine        (                                      );
      FlgT        pasteLine      (                                      );
      FlgT        closeQuote     (                                      );

   public:
                  Editor         (  BytT ux, BytT uy, BytT lx, BytT ly,
                                    BytT att                            );
                  ~Editor        (                                      );

      void        cls            (                                      );
      FlgT        lowMemory      (                                      );
      FlgT        outOfMemory    (                                      );

      void        draw           (                                      );
      void        redraw         (                                      );
      WrdT        handle         (                                      );
      FlgT        openQuote      (                                      );
      FlgT        toggleQuote    (                                      );
   };

/*-------------------------------------------------------------------------*/

   void  TInit       (                                                  );

/*-------------------------------------------------------------------------*/

   extern EditorP TEditor;

/*-------------------------------------------------------------------------*/

#endif

/*---------------------------] END OF THE CODE [---------------------------*/

