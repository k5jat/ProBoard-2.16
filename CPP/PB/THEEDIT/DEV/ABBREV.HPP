
/* ------------------------------------------------------------
 * Filename ............... Abbrev.Hpp
 *
 * General Purpose ........ Abbrev class
 * ------------------------------------------------------------
 * First in version ....... 1.00
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

#ifndef __ABBREV_HPP
#  define __ABBREV_HPP

/*-----------------------------------------------] AbbrevEntry [-----------*/

   class AbbrevEntry
   {
   public:
      ChrP        abbrev;
      ChrP        replace;

   public:
      AbbrevEntry * next;

   public:
                  AbbrevEntry    (                                      );
                  ~AbbrevEntry   (                                      );

      void        set            (  ChrP a, ChrP r                      );
      ChrP        check          (  ChrP a                              );
   };

/*-----------------------------------------------] AbbrevList [------------*/

   class AbbrevList
   {
   private:
      AbbrevEntry * first;
      AbbrevEntry * last;

   public:
                  AbbrevList     (                                      );
                  ~AbbrevList    (                                      );

      void        read           (  ChrP path, ChrP lang                );
      ChrP        get            (  ChrP abbrev                         );
      FlgT        list           (                                      );
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __ABBREV_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
