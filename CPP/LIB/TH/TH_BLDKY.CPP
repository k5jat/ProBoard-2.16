
/* ------------------------------------------------------------
 * Filename ............... Th_BldKy.Cpp
 *
 * General Purpose ........ Classes for the key generator
 * ------------------------------------------------------------
 * First date ............. 6 october 1993
 *
 * First in version ....... 2.02
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

#ifndef PEX                               // module only for non-pex files

/*-------------------------------------------------------------------------*/

#  include <stdlib.h>
#  include <string.h>

#  define Uses_ProKeng
#  define Uses_ProKengBuild
#  include <TheHouse.Hpp>

// ---[ Code ]-------------------------------------------------------------

/*
 * Name       : ProKengRnd()
 * Purpose    : The constructor
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

ProKengRnd::ProKengRnd(ChrP project)
{
    binaryFname  = FileName(project,".Rnd");    binaryFname .mixedCase();
    includeFname = FileName(project,".Reg");    includeFname.mixedCase();
}

/*
 * Name       : exist()
 * Purpose    : check of the random file for the specified project exists
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT ProKengRnd::exist()
{
    DirScan find;
    return(find.first(binaryFname));
}

/*
 * Name       : fillRnd()
 * Purpose    : fill the tables with random values
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void ProKengRnd::fillRnd()
{
    for(UShrtT cnt = 0; cnt < regTableLen; cnt++)
    {
        rnd.t1[cnt] = BytT(random(255));
        rnd.t2[cnt] = BytT(random(255));
    }
}

/*
 * Name       : create()
 * Purpose    : create the random file
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : TRUE if ON, FALSE if NOT
 */

FlgT ProKengRnd::create()
{
    FlgT ret_val = TRUE;

    if(    (binaryFile .open(binaryFname , fmode_create           ) == FALSE)
        || (includeFile.open(includeFname, fmode_create|fmode_text) == FALSE)
      )
        return(FALSE);

    srand((word)clockticks());

    writeHppHeader();

    for(UShrtT cnt = 0; cnt < regMaxTable; cnt++)
    {
        fillRnd();
        writeRnd(cnt);
        writeHpp(cnt);
    }

    writeHppFooter();
    binaryFile.close();
    includeFile.close();
    return(ret_val);
}

/*
 * Name       : writeHppHeader()
 * Purpose    : write the HPP header
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void ProKengRnd::writeHppHeader()
{
    includeFile << "// ------------------------------------------------------------------------\n\n";
}

/*
 * Name       : writeHppFooter()
 * Purpose    : write the HPP footer
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

void ProKengRnd::writeHppFooter()
{
    includeFile << "// ===[ End of file ]======================================================\n";
}

/*
 * Name       : writeHpp()
 * Purpose    : write an element of the table to the HPP file
 * -------------------------------------------------------------------------
 * Parameters : table number
 * Return     : None
 */

void ProKengRnd::writeHpp(UShrtT table)
{
    String text("");

    text    << "#if(regSelectedTable == " << IntT(table) << ")\n"
            <<  "    BytT regTable1[] =\n"
            <<  "    {\n";

    UShrtT cnt;
    for(cnt = 0; cnt < regTableLen; cnt += 8)
    {
        String line = form("        0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X,\n",
                            rnd.t1[cnt + 0],    rnd.t1[cnt + 1],
                            rnd.t1[cnt + 2],    rnd.t1[cnt + 3],
                            rnd.t1[cnt + 4],    rnd.t1[cnt + 5],
                            rnd.t1[cnt + 6],    rnd.t1[cnt + 7]);

        text << line;
    }

    text    << "    };\n"
            <<  "\n"
            <<  "\n"
            <<  "    BytT regTable2[] =\n"
            <<  "    {\n";

    for(cnt = 0; cnt < regTableLen; cnt += 8)
    {
        String line = form("        0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X,\n",
                            rnd.t2[cnt + 0],    rnd.t2[cnt + 1],
                            rnd.t2[cnt + 2],    rnd.t2[cnt + 3],
                            rnd.t2[cnt + 4],    rnd.t2[cnt + 5],
                            rnd.t2[cnt + 6],    rnd.t2[cnt + 7]);

        text << line;
    }

    text    << "    };\n"
            <<  "\n"
            <<  "#endif                              "
            << "// #if(regSelectedTable == " << IntT(table) << ")\n"
            << "\n\n";

    includeFile << ChrP(text);
}

/*
 * Name       : writeRnd()
 * Purpose    : write a record to the binary file
 * -------------------------------------------------------------------------
 * Parameters : table number
 * Return     : none
 */

void ProKengRnd::writeRnd(UShrtT table)
{
    UShrtT pos = table * 2 * regTableLen;
    if(binaryFile.seek(pos, seek_set) == pos)
    {
        binaryFile.write(rnd.t1, regTableLen);
        binaryFile.write(rnd.t2, regTableLen);
    }
}

/*
 * Name       : openTable()
 * Purpose    : open random binary file for reading
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT ProKengRnd::openTable()
{
    return(binaryFile.open(binaryFname, fmode_read));
}

/*
 * Name       : readTable()
 * Purpose    : read a table record
 * -------------------------------------------------------------------------
 * Parameters : table number
 * Return     : None
 */
void ProKengRnd::readTable(UShrtT table)
{
    UShrtT pos = table * 2 * regTableLen;
    if(binaryFile.seek(pos, seek_set) == pos)
    {
        binaryFile.read(&rnd.t1, regTableLen);
        binaryFile.read(&rnd.t2, regTableLen);
    }
}

/*--] Code [---------------------------------------------------------------*/

/*
 * Name       : ProKengBld()
 * Purpose    : the constructor
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

ProKengBld::ProKengBld()
   : ProKeng()
{
   srand(clockticks());

   for(UShrtT cnt = 0; cnt < randomLen; cnt++)
      data.random[cnt] = random(255);
}

/*
 * Name       : create()
 * Purpose    : create the keyfile
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

FlgT ProKengBld::create(ProKengRndP rnd)
{
    FlgT ret_val = TRUE;

    strcpy(data.copyright,"(C) 1993 by Alain Schellinck. Reverse Engineering prohibited!");

    if(rnd->openTable() == TRUE)
    {
        for(UShrtT cnt = 0; cnt < regMaxTable; cnt++)
        {
            rnd->readTable(cnt);
            data.keycodes[cnt] = calc(rnd->rnd.t1, rnd->rnd.t2);
        }

      Date today(TODAY);
      strcpy(data.created, today.format("DD-MM-CCYY"));

      scramble();
      data.checksum = crc();

        ret_val = write();
    }
    else
        ret_val = FALSE;

    return(ret_val);
}

/*
 * Name       : write()
 * Purpose    : write the keyfile
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT ProKengBld::write()
{
    File out;
    FlgT ret_val = FALSE;

    if(out.open(fname, fmode_create) == TRUE)
        if(out.write(&data, sizeof(KeyRecT)) == sizeof(KeyRecT))
        {
            out.close();
            ret_val = TRUE;
        }

    return(ret_val);
}

/*-------------------------------------------------------------------------*/

#endif                        // ifndef PEX (module only for non-pex files)

/*---------------------------] END OF THE CODE [---------------------------*/
