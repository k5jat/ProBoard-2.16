
/* ------------------------------------------------------------
 * Filename ............... VideoRam.Hpp
 *
 * General Purpose ........ Video ram functions
 * ------------------------------------------------------------
 * First date ............. 12-05-94 02:26pm
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

#ifndef __VIDEORAM_HPP
#  define __VIDEORAM_HPP

/*-------------------------------------------------------------------------*/

   extern "C"
   {
      void           vrInit      (                                      );
   }

   extern "C"
   {
      extern   BytT  vrVideoCard;
      extern   WrdT  vrHorSize;
      extern   WrdT  vrVerSize;
      extern   ChrFP vrVideoAddr;
      extern   BytT  vrScanTop;
      extern   BytT  vrScanBottom;
   };

/*-------------------------------------------------------------------------*/

#endif                                 // __VIDEORAM_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
