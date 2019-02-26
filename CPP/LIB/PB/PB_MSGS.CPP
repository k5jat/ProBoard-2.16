
/* ------------------------------------------------------------
 * Filename ............... Pb_Msgs.Cpp
 *
 * General Purpose ........ Message configuration class
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

#define Uses_PbMessage
#include <ProBoard.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Routine   : PbMessage()
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: ProBoard system path, filename
 * Return    : None
 *
 */

PbMessage::PbMessage(ChrP path, ChrP fn)
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

FlgT PbMessage::readRec()
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

FlgT PbMessage::writeRec()
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

ShrtT PbMessage::recLen()
{
   return(SIZ(message_data));
}

/*
 * Routine   : clear()
 * Purpose   : clear the current record
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void PbMessage::clear()
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

FlgT PbMessage::inUse()
{
   return((strlen(name) > 0) ? TRUE : FALSE);
}

/*
 * Routine   : copy()
 * Purpose   : Copy the data from another object (of the same type !)
 * ------------------------------------------------------------------------
 * Parameters: pointer to the other object
 * Return    : None
 *
 */

void PbMessage::copy(PbMessageP orig)
{
   memcpy(this, orig, recLen());
}

/*---------------------------] END OF THE CODE [---------------------------*/
