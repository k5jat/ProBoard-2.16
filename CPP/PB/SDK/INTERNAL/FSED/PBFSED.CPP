
/* ------------------------------------------------------------
 * Filename ............... TheEdit.Cpp
 *
 * General Purpose ........ Main source file
 * ------------------------------------------------------------
 * First date ............. 12-09-94 01:35pm
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

#include "applicat.hpp"
#include "usestamp.hpp"

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : main()
 * Purpose   : the main routine
 * ------------------------------------------------------------------------
 * Parameters: command line parameters
 * Return    : None
 *
 */

void main(IntT argc, ChrP argv[])
{
   printf("\n\nLoading %s version %s...", projectName, versionText);

   TApp = new Application(argc, argv);
   TApp->run();
   delete TApp;

   return;
}

/*---------------------------] END OF THE CODE [---------------------------*/
