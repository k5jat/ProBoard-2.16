
/* ------------------------------------------------------------
 * Filename ............... Text.Hpp
 *
 * General Purpose ........ Text class
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

#ifndef __TEXT_HPP
#  define __TEXT_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __ROWS_HPP
#     include "rows.hpp"
#  endif

#  ifndef __CONSOLE_HPP
#     include "console.hpp"
#  endif

#  ifndef __ABREV_HPP
#     include "abbrev.hpp"
#  endif

/*--] Struct [-----------------------------------] Point [-----------------*/

   _StructType(Point)
   struct Point
   {
      BytT        x;
      BytT        y;
   };

/*--] Class [------------------------------------] Text [------------------*/

   _ClassType(Text)
   class Text
   {
   protected:
      RowsT       rows;
      PointT      cursor;
      ConsoleP    console;
      ChrP        text;
      WrdT        length;
      AbbrevList *abbrevList;

   public:
      WrdT        curPos;
      WrdT        topPos;
      WrdT        botPos;

   protected:
      IntT        topLine;
      ChrT        buffer[128];

   public:
      FlgT        needsUpdate;
      FlgT        editable;
      BytT        upperY;
      BytT        width;
      BytT        height;
      BytT        attrib;

   protected:
      FlgT        makeGap        (  WrdT gapSize                        );
      void        findCursor     (                                      );
      void        setPos         (  BytT x, IntT line                   );
      FlgT        scrollUp       (                                      );
      FlgT        scrollDown     (                                      );
      FlgT        isSeparator    (  ChrT c                              );

      public:
                  Text           (  ConsoleP con                        );
                  ~Text          (                                      );

      WrdT        getPos         (                                      );
      void        seek           (  WrdT pos                            );
      WrdT        len            (                                      )  {  return length;    }
      WrdT        rLen           (                                      )  {  return rows.used; }
      FlgT        clear          (                                      );
      void        setAbbrev      (  AbbrevList * abbrevs                );

      // --- flags changing ---------------------------------------------
      virtual  void  toggleInsert(                                      );
      virtual  void  toggleAbbrev(                                      );

      // --- text processing --------------------------------------------
      virtual  IntT  curLine     (                                      );
      virtual  ChrP  get         (  IntT lineNo                         );
      virtual  void  format      (                                      );
      virtual  FlgT  addChar     (  ChrT c                              );
      virtual  FlgT  backSpace   (                                      );
      virtual  FlgT  delChar     (                                      );
      virtual  FlgT  delWord     (                                      );
      virtual  FlgT  cutLine     (                                      );
      virtual  FlgT  pasteLine   (  ChrP t                              );
      virtual  WrdT  percent     (                                      );
      virtual  ChrP  getLastWord (                                      );
      virtual  ChrP  getLastSentence   (                                );

      // --- file processing --------------------------------------------
      virtual  WrdT  import      (  ChrP fileName                       );
      virtual  FlgT  export      (  ChrP fileName, ChrP footer = NULL   );

      // --- cursor movement --------------------------------------------
      virtual  void  drawCursor  (                                      );
      virtual  FlgT  left        (                                      );
      virtual  FlgT  right       (                                      );
      virtual  FlgT  up          (                                      );
      virtual  FlgT  down        (                                      );
      virtual  FlgT  pageUp      (                                      );
      virtual  FlgT  pageDown    (                                      );
      virtual  FlgT  home        (                                      );
      virtual  FlgT  end         (                                      );
      virtual  FlgT  wordLeft    (                                      );
      virtual  FlgT  wordRight   (                                      );
      virtual  FlgT  centerLine  (                                      );

      virtual  void  update      (                                      );
      virtual  WrdT  handle      (  WrdT key                            );
   };

/*-------------------------------------------------------------------------*/

extern "C"
{
   WrdT  FormatText (  ChrP, WrdP, WrdT, WrdT, WrdT                     );
}

/*-------------------------------------------------------------------------*/

#endif                                 // __TEXT_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
