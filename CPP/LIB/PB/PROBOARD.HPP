
/* ------------------------------------------------------------
 * Filename ............... ProBoard.Hpp
 *
 * General Purpose ........ ProBoard library classes
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

/*-------------------------------------------------------------------------*/

#ifndef PEX
#  include <string.h>
#endif

/*-------------------------------------------------------------------------*/

#ifndef Used_PbLibrary
#  define Used_PbLibrary

/*-------------------------------------------------------------------------*/

#  if(   (defined Uses_PbBinLog  )    \
      || (defined Uses_PbConfig  )    \
      || (defined Uses_PbEvent   )    \
      || (defined Uses_PbFile    )    \
      || (defined Uses_PbFileIdx )    \
      || (defined Uses_PbLimits  )    \
      || (defined Uses_PbMessage )    \
      || (defined Uses_PbModem   )    \
      || (defined Uses_PbOnLine  )    \
      || (defined Uses_PbProtocol)    \
      || (defined Uses_PbUsersBbs)    \
      || (defined Uses_PbUsersIdx)    \
      || (defined Uses_PbUsersPb )    )
#    define Uses_MultiRecIO
#  endif

/*-------------------------------------------------------------------------*/

#  include <TheHouse.Hpp>

#  include <PbFnames.Hpp>
#  include <PbTypes.Hpp>
#  include <PbStruct.Hpp>
#  include <PbClass.Hpp>

/*-------------------------------------------------------------------------*/

   FlgT        checkPbKey        (  ChrP path, ShrtT tabNo, BytP tb1,
                                    BytP tb2, keyDataR copy             );

   FileName    getSystemPath     (  ChrP argv                           );
   DwdT        raCRC             (  ChrP str                            );

/*-------------------------------------------------------------------------*/

#endif

/*---------------------------] END OF THE CODE [---------------------------*/
