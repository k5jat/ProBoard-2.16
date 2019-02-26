
/* ------------------------------------------------------------
 * Filename ............... PbStruct.Hpp
 *
 * General Purpose ........ ProBoard structures
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

/*--] Structures [-------------------------------] keydata [---------------*/

extern ChrP    cryptext;
extern ShrtT   crypt_ptr;
extern ShrtT   crypt_length;

_StructType(keyData)
struct keyData
{
   Date   createDate;
   Time   createTime;
   ChrT   createdBy[36];
   ChrT   sysopName[36];
   ChrT   bbsName[36];
   FlgT   alpha;
   FlgT   beta;
   FlgT   gamma;
   FlgT   distSite;
   FlgT   regSite;
   FlgT   commercial;
   Date   expirDate;
   UShrtT expirVersion;
   ChrT   extra[2];
};

_StructType(keyFile)
struct keyFile
{
   keyDataT keys[20];
   BytT     xorData[101];
   DwdT     keyCrc;
   DwdT     securityCheck[32];
   DwdT     xorCrc;
};

/*---------------------------] END OF THE CODE [---------------------------*/
