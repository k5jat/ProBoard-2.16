
/* ------------------------------------------------------------
 * Filename ............... Th_Env.Cpp
 *
 * General Purpose ........ Some misc. environment functions
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
 * Name       : addToEnv()
 * Purpose    : add a path to the specified environment variable
 * -------------------------------------------------------------------------
 * Parameters : environment variable, path to add
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT addToEnv(ChrP env, FileName &path)
{
   FlgT ret_val = FALSE;
   String envVar(env);
   envVar.upperCase();

   ChrP envSet = getenv(envVar);
   if(envSet != NULL)
   {
       static ChrT buffer[1024];
       path.appendBS();
       sprintf(buffer,"%s=%s;%s", ChrP(envVar), ChrP(path), envSet);
       if(putenv(buffer) != -1)
           ret_val = TRUE;
   }

   return(ret_val);
}

/*
 * Routine   : getEnvironPath()
 * Purpose   : get a path from a environment variable or ARGV[0]
 * ------------------------------------------------------------------------
 * Parameters: ARGV[0] parameter, environment variable
 * Return    : Filename class with path
 *
 */

FileName getEnvironPath(ChrP argv, ChrP envVar)
{
   FileName environPath(argv);
   ChrP p;

   if( (p = getenv(envVar)) == NULL)
      environPath.stripName();

   if(p != NULL)
      environPath = p;

   environPath.appendBS();
   environPath.mixedCase();
   return(environPath);
}

/*---------------------------] END OF THE CODE [---------------------------*/
