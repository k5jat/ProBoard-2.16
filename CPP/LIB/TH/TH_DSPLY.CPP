
/* ------------------------------------------------------------
 * Filename ............... Th_Dsply.Cpp
 *
 * General Purpose ........ Some misc. display functions
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
#  include <stdlib.h>
#  include <stdio.h>
#  include <dos.h>
#  include <string.h>
#endif

#include <TheHouse.Hpp>

/*-------------------------------------------------------------------------*/

/*
 * Name       : makeTitle()
 * Purpose    : make a title block
 * -------------------------------------------------------------------------
 * Parameters : Progname, version, description, copyright year,
 *              author, release
 * Return     : String class with the title block
 */

String makeTitle( ChrP progName, ChrP version, ChrP description,
                  ChrP copyright, ChrP author, ChrP release             )
{
   String line1("");
   String line2("");
   String title("");

   line1 << progName << " version " << version << ". " << description << ".";
   line1.justify(String::centered, 79);

   line2 << "(C) " << copyright << " " << author << ". All rights reserved!";

   if(strlen(release) != 0)
      line2 << " " << release << " release!";

   line2.justify(String::centered, 79);

   title << line1 << "\n" << line2 << "\n" << String('Ä', 79);

   return(title);
}

/*
 * Name       : spinWheel()
 * Purpose    : Display a spinning cursor on the screen
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void spinWheel()
{
   static DwdT ticks = clockticks();
   static ShrtT pos = 0;
   ChrT cursor[8] = "|/-\\";

   if(clockdiff(ticks) > 1)
   {
      ticks = clockticks();
      pos++;
      pos = pos % 4;
#ifdef PEX
      printf("\b%c\0xFF", cursor[pos]);
#else
      printf("%c\b", cursor[pos]);
#endif
   }
}

/*---------------------------] END OF THE CODE [---------------------------*/




















