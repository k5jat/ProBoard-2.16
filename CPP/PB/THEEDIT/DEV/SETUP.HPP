
/* ------------------------------------------------------------
 * Filename ............... Setup.Hpp
 *
 * General Purpose ........ Setup class
 * ------------------------------------------------------------
 * First date ............. 12-19-94 07:32pm
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

#ifndef __SETUP_HPP
#  define __SETUP_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __LANGUAGE_HPP
#     include "language.hpp"
#  endif

/*--] Structure [--------------------------------] FormField [-------------*/

   _StructType(FormField)
   struct FormField
   {
      BytT        x;
      BytT        y;
      BytT        attr;
      BytT        length;
      ChrP        text;

                  FormField      (                                      );
                  ~FormField     (                                      );
   };

/*-------------------------------------------------------------------------*/

enum FormFields
{
   From          ,   To            ,   Subj          ,   Msg           ,
   Area          ,   PvtMsg        ,   Insert        ,   Raw           ,
   Abbrevs       ,   EscForMenu    ,   TextWindow    ,   MenuBar       ,
   EndOfFields
};

/*--] Classes [----------------------------------] Setup [-----------------*/

   _ClassType(Setup)
   class Setup
   {
   public:
      FormFieldI  fields;
      LanguageP   language;

      // --- variables from the configuration file -------------------------
      FlgT        stopBanner;
      FlgT        fastScroll;
      FlgT        insertMode;
      FlgT        fullQuote;
      FlgT        addTag;
      ChrP        bannerText;
      ChrP        importFiles[10];
      ChrP        ptrPort;
      FlgT        logging;

      // --- runtime flags -------------------------------------------------
      FlgT        rawMode;
      FlgT        abbrevMode;
      FlgT        isRegistered;
      FlgT        isSysop;

      // --- runtime variables ---------------------------------------------
      WrdT        maxText;
      WrdT        maxRows;
      ChrP        sysop;
      ChrP        bbs;
      ChrP        user;
      ChrP        userLang;
      LngT        bpsRate;

   protected:
      IntT        findDomain     (  ChrP k                              );
      IntT        findVar        (  ChrP k                              );
      FlgT        yesNo          (  ChrP yn, FlgT def                   );
      BytT        colorCode      (  ChrP attr                           );
      void        checkRegKey    (  ChrP regKey                         );
      void        readMsgInfo    (                                      );

   public:
                  Setup          (                                      );
                  ~Setup         (                                      );

      FlgT        read           (  ChrP path, ChrP proj                );

   };

/*-------------------------------------------------------------------------*/

   extern SetupP TSetup;

/*-------------------------------------------------------------------------*/

#endif

/*---------------------------] END OF THE CODE [---------------------------*/
