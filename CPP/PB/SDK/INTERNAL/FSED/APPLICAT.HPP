
/* ------------------------------------------------------------
 * Filename ............... Applicat.Hpp
 *
 * General Purpose ........ Application class
 * ------------------------------------------------------------
 * First date ............. 12-16-94 03:18pm
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

#ifndef __APPLICAT_HPP
#  define __APPLICAT_HPP

/*--] Classes [----------------------------------] Application [-----------*/

   _ClassType(Application)
   class Application
   {
   protected:
      ChrT        buffer[256];
      ChrT        pokeStr[256];

   public:
      // --- runtime flags ----------------------------------------------
      FlgT        redrawScreen;
      FlgT        redrawBanner;
      FlgT        deleteSource;

      // --- runtime variables ------------------------------------------
      ChrT        saveName [81];
      ChrT        loadName [81];
      ChrT        pexPath  [81];
      ChrT        startPath[81];

   protected:
      FlgT        askFileName    (                                      );
      void        handleQuote    (                                      );
      void        showEnding     (                                      );

   public:
                  Application    (  IntT argc, ChrI argv                );
                  ~Application   (                                      );

      void        run            (                                      );
   };

/*-------------------------------------------------------------------------*/

   extern ApplicationP TApp;

/*-------------------------------------------------------------------------*/

#endif                                 // __APPLICAT_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
