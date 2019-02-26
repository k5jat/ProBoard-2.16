
/* ------------------------------------------------------------
 * Filename ............... TheHouse.Hpp
 *
 * General Purpose ........ Main Include file
 * ------------------------------------------------------------
 * First date ............. 14 sep 1993
 *
 * First in version ....... 2.00
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

/*-------------------------------------------------------------------------*/

#ifndef Used_TheHouse_Lib
#  define Used_TheHouse_Lib

/*-------------------------------------------------------------------------*/

#  ifdef PEX
#    include <Pb_Sdk.h>
#  endif

/*-------------------------------------------------------------------------*/

   const int defaultBufferSize = 4096;

/*-------------------------------------------------------------------------*/

#  if(    (defined Uses_IniFile  ) \
       || (defined Uses_IniFile  ) )
#     define Uses_LinkedList
#endif

/*-------------------------------------------------------------------------*/

#  include <TsLib.Hpp>
#  include <ThTypes.Hpp>
#  include <ThClass.Hpp>

/*-------------------------------------------------------------------------*/

   FlgT     isUpToDate        (  ChrP src, ChrP dst                     );
   FlgT     copyFile          (  File& src, File& dst                   );
   FlgT     copyBlock         (  File& src, File& dst, LngT length      );

/*-------------------------------------------------------------------------*/

   FlgT     addToEnv          (  ChrP env, FileName &path               );

/*-------------------------------------------------------------------------*/

   FileName getUniqueName     (  FileName dir, String ext               );
   FileName getEnvironPath    (  ChrP argv, ChrP envVar                 );

/*-------------------------------------------------------------------------*/

   String   makeTitle         (  ChrP progName, ChrP version,
                                 ChrP description, ChrP author,
                                 ChrP copyright, ChrP release           );
   void     spinWheel         (                                         );

/*-------------------------------------------------------------------------*/

   ChrP     CToPas            (  ChrP str                               );
   ChrP     PasToC            (  ChrP str                               );

/*-------------------------------------------------------------------------*/

   void     decompress_data   (  BytP input, BytP output, ShrtT insize,
                                 ShrtT outsize                          );

/*-------------------------------------------------------------------------*/

#endif

/*---------------------------] END OF THE CODE [---------------------------*/
