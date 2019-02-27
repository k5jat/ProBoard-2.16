
/* ------------------------------------------------------------
 * Filename ............... Text.Cpp
 *
 * General Purpose ........ Text class
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

#include "text.hpp"
#include "setup.hpp"
#include "keyboard.hpp"
#include "banner.hpp"

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : checkBuffer()
 * Purpose   : check if there are binary characters in the file
 * ------------------------------------------------------------------------
 * Parameters: buffer, size
 * Return    : True if OK, False if NOT (false = binary chars)
 *
 */

LCL FlgT checkBuffer(ChrP ptr, WrdT size)
{
   FlgT ret_val = True;

   for(WrdT cnt = 0; cnt < size && ret_val == True; cnt++)
   {
      BytT c = BytT(*(ptr + cnt));
      if(! (c >= 32 || c == 13 || c == 10 || c == 9 || c == 26))
         ret_val = False;
   }

   return(ret_val);
}

/*--] Code [-------------------------------------] Text [------------------*/

/*
 * Routine   : Text()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: console object
 * Return    : None
 *
 */

Text::Text(ConsoleP con)
{
   console     = con;
   editable    = True;
   text        = ChrP(malloc(TSetup->maxText * sizeof(ChrT)));
   upperY      = 0;
   width       = 0;
   height      = 0;
   abbrevList  = NULL;

   clear();
}

/*
 * Routine   : ~Text()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Text::~Text()
{
   free(text);
}

/*--] Code [-----------------> PROTECTED <-------] Text [-----------------*/

/*
 * Routine   : makeGap()
 * Purpose   : make some free space at the current position
 * ------------------------------------------------------------------------
 * Parameters: number of bytes needed in the gap
 * Return    : True if OK, False if NOT
 *
 */

FlgT Text::makeGap(WrdT gapSize)
{
   FlgT ret_val = False;

   if(curPos + gapSize < TSetup->maxText)
   {
      if(length > 0)
      {
         memmove( text + curPos + gapSize,
                  text + curPos          ,
                  length - curPos
                );
      }

      memset(text + curPos, ' ', gapSize);
      length += gapSize;
      ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : findCursor()
 * Purpose   : find the current X and Y coordinates for the cursor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None (set cursor object)
 *
 */

void Text::findCursor()
{
   topPos = rows.get(topLine);
   botPos = 0;

   for(IntT i = topLine + height - 1; i >= topLine; i--)
   {
      if(rows.exist(i) || i == 0)
      {
         WrdT start = rows.get(i);
         WrdT llen  = rows.llen(i, length);
         WrdT end   = start + llen;

         if(botPos == 0 && end >= 1)
            botPos = end - 1;

         if(curPos >= start && curPos <= end)
         {
            cursor.x = BytT(curPos - start  );
            cursor.y = BytT(i      - topLine);
            break;
         }
      }
   }
}

/*
 * Routine   : setPos()
 * Purpose   : set the position for the specified X value on a line
 * ------------------------------------------------------------------------
 * Parameters: X and line value. If X is past line length, goto end of line
 * Return    : None
 *
 */

void Text::setPos(BytT x, IntT line)
{
   if(rows.exist(line))
   {
      WrdT llen = rows.llen(line, length);
      if(x < llen)
         curPos = rows.get(line) + x;
      else
         curPos = rows.get(line) + llen - (llen > 0);
   }
}

/*
 * Routine   : scrollUp()
 * Purpose   : scroll the window up
 * ------------------------------------------------------------------------
 * Parameters: None (virtual function can change number of lines)
 * Return    : True if scrolled, False if NOT
 *
 */

FlgT Text::scrollUp()
{
   FlgT ret_val = False;

   if(curLine() + 1 < rows.last())
   {
      if(TSetup->fastScroll)
      {
         topLine += cursor.y;
         cursor.y = 0;
      }
      else
      {
         topLine++;
         console->scrollUp(upperY, height);
      }

      ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : scrollDown()
 * Purpose   : scroll the window down
 * ------------------------------------------------------------------------
 * Parameters: None (virtual function can change number of lines)
 * Return    : True if scrolled, False if NOT
 *
 */

FlgT Text::scrollDown()
{
   FlgT ret_val = False;

   if(topLine > 0)
   {
      if(TSetup->fastScroll)
      {
         IntT moved = min(height - 1, topLine);

         topLine -= moved;
         cursor.y += moved;
      }
      else
      {
         topLine--;
         console->scrollDown(upperY, height);
      }

      ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : getPos()
 * Purpose   : return the current position
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : current position
 *
 */

WrdT Text::getPos()
{
   return(curPos);
}

/*
 * Routine   : seek()
 * Purpose   : move to a specified location
 * ------------------------------------------------------------------------
 * Parameters: location to move to
 * Return    : none
 *
 */

void Text::seek(WrdT p)
{
   if(p <= length)
   {
      curPos = p;

      while(curPos > botPos)
      {
         topLine++;
         format();
      }
   }
}

/*--] Code [-----------------> PUBLIC <----------] Text [-----------------*/

/*
 * Routine   : clear();
 * Purpose   : clear the whole buffer
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : always TRUE
 *
 */

FlgT Text::clear()
{
   needsUpdate = True;

   length      = 0;
   curPos      = 0;
   cursor.x    = 0;
   cursor.y    = 0;
   topPos      = 0;
   botPos      = 0;
   topLine     = 0;

   memset(text, 0, TSetup->maxText * sizeof(ChrT));
   rows.clear();

   return(True);
}

/*
 * Routine   : curLine()
 * Purpose   : return the current line number
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : current line number
 *
 */

IntT Text::curLine()
{
   return(topLine + cursor.y);
}

/*
 * Routine   : get()
 * Purpose   : get the data for the specified line
 * ------------------------------------------------------------------------
 * Parameters: line number
 * Return    : pointer to buffer or NULL if line doesn't exist
 *
 */

ChrP Text::get(IntT lineNo)
{
   ChrP ret_val = NULL;

   if(rows.exist(lineNo))
   {
      WrdT start = rows.get(lineNo);
      WrdT llen  = rows.llen(lineNo, length);
      memcpy(&buffer[0], &text[start], llen);
      buffer[llen] = '\0';
      ret_val = &buffer[0];
   }

   return(ret_val);
}

/*
 * Routine   : getLastWord()
 * Purpose   : get the word just before the current cursor position
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : pointer to that word
 *
 */

ChrP Text::getLastWord()
{
   static ChrT ret_val[32];

   memset(ret_val, 0, 32);

   WrdT tempPos = curPos;

   if(isSeparator(text[tempPos - 1] == True))
   {
      tempPos--;

      while(isSeparator(text[tempPos - 1]) == True && tempPos > 0)
         tempPos--;                     // skip spaces

      while(isSeparator(text[tempPos - 1]) == False && tempPos > 0)
         tempPos--;                     // skip to start of the word
   }
   else
   {
      while(isSeparator(text[tempPos - 1]) == False && tempPos > 0)
         tempPos--;                     // skip to start of the word
   }

   int cnt = 0;
   while(!isSeparator(text[tempPos]) && cnt < 32)
      ret_val[cnt++] = text[tempPos++];

   return(ret_val);
}

/*
 * Routine   : getLastSentence()
 * Purpose   : get the sentence just before the current cursor position
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : pointer to that sentence
 *
 */

ChrP Text::getLastSentence()
{
   static ChrT ret_val[1024];

   memset(ret_val, 0, 1024);

   WrdT tempPos = curPos;
   if(tempPos > 0)
   {
      while((text[tempPos] == '\n' || text[tempPos] == '\0') && tempPos > 0)
         tempPos--;

      if(tempPos > 0)
      {
         while(text[tempPos] != '\n' && tempPos > 0)
            tempPos--;

         if(text[tempPos] == '\n')
            tempPos++;
      }
   }

   WrdT cnt = 0;
   while(cnt < 1024 && text[tempPos] != '\n')
      ret_val[cnt++] = text[tempPos++];

   return(ret_val);
}

/*
 * Routine   : setAbbrev()
 * Purpose   : set the abbrevs list
 * ------------------------------------------------------------------------
 * Parameters: abbrevs
 * Return    : None
 *
 */

void Text::setAbbrev(AbbrevList *abbrevs)
{
   abbrevList = abbrevs;
}

/*
 * Routine   : format()
 * Purpose   : reformat the text
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Text::format()
{
   IntT lineCnt = topLine;
   if(lineCnt > 0) lineCnt--;
   if(lineCnt > 0) lineCnt--;

   if(length == 0)
   {
      rows.clear();
   }
   else
   {
      WrdT limit   = min(WrdT(width * (height + 6)), length + 16);
      rows.used    = FormatText(text,
                                rows.lines,
                                lineCnt,
                                width,
                                limit
                               ) + 1;
   }

   findCursor();
}

/*
 * Routine   : isSeparator()
 * Purpose   : check if the specified character is a word separator
 * ------------------------------------------------------------------------
 * Parameters: character
 * Return    : True if a separator, False if NOT
 *
 */

FlgT Text::isSeparator(ChrT c)
{
   return(!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')));
}

/*
 * Routine   : left()
 * Purpose   : move the cursor one position to the left
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if moved, FALSE if NOT
 *
 */

FlgT Text::left()
{
   FlgT ret_val = True;

   if(curPos > topPos)
   {
      curPos--;
      findCursor();
   }
   else
   {
      if(scrollDown() == True)
      {
         curPos--;
         format();
      }
      else
         ret_val = False;
   }

   return(ret_val);
}

/*
 * Routine   : right()
 * Purpose   : move the cursor one position to the right
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : TRUE if moved, FALSE if NOT
 *
 */

FlgT Text::right()
{
   FlgT ret_val = True;

   if(curPos < botPos)
   {
      curPos++;
      findCursor();
   }
   else
   {
      if(scrollUp() == True)
      {
         curPos++;
         format();
      }
      else
         ret_val = False;
   }

   return(ret_val);
}

/*
 * Routine   : down()
 * Purpose   : move one line down
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if MOVED, False if not
 *
 */

FlgT Text::down()
{
   FlgT ret_val = False;

   if(rows.exist(curLine() + 1))
   {
      BytT xOnLine = curPos - rows.get(curLine());
      setPos(xOnLine, curLine() + 1);

      if(curPos > botPos)
      {
         scrollUp();
         format();
      }
      else
         findCursor();

      ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : up()
 * Purpose   : move one line up
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if MOVED, False if not
 *
 */

FlgT Text::up()
{
   FlgT ret_val = True;

   if(curLine() > 0)
   {
      BytT xOnLine = curPos - rows.get(curLine());
      setPos(xOnLine, curLine() - 1);

      if(curPos < topPos)
      {
         scrollDown();
         format();
      }
      else
         findCursor();

      ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : home()
 * Purpose   : move to the home position
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if moved, false if not
 *
 */

FlgT Text::home()
{
   curPos = rows.get(curLine());
   findCursor();
   return(True);
}

/*
 * Routine   : end()
 * Purpose   : move to the end position
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if moved, false if not
 *
 */

FlgT Text::end()
{
   WrdT llen = rows.llen(curLine(), length);
   if(llen > 0)
      curPos = rows.get(curLine()) + llen - 1;

   findCursor();
   return(True);
}

/*
 * Routine   : pageUp()
 * Purpose   : move the cursor one page up
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if moved, False if not
 *
 */

FlgT Text::pageUp()
{
   FlgT ret_val = False;
   IntT cl = curLine();
   if(cl != 0)
   {
      for(IntT cnt = 0; cnt < height; cnt++)
         if(topLine > 0)
            topLine--;
         else
            if(cursor.y > 0)
               cursor.y--;

      if(cl != curLine())
      {
         setPos(cursor.x, topLine + cursor.y);
         findCursor();
         ret_val = True;
      }
   }

   return(ret_val);
}

/*
 * Routine   : pageDown()
 * Purpose   : move the cursor one page down
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if moved, False if not
 *
 */

FlgT Text::pageDown()
{
   FlgT ret_val = False;
   IntT cl = curLine();
   if(cl < rows.last() - 1)
   {
      for(IntT cnt = 0; cnt < height; cnt++)
         if(curLine() < rows.last() - 1)
            topLine++;

      if(cl != curLine())
      {
         setPos(cursor.x, curLine());
         format();
         ret_val = True;
      }
   }

   return(ret_val);
}

/*
 * Routine   : wordLeft()
 * Purpose   : move to the beginning of the previous word
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if moved, False if NOT
 *
 */

FlgT Text::wordLeft()
{
   FlgT ret_val = True;

   if(curPos != 0)
   {
      if(isSeparator(text[curPos - 1] == True))
      {
         curPos--;

         while(isSeparator(text[curPos - 1]) == True && curPos > 0)
            curPos--;                     // skip spaces

         while(isSeparator(text[curPos - 1]) == False && curPos > 0)
            curPos--;                     // skip to start of the word
      }
      else
      {
         while(isSeparator(text[curPos - 1]) == False && curPos > 0)
            curPos--;                     // skip to start of the word
      }

      if(curPos >= topPos)
         findCursor();
      else
      {
         while(curPos < topPos)
         {
            scrollDown();
            findCursor();
         }
      }
   }
   else
      ret_val = False;

   return(ret_val);
}

/*
 * Routine   : wordRight()
 * Purpose   : move to the beginning of the next word
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if moved, False if NOT
 *
 */

FlgT Text::wordRight()
{
   FlgT ret_val = True;

   if(curPos + 1 < length)
   {
      if(isSeparator(text[curPos]) == False)
      {
         while(isSeparator(text[curPos]) == False && curPos < length)
            curPos++;                     // skip to end of current word

         while(isSeparator(text[curPos]) == True && curPos < length)
            curPos++;                     // skip spaces
      }
      else
         while(isSeparator(text[curPos]) == True && curPos < length)
            curPos++;                     // skip spaces

      if(curPos <= botPos)
         findCursor();
      else
      {
         while(curPos > botPos)
         {
            scrollUp();
            format();
         }
      }
   }
   else
      ret_val = False;

   return(ret_val);
}

/*
 * Routine   : toggleInsert()
 * Purpose   : toggle the insert key
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Text::toggleInsert()
{
   TSetup->insertMode = !TSetup->insertMode;
   needsUpdate = True;
}

/*
 * Routine   : toggleAbbrev()
 * Purpose   : toggle the abbrev key
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Text::toggleAbbrev()
{
   TSetup->abbrevMode = !TSetup->abbrevMode;
   needsUpdate = True;
}

/*
 * Routine   : addChar()
 * Purpose   : add a character at the current position
 * ------------------------------------------------------------------------
 * Parameters: character
 * Return    : True if OK, False if NOT
 *
 */

FlgT Text::addChar(ChrT c)
{
   FlgT ret_val = True;

   if(editable)
   {
      // --- test for a separator. If so, check to replace abbrev
      if(isSeparator(c) && abbrevList != NULL && TSetup->abbrevMode == True)
      {
         ChrP lastWord = getLastWord();
         if(lastWord[0] != '\0')
         {
            ChrP replace = abbrevList->get(lastWord);
            if(replace != NULL)
            {
               wordLeft();
               delWord();

               int repLen = strlen(replace);
               makeGap(repLen);
               memcpy(&text[curPos], replace, repLen);
               curPos += repLen;
            }
         }
      }

      if(TSetup->insertMode || text[curPos] == '\n' || c == '\n' || c == 9)
      {
         if(c != '\t')
         {
            makeGap(1);
            text[curPos++] = c;
         }
         else
         {
            BytT newX = (((cursor.x / 4) + 1) * 4);
            if(newX + 1 < width)
            {
               WrdT gapSize = newX - cursor.x;
               makeGap(gapSize);
               curPos += gapSize;
            }
            else
               ret_val = False;
         }
      }
      else
      {
         text[curPos] = c;
         if(curPos == length)
         {
            text[curPos + 1] = '\0';
            length++;
         }

         curPos++;
      }
   }

   format();

   if(curPos > botPos)
   {
      scrollUp();
      format();
   }

   return(ret_val);
}

/*
 * Routine   : pasteLine()
 * Purpose   : insert the specified text from the start of the current line
 * ------------------------------------------------------------------------
 * Parameters: text to add
 * Return    : True if changed, False if NOT
 *
 */

FlgT Text::pasteLine(ChrP t)
{
   FlgT ret_val = False;

   if(editable)
   {
      WrdT llen = strlen(t);
      if(llen > 0)
      {
         if(llen + length < TSetup->maxText && llen > 0)
         {
            curPos = rows.get(curLine());
            makeGap(llen);
            memcpy(text + curPos, t, llen);
            format();
            ret_val = True;
         }
      }
   }

   return(ret_val);
}

/*
 * Routine   : backSpace()
 * Purpose   : Delete the character before the current position
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if changed, false if not
 *
 */

FlgT Text::backSpace()
{
   if(editable && curPos > 0)
   {
      if(curPos < length)
         memmove(&text[curPos - 1], &text[curPos], (length + 1) - curPos);
      else
         text[curPos - 1] = '\0';

      curPos--;
      length--;
   }

   if(curPos < topPos)
      scrollDown();

   format();

   return(editable);
}

/*
 * Routine   : delChar()
 * Purpose   : Delete the character at the current position
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if changed, false if not
 *
 */

FlgT Text::delChar()
{
   if(editable && (curPos != length) && length > 0)
   {
      memmove(&text[curPos], &text[curPos + 1], length - curPos);
      length--;
   }

   format();
   return(editable);
}

/*
 * Routine   : delWord()
 * Purpose   : delete the word right
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if changed, False if not
 *
 */

FlgT Text::delWord()
{
   FlgT ret_val = False;

   WrdT fromPos = curPos;
   wordRight();

   if(curPos > fromPos)
   {
      memmove(text + fromPos, text + curPos, length + 1 - fromPos);
      length -= (curPos - fromPos);
      curPos = fromPos;
      format();

      ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : cutLine()
 * Purpose   : delete the specified line
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if changed, false if not
 *
 */

FlgT Text::cutLine()
{
   FlgT ret_val = False;
   if(editable && length > 0)
   {
      WrdT start = rows.get(curLine());
      WrdT llen  = rows.llen(curLine(), length);
      if(llen > 0)
      {
         if(!rows.exist(curLine() + 1))
         {
            memset(&text[start], 0, llen);
            length = start;
         }
         else
         {
            if(llen != 0)
            {
               memmove( &text[start       ],
                        &text[start + llen],
                        length - (start + llen)
                      );

               length -= llen;
               memset(&text[length], 0, llen);
            }
         }

         curPos  = rows.get(curLine());
         format();
         ret_val = True;
      }
   }

   return(ret_val);
}

/*
 * Routine   : centerLine()
 * Purpose   : center the current line
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if changed, False if NOT
 *
 */

FlgT Text::centerLine()
{
   FlgT ret_val = False;

   ChrP p = get(curLine());
   if(p != NULL)
   {
      IntT leading = 0;

      while(*p == ' ')
      {
         p++;
         leading++;
      }

      IntT len = strlen(p);
      for(IntT cnt = len - 1; cnt > 0 && *p == ' '; cnt--)
         len--;

      if(len > 0)
      {
         IntT space = (console->width - len) / 2;
         if(space > leading)
         {
            WrdT pos = curPos;

            home();
            makeGap(space - leading);
            curPos = pos + space - leading;
            format();

            ret_val = True;
         }
      }
   }

   return(ret_val);
}

/*
 * Routine   : percent
 * Purpose   : check how many percent is used
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : percentage used
 *
 */

WrdT Text::percent()
{
   WrdT linePct = WrdT(rows.last() / (TSetup->maxRows / 100));
   WrdT textPct = WrdT(length      / (TSetup->maxText / 100));
   return(max(linePct, textPct));
}

/*
 * Routine   : import()
 * Purpose   : import a text
 * ------------------------------------------------------------------------
 * Parameters: filename
 * Return    : actual size of the imported text
 *
 */

WrdT Text::import(ChrP fileName)
{
   WrdT ret_val = 0;

   FILE *handle = fopen(fileName, "rt");
   if(handle != NULL)
   {
      ChrP tmp = new ChrT[WrdT(TSetup->maxText)];
      if(tmp != NULL)
      {
         WrdT realSize = WrdT(fread(tmp, 1, WrdT(TSetup->maxText - 1), handle));
         if(realSize > 0)
         {
            if(length >= TSetup->maxText - realSize)
               realSize = TSetup->maxText - length;

            if(checkBuffer(tmp, realSize) == True)
            {
               makeGap(WrdT(realSize));
               memcpy(&text[curPos], tmp, WrdT(realSize));
               text[length] = '\0';
               ret_val = realSize;
            }
            else
               TBanner->putS(TSetup->language->get(FileHasBinChar));
         }

         delete tmp;
      }

      fclose(handle);
      format();
      needsUpdate = True;
   }


   return(ret_val);
}

/*
 * Routine   : export()
 * Purpose   : export the whole text to a file
 * ------------------------------------------------------------------------
 * Parameters: filename
 * Return    : True if OK, False if NOT
 *
 */

FlgT Text::export(ChrP fileName, ChrP footer)
{
   FlgT ret_val = False;

   FILE *handle = fopen(fileName, "wt");
   if(handle != NULL)
   {
      fwrite(text, 1, length, handle);

      if(footer != NULL)
         fprintf(handle, "\n\n%s\n", footer);

      fclose(handle);
      ret_val = True;
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

void Text::update()
{
   for(IntT i = 0; i < height; i++)
   {
      ChrP p = get(topLine + i);
      if(p != NULL)
      {
         console->setLine(upperY + i, p , attrib);
      }
      else
      {
         console->setLine(upperY + i, "", attrib);
      }
   }

   needsUpdate = False;
}

/*
 * Routine   : drawCursor()
 * Purpose   : draw the cursor at the current location
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Text::drawCursor()
{
   console->gotoXY(cursor.x, upperY + cursor.y);
}

/*
 * Routine   : handle()
 * Purpose   : handle the keystroke
 * ------------------------------------------------------------------------
 * Parameters: key to handle
 * Return    : unhandled key or 0
 *
 */

WrdT Text::handle(WrdT key)
{
   WrdT ret_val = 0;

   switch(key)
   {
      // --- ^A -----------------------------------------------------------
      case 0x01         :  needsUpdate = wordLeft        ();         break;
      // --- ^B -----------------------------------------------------------
      case 0x02         :  needsUpdate = centerLine      ();         break;
      // --- ^C -----------------------------------------------------------
      case 0x03         :  needsUpdate = pageDown        ();         break;
      // --- ^D -----------------------------------------------------------
      case 0x04         :  needsUpdate = right           ();         break;
      // --- ^F -----------------------------------------------------------
      case 0x06         :  needsUpdate = wordRight       ();         break;
      // --- ^G -----------------------------------------------------------
      case 0x07         :  needsUpdate = delChar         ();         break;
      // --- ^J -----------------------------------------------------------
    //case 0x0A         :                toggleAbbrev    ();         break;
      // --- ^P -----------------------------------------------------------
      case 0x10         :  needsUpdate = end             ();         break;
      // --- ^Q -----------------------------------------------------------
      case 0x11         :  needsUpdate = clear           ();         break;
      // --- ^R -----------------------------------------------------------
      case 0x12         :  needsUpdate = pageUp          ();         break;
      // --- ^S -----------------------------------------------------------
      case 0x13         :  needsUpdate = left            ();         break;
      // --- ^T -----------------------------------------------------------
      case 0x14         :  needsUpdate = delWord         ();         break;
      // --- ^V -----------------------------------------------------------
      case 0x16         :                toggleInsert    ();         break;
      // --- ^W -----------------------------------------------------------
      case 0x17         :  needsUpdate = home            ();         break;
      // --- ^X -----------------------------------------------------------
      case 0x18         :  needsUpdate = down            ();         break;
      // --- ^Y -----------------------------------------------------------
      case 0x19         :  needsUpdate = cutLine         ();         break;

      // --- DEL ----------------------------------------------------------
      case 0x7F         :  needsUpdate = delChar         ();         break;

      // --- ^LEFT --------------------------------------------------------
      case KEY_CTL_LT   :  needsUpdate = wordLeft        ();         break;
      // --- ^RIGHT--------------------------------------------------------
      case KEY_CTL_RT   :  needsUpdate = wordRight       ();         break;
      // --- PGDOWN -------------------------------------------------------
      case KEY_PGDN     :  needsUpdate = pageDown        ();         break;
      // --- PGUP ---------------------------------------------------------
      case KEY_PGUP     :  needsUpdate = pageUp          ();         break;
      // --- RIGHT --------------------------------------------------------
      case KEY_RT       :  needsUpdate = right           ();         break;
      // --- LEFT ---------------------------------------------------------
      case KEY_LT       :  needsUpdate = left            ();         break;
      // --- UP -----------------------------------------------------------
      case KEY_UP       :  needsUpdate = up              ();         break;
      // --- DOWN ---------------------------------------------------------
      case KEY_DN       :  needsUpdate = down            ();         break;
      // --- HOME ---------------------------------------------------------
      case KEY_HOME     :  needsUpdate = home            ();         break;
      // --- END ----------------------------------------------------------
      case KEY_END      :  needsUpdate = end             ();         break;
      // --- ENTER --------------------------------------------------------
      case KEY_ENT      :  needsUpdate = addChar     ('\n');         break;
      // --- DEL ----------------------------------------------------------
      case KEY_DEL      :  needsUpdate = delChar         ();         break;
      // --- BACK ---------------------------------------------------------
      case KEY_BACK     :  needsUpdate = backSpace       ();         break;
      // --- INSERT--------------------------------------------------------
      case KEY_INS      :                toggleInsert    ();         break;
      // --- OTHER --------------------------------------------------------
      default           :
      {
         if(key > 31 && key < 256 || key == 9)
            needsUpdate = addChar(key);
         else
            ret_val = key;
      }
      break;
   }

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
