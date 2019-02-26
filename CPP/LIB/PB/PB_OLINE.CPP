
/* ------------------------------------------------------------
 * Filename ............... Pb_OLine.Cpp
 *
 * General Purpose ........ OnLine configuration class
 * ------------------------------------------------------------
 * First date ............. 1 october 1993
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

#define Uses_PbOnLine
#include <ProBoard.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Routine   : PbOnLine()
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: ProBoard system path, filename
 * Return    : None
 *
 */

PbOnLine::PbOnLine(ChrP path, ChrP fn)
{
   setFname(path, fn);
}

/*
 * Routine   : readRec()
 * Purpose   : read a record from the current file
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT PbOnLine::readRec()
{
   return(File::read(this, recLen()) == recLen());
}

/*
 * Routine   : writeRec()
 * Purpose   : write a record to the current file
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT PbOnLine::writeRec()
{
   return(File::write(this, recLen()) == recLen());
}

/*
 * Routine   : recLen()
 * Purpose   : return the record length
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : record length
 *
 */

ShrtT PbOnLine::recLen()
{
   return(SIZ(online_data));
}

/*
 * Routine   : clear()
 * Purpose   : clear the current record
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void PbOnLine::clear()
{
   memset(this, 0, recLen());
}

/*
 * Routine   : inUse()
 * Purpose   : check if a record is in use
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if in use, FALSE if NOT
 *
 */

FlgT PbOnLine::inUse()
{
   return(TRUE);           // I should check on it...
}

/*
 * Routine   : copy()
 * Purpose   : Copy the data from another object (of the same type !)
 * ------------------------------------------------------------------------
 * Parameters: pointer to the other object
 * Return    : None
 *
 */

void PbOnLine::copy(PbOnLineP orig)
{
   memcpy(this, orig, recLen());
}

/*---------------------------] END OF THE CODE [---------------------------*/

