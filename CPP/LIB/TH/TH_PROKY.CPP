
/* ------------------------------------------------------------
 * Filename ............... Th_ProKy.Cpp
 *
 * General Purpose ........ base classes for the key generator
 * ------------------------------------------------------------
 * First date ............. 6 october 1993
 *
 * First in version ....... 2.02
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
#  include <stdlib.h>
#  include <string.h>
#endif

#define Uses_ProKeng
#include <TheHouse.Hpp>

/*--] Code [---------------------------------------------------------------*/

/*
 * Name       : ProKeng()
 * Purpose    : the constructor
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

ProKeng::ProKeng()
{
}

/*
 * Name       : setFname()
 * Purpose    : set the filename
 * -------------------------------------------------------------------------
 * Parameters : path and filename
 * Return     : None
 */

void ProKeng::setFname(ChrP path, ChrP fn)
{
   fname = "";
   fname << path << fn << ".Key";
   fname.mixedCase();
}

/*
 * Name       : calc()
 * Purpose    : calculate the keyvalue for one table
 * -------------------------------------------------------------------------
 * Parameters : pointer to the random generator
 * Return     : key value
 */

DwdT ProKeng::calc(BytP table1, BytP table2)
{
    BytT ltr, tab1, tab2;
    DwdT keyVal = 0;

    for(UShrtT cnt = 0; cnt < 32; cnt++)  // encode each letter
    {
        tab1 = table1[cnt];             // take a byte from the 1st table
        tab2 = table2[cnt];             // take a byte from the 2nd table

        // take a letter from the user's name, bbs name and copyright message
        ltr = (data.name[cnt] ^ data.system[cnt] ^ data.copyright[cnt]);

        ltr = (ltr ^ tab1);             // encode the letter
        keyVal = keyVal + DwdT((ltr * tab2 * (cnt ^ tab1) * (data.license ^ ltr)));
    }

    return(keyVal);                     // return the key value
}

/*
 * Name       : crc()
 * Purpose    : calculate the CRC code
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : CRC code
 */

DwdT ProKeng::crc()
{
   ChrP p = data.copyright;
   DwdT crc = 0;

   for(UShrtT cnt = 0; cnt < regCheckLen; cnt++)
      crc += DwdT(((*(p++) * (0xF0 >> (cnt % 8)))));

   return(crc);
}

/*
 * Name       : read()
 * Purpose    : read the keyfile
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT ProKeng::read()
{
    File in;
    FlgT ret_val = FALSE;

    if(in.open(fname, fmode_read) == TRUE)
        if(in.read(&data, sizeof(KeyRecT)) == sizeof(KeyRecT))
        {
            in.close();
            ret_val = TRUE;
        }

    return(ret_val);
}

/*
 * Name       : scramble()
 * Purpose    : scramble the keyfile
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void ProKeng::scramble()
{
   ChrP p = data.copyright;
   for(UShrtT cnt = 0; cnt < randomLen; cnt++)
      p[cnt] = p[cnt] ^ data.random[cnt];
}

/*
 * Name       : check()
 * Purpose    : check the keyfile
 * -------------------------------------------------------------------------
 * Parameters : name of the sysop, name of the system, license required,
 *              pointer to the two tables, table number
 * Return     : TRUE or one of the erXXX messages
 */

UShrtT ProKeng::check(ChrP name, ChrP system, UShrtT license, BytP table1, BytP table2, UShrtT tableNo)
{
   if(read() == FALSE)
      return(erNoKeyFileFound);        // check for a key file

   if(crc() != data.checksum)
      return(erWrongCrcCode);          // check CRC code

   scramble();                         // make key readable...

   if(calc(table1, table2) != data.keycodes[tableNo])
      return(erWrongCheckCode);        // check registration code

   if(stricmp(name, data.name) != 0)
      return(erWrongName);             // check sysop name

   if(stricmp(system, data.system) != 0)
      return(erWrongSystem);           // check BBS name

   if((data.license & license) != license)
      return(erWrongLicense);          // check license flags

   return(TRUE);                       // Right! a legal key!
}

/*---------------------------] END OF THE CODE [---------------------------*/











































