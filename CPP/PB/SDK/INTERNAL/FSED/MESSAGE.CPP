
/* ------------------------------------------------------------
 * Filename ............... Message.Cpp
 *
 * General Purpose ........ Message class
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

#include "message.hpp"

/*--] Code [-------------------------------------] Message [---------------*/

/*
 * Routine   : getQuote()
 * Purpose   : get the quote string for the specified line
 * ------------------------------------------------------------------------
 * Parameters: line number, buffer to fill
 * Return    : True if line is a quote, False if NOT
 *
 */

FlgT Message::getQuote(IntT lineNo, ChrP buffer)
{
   FlgT ret_val = False;

   if(rows.exist(lineNo))
   {
      WrdT llen  = rows.llen(lineNo, length);
      if(llen > 0)
      {
         WrdT start = rows.get(lineNo);
         for(IntT cnt = 0; cnt < min(8, llen) && ret_val == False; cnt++)
         {
            if(buffer != NULL)
               buffer[cnt] = text[start + cnt];

            if(text[start + cnt] == '>')
               ret_val = True;
         }

         if(buffer != NULL)
         {
            buffer[cnt++] = ' ' ;
            buffer[cnt  ] = '\0';
         }
      }
   }

   return(ret_val);
}

/*
 * Routine   : addChar()
 * Purpose   : add a character in the text
 * ------------------------------------------------------------------------
 * Parameters: character
 * Return    : True if changed, False if NOT
 *
 */

FlgT Message::addChar(ChrT c)
{
   FlgT ret_val = True;

   if(editable)
   {
      if(c == '\n')
      {
         ChrT buffer[12];
         FlgT isQuote = getQuote(curLine(), buffer);
         WrdT quoteLen = strlen(buffer);
         if(cursor.x + 1 < quoteLen || cursor.x + 1 >= rows.llen(curLine(), length))
            isQuote = False;

         if(isQuote)
         {
            WrdT gapSize = quoteLen + 1;
            makeGap(gapSize);
            text[curPos++] = c;

            for(IntT cnt = 0; cnt < quoteLen; cnt++)
               text[curPos++] = buffer[cnt];

            format();
         }
         else
            ret_val = Text::addChar(c);
      }
      else
         ret_val = Text::addChar(c);
   }

   return(ret_val);
}

/*
 * Routine   : update()
 * Purpose   : update the console object
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Message::update()
{
   if(cursor.y >= height)
      while(cursor.y >= height)
      {
         cursor.y--;
         topLine++;
      }

   for(IntT i = 0; i < height; i++)
   {
      ChrP p = get(topLine + i);
      if(p != NULL)
      {
         if(getQuote(topLine + i, NULL))
         {
            console->setLine(i, p, qAttrib);
         }
         else
         {
            console->setLine(i, p, attrib );
         }
      }
      else
      {
         console->setLine(i, ""    , attrib);
      }
   }
}

/*---------------------------] END OF THE CODE [---------------------------*/
