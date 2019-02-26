
/* ------------------------------------------------------------
 * Filename ............... Th_Form.Cpp
 *
 * General Purpose ........ Formatted Pages class
 * ------------------------------------------------------------
 * First date ............. 24 sep 1993
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
#  include <string.h>
#endif

#define Uses_FormTxt
#include <TheHouse.Hpp>

// ---[ Code ]-------------------------------------------------------------

/*
 * Name       : FormTxt()
 * Purpose    : Default constructor
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

FormTxt::FormTxt()
   : File()
{
   curLine = numLines = 0;
   pageNum = 1;
   headerDisabled = FALSE;
}

/*
 * Name       : FormTxt()
 * Purpose    : constructor with parameters
 * -------------------------------------------------------------------------
 * Parameters : filename, mode,buffersize
 * Return     : None
 */

FormTxt::FormTxt(ChrP fname, WrdT mode, WrdT bufsize)
   : File(fname, mode, bufsize)
{
   curLine = numLines = 0;
   pageNum = 1;
   headerDisabled = FALSE;
}

/*
 * Name       : setSize()
 * Purpose    : set the size of the page
 * -------------------------------------------------------------------------
 * Parameters : number of lines
 * Return     : None
 */

void FormTxt::setSize(ShrtT lines)
{
   numLines   = lines;
   curLine    = 0;
}

/*
 * Name       : setHeader()
 * Purpose    : set the text to use as the header
 * -------------------------------------------------------------------------
 * Parameters : text
 * Return     : none
 */

void FormTxt::setHeader(ChrP text)
{
   String tLine(" ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป\n");
   String cLine("");
   String bLine(" ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ\n");

   String header(" ");
   header << text << " ";
   ShrtT space = (80 - header.len());
   ShrtT lSpace = space / 2;
   ShrtT rSpace = space - lSpace;

   cLine << " บ" << String('ฐ', lSpace - 2) << header << String('ฐ', rSpace - 2) << "บ\n";

   headerText = "";
   headerText << "\n" << tLine << cLine << bLine << "\n";
   headerSize = countLines(headerText);
}

/*
 * Name       : addHeader()
 * Purpose    : add some text to the header
 * -------------------------------------------------------------------------
 * Parameters : text to add
 * Return     : none
 */

void FormTxt::addHeader(ChrP text)
{
   headerText << text;
   headerSize = countLines(headerText);
}

/*
 * Name       : onTop()
 * Purpose    : check if we at on top of a page
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : TRUE if ON TOP, FALSE if NOT
 */

FlgT FormTxt::onTop()
{
  return( ((curLine == 0) ? TRUE : FALSE) );
}

/*
 * Name       : disableHeader()
 * Purpose    : Don't write a header the next time it is required
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void FormTxt::disableHeader()
{
   headerDisabled = TRUE;
}

/*
 * Name       : newPage()
 * Purpose    : start a new page
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void FormTxt::newPage()
{
   footer();
   formFeed();
}

/*
 * Name       : formFeed()
 * Purpose    : send a formfeed character
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void FormTxt::formFeed()
{
   write("\r\f", 2);
   curLine = 0;
   pageNum++;
   headerDisabled = FALSE;
}

/*
 * Name       : getPageNum()
 * Purpose    : return the page number
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

ShrtT FormTxt::getPageNum()
{
   return(pageNum);
}

/*
 * Name       : header()
 * Purpose    : write the header
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void FormTxt::header()
{
   if(headerText.len() != 0)
   {
      write(headerText, headerText.len());
      curLine += countLines(headerText);
   }
}
/*
 * Name       : footer()
 * Purpose    : write the footer
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void FormTxt::footer()
{
   String footer("\n\n");

   ShrtT skip = numLines - curLine - 1;
   for(ShrtT cnt = 0; cnt < skip; cnt++)
      footer << "\n";

   write(footer, footer.len());
   curLine += countLines(footer);

   footer = form("-=๐ %d ๐=-", pageNum);
   footer.justify(String::centered, 80);

   write(footer, footer.len());
   curLine += countLines(footer);
}

/*
 * Name       : countLines()
 * Purpose    : count the number of lines in a text block
 * -------------------------------------------------------------------------
 * Parameters : text block
 * Return     : number of lines
 */

ShrtT FormTxt::countLines(ChrP text)
{
   ShrtT ret_val = 0;
   ShrtT len = strlen(text);

   if(len != 0)
   {
      for(ShrtT cnt = 0; cnt < len; cnt++)
         if(text[cnt] == '\n')
            ret_val++;
   }

   return(ret_val);
}

/*
 * Name       : operator << (ChrP)
 * Purpose    : write string to file
 * -------------------------------------------------------------------------
 * Parameters : text
 * Return     : Reference to output stream
 */

FormTxt& FormTxt::operator <<(ChrP text)
{
   String block(text);
   ShrtT freeLines = numLines - curLine - 3;
   ShrtT useLines  = countLines(block);
   if(freeLines < useLines)
      newPage();

   if(onTop() == TRUE)
      if(headerDisabled == FALSE)
         header();

   write(block, block.len());
   curLine += useLines;

   return *this;
}

/*---------------------------] END OF THE CODE [---------------------------*/
