
/* ------------------------------------------------------------
 * Filename ............... Th_RecOI.Cpp
 *
 * General Purpose ........ Multi Record IO class
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

#ifndef PEX
#endif

#define Uses_MultiRecIO
#include <TheHouse.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Routine   : MultiRecIO()
 * Purpose   : constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

MultiRecIO::MultiRecIO()
   : File()
{
}

/*
 * Routine   : ~MultiRecIO()
 * Purpose   : destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

MultiRecIO::~MultiRecIO()
{
}

/*
 * Routine   : setFname()
 * Purpose   : set the filename for the file
 * ------------------------------------------------------------------------
 * Parameters: path, filename
 * Return    : None
 *
 */

void MultiRecIO::setFname(ChrP path, ChrP name)
{
   fname = path;
   fname.appendBS();
   fname << name;
   fname.mixedCase();
}

/*
 * Routine   : getFname()
 * Purpose   : return the complete filename
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : complete filename
 *
 */

FileName MultiRecIO::getFname()
{
   return(fname);
}

/*
 * Routine   : open()
 * Purpose   : open the file for reading and/or writing
 * ------------------------------------------------------------------------
 * Parameters: fmode_xxx, buffer size
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT MultiRecIO::open(IntT fmode, IntT bufSize)
{
   if(opened())
      close();

   idle(idleOpen);
   return(File::open(fname, fmode, bufSize));
}


/*
 * Routine   : close()
 * Purpose   : close the file
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void MultiRecIO::close()
{
   idle(idleClose);
   File::close();
}


/*
 * Routine   : read()
 * Purpose   : read a specified record
 * ------------------------------------------------------------------------
 * Parameters: record number (-1 = current record)
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT MultiRecIO::read(ShrtT recno)
{
   FlgT ret_val = TRUE;

   idle(idleRead);

   if(recno != -1)
      ret_val = pos(recno);

   if(ret_val == TRUE)
      ret_val = readRec();

   return(ret_val);
}

/*
 * Routine   : write()
 * Purpose   : write a specified record
 * ------------------------------------------------------------------------
 * Parameters: record number (-1 = current record)
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT MultiRecIO::write(ShrtT recno)
{
   FlgT ret_val = TRUE;

   idle(idleWrite);

   if(recno != -1)
      ret_val = pos(recno);

   if(ret_val == TRUE)
      ret_val = writeRec();

   return(ret_val);
}

/*
 * Routine   : pos()
 * Purpose   : seek to the specified record
 * ------------------------------------------------------------------------
 * Parameters: required record number
 * Return    : TRUE if OK, FALSE if NOT
 *
 */

FlgT MultiRecIO::pos(ShrtT  recno)
{
   FlgT ret_val = FALSE;

   idle(idleSeek);

   LngT fpos = (LngT(recno) * LngT(recLen()));
   if(seek(fpos - File::pos(), seek_cur) == fpos)
      ret_val = TRUE;

   return(ret_val);
}

/*
 * Routine   : count()
 * Purpose   : count the number of records
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : number of records
 *
 */

ShrtT MultiRecIO::count()
{
   ShrtT ret_val = ShrtT(len() / LngT(recLen()));
   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
