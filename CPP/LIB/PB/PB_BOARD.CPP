
/* ------------------------------------------------------------
 * Filename ............... Pb_Board.Cpp
 *
 * General Purpose ........ ProBoard combined boards handling
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
#endif

#define Uses_PbBoards
#include <ProBoard.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Routine   : PbBoards()
 * Purpose   : the constructord
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

PbBoards::PbBoards()
{
   for(UShrtT cnt = 0; cnt < 125; cnt++)
      bitmap[cnt] = 0xFF;
}

/*
 * Routine   : set()
 * Purpose   : set the specified board to SELECTED
 * ------------------------------------------------------------------------
 * Parameters: board number
 * Return    : None
 *
 */

void PbBoards::set(UShrtT brd)
{
   if( (brd < 1) || (brd > 1000) )
      return;

   brd--;
   bitmap[brd / 8] |= (1 << (brd % 8));
}

/*
 * Routine   : reset()
 * Purpose   : set the specified board to NOT SELECTED
 * ------------------------------------------------------------------------
 * Parameters: board number
 * Return    : None
 *
 */

void PbBoards::reset(UShrtT brd)
{
   if( (brd < 1) || (brd > 1000) )
      return;

   brd--;
   bitmap[brd / 8] &= ~(1 << (brd % 8));
}

/*
 * Routine   : get()
 * Purpose   : get the status of the specified board
 * ------------------------------------------------------------------------
 * Parameters: board number
 * Return    : TRUE if SELECTED, FALSE if NOT
 *
 */

FlgT PbBoards::get(UShrtT brd)
{
   FlgT ret_val = FALSE;

   if( (brd < 1) || (brd > 1000) )
      return(ret_val);

   brd--;
   ret_val = (bitmap[brd / 8] & (1 << (brd % 8))) ? TRUE : FALSE;

   return(ret_val);
}

/*
 * Routine   : toggle()
 * Purpose   : toggle the setting of the specified board
 * ------------------------------------------------------------------------
 * Parameters: board number
 * Return    : new status
 *
 */

FlgT PbBoards::toggle(UShrtT brd)
{
   if(get(brd) == TRUE)
      reset(brd);
   else
      set(brd);

   return(get(brd));
}

/*---------------------------] END OF THE CODE [---------------------------*/

