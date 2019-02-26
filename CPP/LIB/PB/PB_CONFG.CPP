
/* ------------------------------------------------------------
 * Filename ............... Pb_Confg.Cpp
 *
 * General Purpose ........ ProBoard configuration class
 * ------------------------------------------------------------
 * First date ............. 19 sep 1993
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

#define Uses_PbConfig
#include <ProBoard.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Routine   : PbConfig()
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: ProBoard system path, filename
 * Return    : None
 *
 */

PbConfig::PbConfig(ChrP path, ChrP fn)
{
   setFname(path, fn);
}

/*
 * Routine   : read()
 * Purpose   : read the configuration
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT PbConfig::read()
{
   FlgT ret_val = FALSE;

   if(open(fmode_read, defaultBufferSize) == TRUE)
      if(MultiRecIO::read(0) == TRUE)
      {
         close();
         ret_val = TRUE;
      }

   return(ret_val);
}

/*
 * Routine   : write()
 * Purpose   : write the configuration
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT PbConfig::write()
{
   FlgT ret_val = FALSE;

   if(open(fmode_create, defaultBufferSize) == TRUE)
      if(MultiRecIO::write(0) == TRUE)
      {
         close();
         ret_val = TRUE;
      }

   return(ret_val);
}

/*
 * Routine   : readRec()
 * Purpose   : read a record from the current file
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT PbConfig::readRec()
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

FlgT PbConfig::writeRec()
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

ShrtT PbConfig::recLen()
{
   return(SIZ(config_data));
}

/*
 * Routine   : clear()
 * Purpose   : clear the current record
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void PbConfig::clear()
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

void PbConfig::copy(PbConfigP orig)
{
   memcpy(this, orig, recLen());
}

/*---------------------------] END OF THE CODE [---------------------------*/
