
/* ------------------------------------------------------------
 * Filename ............... Rows.Cpp
 *
 * General Purpose ........ Rows class
 * ------------------------------------------------------------
 * First date ............. 12-06-94 03:10pm
 *
 * First in version ....... 2.00
 *
 * Written by ............. Alain Schellinck
 *
 * Copyright .............. Nephilim software development
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

#include "main.hpp"
#pragma  hdrstop

/*-------------------------------------------------------------------------*/

#include "rows.hpp"
#include "setup.hpp"

/*--] Code [-------------------------------------] Rows [------------------*/

/*
 * Routine   : Rows()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Rows::Rows()
{
   lines = WrdP(malloc(WrdT(TSetup->maxRows) * sizeof(WrdT)));
   clear();
}

/*
 * Routine   : ~Rows()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Rows::~Rows()
{
   if(lines != NULL)
      free(lines);
}

/*
 * Routine   : clear()
 * Purpose   : clear the rows buffer
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */

void Rows::clear()
{
   used  = 0;
   memset(lines, 0, WrdT(TSetup->maxRows) * sizeof(WrdT));
}

/*
 * Routine   : exist()
 * Purpose   : check if the specified line does exist
 * ------------------------------------------------------------------------
 * Parameters: line number
 * Return    : True if OK, False if NOT
 *
 */

FlgT Rows::exist(IntT lineNo)
{
   return(lineNo < used ? True : False);
}

/*
 * Routine   : set()
 * Purpose   : set the offset for the specified line
 * ------------------------------------------------------------------------
 * Parameters: line number, offset
 * Return    : True if OK, False if NOT
 *
 */

FlgT Rows::set(IntT lineNo, WrdT offset)
{
   FlgT ret_val = False;

   if(lineNo <= TSetup->maxRows)
   {
      lines[lineNo] = offset;

      if(!exist(lineNo))
         used = lineNo + 1;

      ret_val = True;
   }

   return(ret_val);
}


/*
 * Routine   : get()
 * Purpose   : get the offset for the specified line
 * ------------------------------------------------------------------------
 * Parameters: Line number
 * Return    : offset
 *
 */

WrdT Rows::get(IntT lineNo)
{
   WrdT ret_val = 0;
   if(exist(lineNo))
      ret_val = lines[lineNo];

   return(ret_val);
}

/*
 * Routine   : last()
 * Purpose   : return the number of lines in use
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : number of lines in use
 *
 */

IntT Rows::last()
{
   return(used);
}


/*
 * Routine   : llen()
 * Purpose   : return the length of the specified line
 * ------------------------------------------------------------------------
 * Parameters: line number, maximum text length
 * Return    : length of the line
 *
 */

WrdT Rows::llen(IntT lineNo, WrdT textLen)
{
   WrdT ret_val = 0;

   if(exist(lineNo))
   {
      WrdT start = get(lineNo    );
      WrdT end   = get(lineNo + 1) - 1;
      if(!exist(lineNo + 1))
         end = textLen;

      ret_val = (end - start) + 1;
   }

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
