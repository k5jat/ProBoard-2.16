
/* ------------------------------------------------------------
 * Filename ............... Pb_Flags.Cpp
 *
 * General Purpose ........ ProBoard flags handling
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

#include <ProBoard.Hpp>

/*-------------------------------------------------------------------------*/

const WrdT flagLen = 32;

LCL ChrP flagStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456";

/*-------------------------------------------------------------------------*/

/*
 * Routine   : PbFlags()
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

PbFlags::PbFlags()
{
   clear();
}

/*
 * Routine   : PbFlags()
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: flag value
 * Return    : None
 *
 */

PbFlags::PbFlags(LngT flg)
{
   f = flg;
}

/*
 * Routine   : PbFlags()
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: String to convert
 * Return    : None
 *
 */

PbFlags::PbFlags(ChrP str)
{
   toFlag(str);
}

/*
 * Routine   : clear()
 * Purpose   : clear all flag settings
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void PbFlags::clear()
{
   f = 0L;
}

/*
 * Routine   : get()
 * Purpose   : get the setting of the specified flag
 * ------------------------------------------------------------------------
 * Parameters: flag number
 * Return    : TRUE if SET, FALSE if NOT
 *
 */

FlgT PbFlags::get(ShrtT flg)
{
   FlgT ret_val = FALSE;

   if( (flg > 0) && (flg <= flagLen) )
      ret_val = ((f & (1L << (flagLen - flg))) ? TRUE : FALSE);

   return(ret_val);
}

/*
 * Routine   : set()
 * Purpose   : set a number of flags
 * ------------------------------------------------------------------------
 * Parameters: flags bitmap
 * Return    : None
 *
 */

void PbFlags::set(LngT flgs)
{
   f |= flgs;
}

/*
 * Routine   : set()
 * Purpose   : set the specified flag
 * ------------------------------------------------------------------------
 * Parameters: flag number
 * Return    : None
 *
 */

void PbFlags::set(ShrtT flg)
{
   if( (flg > 0) && (flg <= flagLen) )
      f |= (1L << (flagLen - flg));
}

/*
 * Routine   : set()
 * Purpose   : set the specified flags
 * ------------------------------------------------------------------------
 * Parameters: string with flags to set
 * Return    : None
 *
 */

void PbFlags::set(ChrP flgs)
{
   PbFlags setF(flgs);
   set(setF);
}

/*
 * Routine   : reset()
 * Purpose   : reset a number of flags
 * ------------------------------------------------------------------------
 * Parameters: flags bitmap
 * Return    : None
 *
 */

void PbFlags::reset(LngT flgs)
{
   f &= ~(flgs);
}

/*
 * Routine   : reset()
 * Purpose   : reset the specified flag
 * ------------------------------------------------------------------------
 * Parameters: flag number
 * Return    : None
 *
 */

void PbFlags::reset(ShrtT flg)
{
   if( (flg > 0) && (flg <= flagLen) )
      f &= ~(1L << (flagLen - flg));
}

/*
 * Routine   : reset()
 * Purpose   : reset the specified flags
 * ------------------------------------------------------------------------
 * Parameters: string with flags to set
 * Return    : None
 *
 */

void PbFlags::reset(ChrP flgs)
{
   PbFlags resetF(flgs);
   reset(resetF);
}

/*
 * Routine   : toStr()
 * Purpose   : convert the current flag value to a string
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : pointer to a string
 *
 */

String PbFlags::toStr()
{
   String str("");

   for(ShrtT cnt = 1; cnt <= flagLen; cnt++)
      str << (get(cnt) ? getChar(cnt) : '-');

   return(str);
}

/*
 * Routine   : toFlag()
 * Purpose   : convert the specified string to flag settings
 * ------------------------------------------------------------------------
 * Parameters: String to convert
 * Return    : current flag settings
 *
 */

LngT PbFlags::toFlag(ChrP str)
{
   ShrtT maxCnt = strlen(str);

   clear();
   for(ShrtT cnt = 0; cnt < maxCnt; cnt++)
      set(getNum(str[cnt]));

   return(f);
}

/*
 * Routine   : getNum()
 * Purpose   : get the number for a specified flag character
 * ------------------------------------------------------------------------
 * Parameters: the flag character
 * Return    : the value (or 0 if illegal character)
 *
 */

ShrtT PbFlags::getNum(ChrT flg)
{
   String f(flagStr);
   ShrtT ret_val = f.find(String(flg)) + 1;
   return(ret_val);
}

/*
 * Routine   : getChar()
 * Purpose   : get the character for the specified flag number
 * ------------------------------------------------------------------------
 * Parameters: flag number
 * Return    : flag character
 *
 */

ChrT PbFlags::getChar(ShrtT flg)
{
   return(flagStr[flg - 1]);
}

/*---------------------------] END OF THE CODE [---------------------------*/
