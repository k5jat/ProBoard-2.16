
/* ------------------------------------------------------------
 * Filename ............... Pb_Event.Cpp
 *
 * General Purpose ........ Event configuration class
 * ------------------------------------------------------------
 * First date ............. 20 nov 1993
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

#ifndef PEX
#  include <string.h>
#endif

#define Uses_PbEvent
#include <ProBoard.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Routine   : PbEvent()
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: ProBoard system path, filename
 * Return    : None
 *
 */

PbEvent::PbEvent(ChrP path, ChrP fn)
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

FlgT PbEvent::readRec()
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

FlgT PbEvent::writeRec()
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

ShrtT PbEvent::recLen()
{
   return(SIZ(event_data));
}

/*
 * Routine   : clear()
 * Purpose   : clear the current record
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void PbEvent::clear()
{
   memset(this, 0, recLen());
}

/*
 * Routine   : copy()
 * Purpose   : Copy the data from another object (of the same type !)
 * ------------------------------------------------------------------------
 * Parameters: pointer to the other object
 * Return    : None
 *
 */

void PbEvent::copy(PbEventP orig)
{
   memcpy(this, orig, recLen());
}

/*---------------------------] END OF THE CODE [---------------------------*/
