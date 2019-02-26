
/* ------------------------------------------------------------
 * Filename ............... Language.Hpp
 *
 * General Purpose ........ Language class
 * ------------------------------------------------------------
 * First date ............. 12-19-94 07:11pm
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

#ifndef __LANGUAGE_HPP
#  define __LANGUAGE_HPP

/*-------------------------------------------------------------------------*/

enum
{
   YesKey = 0    ,   YesText       ,   NoKey         ,   NoText        ,

   PressEscape   ,   EnterFilename ,   SavingMessage ,   SorryNoHelp   ,
   PressAnyKey   ,   WannaLoadQuote,   AlmostNoMem   ,   OutOfMemory   ,
   PleaseSave    ,   QuoteHeader   ,   FileHasBinChar,   AbbrevTitle   ,
   NoAbbrevsFound,

   InInsert      ,   InOverwrite   ,   InRawMode     ,   InNormal      ,
   InAbbrevs     ,   InNoAbbrevs   ,

   SaveItem      ,   AbortItem     ,   HelpItem      ,   RedrawItem    ,
   UploadItem    ,   ImportItem    ,   ExportItem    ,   QuoteItem     ,
   MessageItem   ,

   EndOfLanguage ,
};

/*--] Classes [----------------------------------] Language [--------------*/

   _ClassType(Language)
   class Language
   {
   protected:
      ChrI        strings;

   public:
                  Language       (                                      );
                  ~Language      (                                      );

      void        read           (  ChrP path, ChrP lang                );
      ChrP        get            (  IntT strNum                         );
   };

/*-------------------------------------------------------------------------*/

#endif

/*---------------------------] END OF THE CODE [---------------------------*/
