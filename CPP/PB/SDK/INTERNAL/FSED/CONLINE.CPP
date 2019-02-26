
/* ------------------------------------------------------------
 * Filename ............... ConLine.Cpp
 *
 * General Purpose ........ ConLine class
 * ------------------------------------------------------------
 * First date ............. 12-06-94 10:31am
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

#include "conline.hpp"

/*--] Code [-------------------------------------] ConLine [---------------*/

/*
 * Routine   : ConLine()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: width of the screen line, default attribute
 * Return    : None
 *
 */

ConLine::ConLine(BytT w, BytT attr)
{
   width = w;

   origAttr = attr;
   origText = new ChrT[width + 1];
   clear();

   copyAttr = attr;
   copyText = new ChrT[width + 1];
   clearCopy();
}

/*
 * Routine   : ~ConLine()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

ConLine::~ConLine()
{
   delete origText;
   delete copyText;
}

/*
 * Routine   : clear()
 * Purpose   : clear the original text
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void ConLine::clear()
{
   memset(origText, ' ', width);
   origText[width] = '\0';
   empty = True;
}

/*
 * Routine   : clearCopy()
 * Purpose   : clear the copy of the original text
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void ConLine::clearCopy()
{
   memset(copyText, ' ', width);
   copyText[width] = '\0';
}

/*
 * Routine   : set()
 * Purpose   : set the content of the original text
 * ------------------------------------------------------------------------
 * Parameters: text to copy
 * Return    : None
 *
 */

void ConLine::set(ChrP t)
{
   BytP p = BytP(t);

   for(IntT l = 0; *p && l < width; p++)
      if(*p >= 32)
         origText[l++] = *p;

   empty = (l == 0 ? True : False);

   memset(origText + l, ' ', width - l);
}

/*
 * Routine   : set()
 * Purpose   : set the attribute for this text line
 * ------------------------------------------------------------------------
 * Parameters: attribute
 * Return    : None
 *
 */

void ConLine::set(BytT a)
{
   origAttr = a;
}

/*
 * Routine   : get()
 * Purpose   : return a pointer to the copy text. Update the copy first
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : pointer to the copy of the text
 *
 */

ChrP ConLine::get()
{
   memcpy(copyText, origText, width);
   return(copyText);
}

/*
 * Routine   : get()
 * Purpose   : fill a buffer with the changed part
 * ------------------------------------------------------------------------
 * Parameters: buffer to fill
 * Return    : start position of the change. -1 if not changed
 *
 */

IntT ConLine::get(ChrP buffer)
{
   IntT ret_val = -1;
   IntT start;
   IntT end;

   if(origAttr != copyAttr)
   {
      ret_val = -2;
      start = 0;
      end = width;
   }
   else
   {
      for(start = 0; start < width; start++)
         if(origText[start] != copyText[start])
         {
            ret_val = start;
            break;
         }

      for(end = width; end > 0; end--)
         if(origText[end] != copyText[end])
            break;
   }

   if(ret_val != -1)
   {
      memcpy(copyText, origText, width);
      copyAttr = origAttr;

      memcpy(buffer, origText + start, end - start + 1);
      buffer[end - start + 1] = '\0';
   }

   return(ret_val);
}

/*
 * Routine   : attrib()
 * Purpose   : return the attribute of the current text line. update copy!
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : attribute of the current text line
 *
 */

BytT ConLine::attrib()
{
   copyAttr = origAttr;
   return(origAttr);
}

