
/* ------------------------------------------------------------
 * Filename ............... UseStamp.Cpp
 *
 * General Purpose ........ UseStamp class
 * ------------------------------------------------------------
 * First date ............. 12-20-94 02:56pm
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

#include "usestamp.hpp"
#include "date.hpp"

/*-------------------------------------------------------------------------*/

UseStampP   TUseStamp;

/*-------------------------------------------------------------------------*/

LCL IntT fdate;
LCL IntT ftime;

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : getStamp()
 * Purpose   : get the file date and time stamp
 * ------------------------------------------------------------------------
 * Parameters: filename
 * Return    : True if OK, False if NOT
 *
 */

LCL FlgT getStamp(ChrP fname)
{
   FlgT ret_val = True;

   asm{
   // --- open -------------------------
      push  ds
      mov   ah, 3Dh
      mov   al, 0
      lds   dx, [fname]
      int   21h
      pop   ds
      jc    isFailed
   // --- get date & time w-------------
      mov   bx, ax
      mov   ah, 57h
      mov   al, 0
      int   21h
      jc    isFailed
      mov   [fdate], dx
      mov   [ftime], cx
   // --- close ------------------------
      mov   ah, 3Eh
      int   21h
      jnc   isOK
   }
isFailed:
   ret_val = False;

isOK:
   return(ret_val);
}

/*
 * Routine   : setStamp()
 * Purpose   : set the file date and time stamp
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

LCL FlgT setStamp(ChrP fname)
{
   FlgT ret_val = True;

   asm{
   // --- open -------------------------
      push  ds
      mov   ah, 3Dh
      mov   al, 0
      lds   dx, [fname]
      int   21h
      pop   ds
      jc    isFailed
   // --- set date & time w-------------
      mov   bx, ax
      mov   ah, 57h
      mov   al, 1
      mov   dx, [fdate]
      mov   cx, [ftime]
      int   21h
      jc    isFailed
   // --- close ------------------------
      mov   ah, 3Eh
      int   21h
      jnc   isOK
   }
isFailed:
   ret_val = False;

isOK:
   return(ret_val);
}

/*--] Code [-------------------------------------] UseStamp [--------------*/

/*
 * Routine   : UseStamp()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

UseStamp::UseStamp(ChrP path, ChrP project)
{

   sprintf(fname,"%s%s.%s",
               path,
               project,
#ifdef PEX
               "PEX"
#else
               "EXE"
#endif
          );

   days      = 0;
   firstTime = True;
   tampered  = False;

   if(read() == True)
   {
      Date today;

      if(value == 0)          // this is the first time
      {
         value = today.pack();
         write();
      }
      else                    // it is not the first time
      {
         Date firstUse;
         firstUse.unpack(value);
         if(firstUse > today || !firstUse.isValid())
         {
            tampered = True;  // but we have an invalid date!
            value = today.pack();
            write();
         }
         else
         {
            days = today - firstUse;
            firstTime = False;
         }
      }
   }
}

/*
 * Routine   : read()
 * Purpose   : read the timestamp from the specified executable
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if OK, False if NOT
 *
 */

FlgT UseStamp::read()
{
   FlgT ret_val = False;

   FILE *handle = fopen(fname,"rb");
   if(handle != NULL)
   {
      if(fseek(handle, 0x12, SEEK_SET) == 0)
         if(fread(&value, sizeof(WrdT), 1, handle) == 1)
            ret_val = True;

      fclose(handle);
   }

   return(ret_val);
}

/*
 * Routine   : write()
 * Purpose   : write the date / time stamp to the executable
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if OK, False if NOT
 *
 */

FlgT UseStamp::write()
{
   FlgT ret_val = False;

   FlgT changeStamp = getStamp(fname);

   FILE *handle = fopen(fname, "r+b");
   if(handle != NULL)
   {
      if(fseek(handle, 0x12, SEEK_SET) == 0)
         if(fwrite(&value, sizeof(WrdT), 1, handle) == 1)
            ret_val = True;

      fclose(handle);
   }

   if(changeStamp)
      setStamp(fname);

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
