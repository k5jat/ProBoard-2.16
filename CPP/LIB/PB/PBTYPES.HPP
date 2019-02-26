
/* ------------------------------------------------------------
 * Filename ............... PbTypes.Hpp
 *
 * General Purpose ........ ProBoard types & common classes
 * ------------------------------------------------------------
 * First date ............. 14 sep 1993
 *
 * First in version ....... 1.40
 *
 * Written by ............. Alain Schellinck
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

typedef BytT PbTime[7][6];

/*--] Classes [----------------------------------] PbBoards [--------------*/

#ifndef Used_PbBoards
#  define Used_PbBoards

   _ClassType(PbBoards)
   class PbBoards
   {
   private:
      BytT bitmap[125];

   public:
               PbBoards       (                                         );

      void     set            (  UShrtT brd                             );
      void     reset          (  UShrtT brd                             );
      FlgT     get            (  UShrtT brd                             );
      FlgT     toggle         (  UShrtT brd                             );
   };

#endif

/*--] Classes [----------------------------------] PbFlags [---------------*/

#ifndef Used_PbFlags
#  define Used_PbFlags

   _ClassType(PbFlags)
   class PbFlags                          // ProBoard security flags
   {
   private:
      LngT f;

   public:
               PbFlags        (                                            );
               PbFlags        (  LngT flags                                );
               PbFlags        (  ChrP str                                  );

      void     clear          (                                            );

      FlgT     get            (  ShrtT flg                                 );
      FlgT     get            (  ChrT  flg                                 )
      {
         return(get(getNum(flg)));
      }

      void     set            (  LngT  flg                                 );
      void     set            (  ShrtT flg                                 );
      void     set            (  ChrP  flgs                                );
      void     set            (  ChrT  flg                                 )
      {
         set(getNum(flg));
      }


      void     reset          (  LngT  flg                                 );
      void     reset          (  ShrtT flg                                 );
      void     reset          (  ChrP  flgs                                );
      void     reset          (  ChrT  flg                                 )
      {
         reset(getNum(flg));
      }

      String   toStr          (                                            );
      LngT     toFlag         (  ChrP  str                                 );
      ShrtT    getNum         (  ChrT  str                                 );
      ChrT     getChar        (  ShrtT flg                                 );

               operator LngT  (                                            )
               {
                  return(f);
               }
   };


#endif

/*---------------------------] END OF THE CODE [---------------------------*/
