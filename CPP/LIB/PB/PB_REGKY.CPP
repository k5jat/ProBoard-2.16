
/* ------------------------------------------------------------
 * Filename ............... Pb_RegKy.Cpp
 *
 * General Purpose ........ ProBoard registration check
 * ------------------------------------------------------------
 * First date ............. 15 october 1993
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

#define Uses_PbRegkey
#include <ProBoard.Hpp>
#include <ProBoard.Reg>

/*-------------------------------------------------------------------------*/

ChrP    cryptext;
ShrtT   crypt_ptr;
ShrtT   crypt_length;

// ------------------------------------------------------------------------

static byte fixed_xor_data[100]=
{
    0x79,0xDE,0xAA,0x71,0x3A,0x68,0x5D,0xBA,0xE7,0x2E,
    0x69,0x90,0x4D,0x7C,0x6D,0x49,0xB7,0x66,0x51,0x57,
    0xCB,0xF4,0xC5,0xC3,0x2D,0x99,0xE7,0x5E,0xE6,0xFA,
    0x8D,0x26,0xC5,0x52,0x16,0x56,0x2F,0xCB,0x8A,0x7F,
    0x59,0xB6,0x6C,0xFE,0x26,0x94,0x3D,0x48,0x35,0xEF,
    0x6D,0x4A,0xF2,0x2D,0x47,0x2A,0x12,0x0E,0xDC,0x0A,
    0x4F,0x99,0x1D,0x76,0x5B,0xD7,0x44,0x9F,0x4E,0x6D,
    0xBD,0x83,0x44,0xC1,0xAE,0xD7,0xA6,0x20,0x26,0xDC,
    0x49,0xAB,0x6F,0xF2,0x1E,0x3F,0x23,0x5E,0x7D,0x67,
    0xE7,0x43,0xA4,0x48,0x8B,0xDF,0x73,0xD7,0x20,0x29
};

// ------------------------------------------------------------------------

DwdT  calcSecurity   (  keyDataP kd, BytP table1, BytP table2           );
void  crypt          (  ChrP buf                                        );
void  encrypt        (  ChrP text, ShrtT len, ChrP key                  );

/*--] Code [---------------------------------------------------------------*/

/*
 * Name       : checkPbKey()
 * Purpose    : Check the ProBoard key
 * -------------------------------------------------------------------------
 * Parameters : path to the keyfile, table number, pointer to the two tables,
 *              keystruct to fill
 * Return     : TRUE if a legal key, FALSE if NOT
 */

   /* WARNING -------------------------------------------------------------

      Only the CRC and TABLE is checked. The calling program should check
      the sysop name, BBS name and license

      ---------------------------------------------------------------------*/

FlgT checkPbKey(ChrP path, ShrtT tabNo, BytP tb1, BytP tb2, keyDataR copy)
{
   FlgT ret_val = TRUE;

   keyFileP kf = new keyFileT;
   File     in;

   FileName fn(path);
   fn.appendBS();
   fn << "RegKey.Pb";

   if(in.open(fn, fmode_read) == TRUE)
   {
      ShrtT size = in.len() - 33;
      BytP  data = new BytT[size];

      if(in.seek(33) != 33)
         ret_val = FALSE;

      if(in.read(data, size) != size)
         ret_val = FALSE;

      in.close();

      for(ShrtT cnt = 0; cnt < size; cnt++)
         data[cnt] ^= fixed_xor_data[cnt % 100];

      decompress_data(data, BytP(kf), size, sizeof(*kf));
      encrypt(ChrP(kf->keys), sizeof(kf->keys), kf->xorData);

      if(    (kf->keyCrc != crc32(kf->keys, sizeof(kf->keys)))
          || (kf->xorCrc != crc32(kf->xorData, sizeof(kf->xorData)))
        )
      {
         ret_val = FALSE;
      }

      if(kf->securityCheck[tabNo] != calcSecurity(&kf->keys[0], tb1, tb2))
         ret_val = FALSE;
   }
   else
      ret_val = FALSE;

   if(ret_val == TRUE)
   {
      memcpy(&copy, &kf->keys[0], sizeof(keyDataT));
   }

   return(ret_val);
}

/*--] Code [-------------------------------------] code from Philippe [----*/

/*
 * Name       : calcSecurity()
 * Purpose    : calculate a security code
 * -------------------------------------------------------------------------
 * Parameters : key structure, pointer to the two tables
 * Return     : security code
 */

DwdT calcSecurity(keyDataP kd, BytP table1, BytP table2)
{
   BytT ltr, tab1, tab2;
   DwdT key = 0;

   for(UShrtT cnt = 0; cnt < 32; cnt++)  // encode each letter
   {
      tab1 = table1[cnt];                // take a byte from the 1st table
      tab2 = table2[cnt];                // take a byte from the 2nd table

      ltr = kd->sysopName[cnt] ^ kd->bbsName[cnt] ^ kd->createdBy[cnt] ^ tab1;
      key = key + DwdT(ltr * tab2 * (cnt ^ tab1) * (kd->beta ^ ltr));
   }

   return key;                            // return the key value
}


/*
 * Name       :
 * Purpose    :
 * -------------------------------------------------------------------------
 * Parameters :
 * Return     :
 */

void crypt(ChrP buf)
{
   *buf ^= cryptext[crypt_ptr] ^ (cryptext[0] * crypt_ptr);

   cryptext[crypt_ptr] += ((crypt_ptr < (crypt_length - 1))
                           ? cryptext[crypt_ptr + 1]
                           : cryptext[0]);

   if(!cryptext[crypt_ptr])
      cryptext[crypt_ptr] += 1;

   if(++crypt_ptr >= crypt_length)
      crypt_ptr = 0;
}

/*
 * Name       :
 * Purpose    :
 * -------------------------------------------------------------------------
 * Parameters :
 * Return     :
 */

void encrypt(ChrP text, ShrtT len, ChrP key)
{
   char x[101];
   strcpy(x,key);

   crypt_length = strlen(key);
   crypt_ptr = 0;
   cryptext = x;

   for(int i=0;i<len;i++)
      crypt(text++);
}

/*---------------------------] END OF THE CODE [---------------------------*/




















