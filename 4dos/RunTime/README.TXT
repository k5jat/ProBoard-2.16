
                               4DOS v6.02

                      README.TXT -- April 12, 1999

                            JP Software Inc.
                             P.O. Box 1470
                      E. Arlington, MA 02474, USA

                             (781) 646-3975
                           fax (781) 646-0904
                         Email sales@jpsoft.com
                       Web http://www.jpsoft.com/


Greetings, and thanks for trying 4DOS!

This file contains a variety of information you should read before using
4DOS, including:

      Installation and "Branding" Instructions
      New Version Overview
      Revision Information
      Files Included with this Version
      Technical Support
      Legal Notices

Other information is available in other files:

      For ...                                See ...
      -----------------------------------    ------------
      Product and upgrade information        PRODUCTS.TXT
      New customer order form                ORDFORM.TXT
      Current customer upgrade order form    UPGFORM.TXT
      Introduction and Installation Guide    4DOS.TXT
         (introductory documentation)
      Complete list of new features          Online help
      Trial version information              TRIAL.TXT
      License agreement                      LICENSE.TXT


INSTALLATION AND "BRANDING" INSTRUCTIONS
----------------------------------------

   This section explains how to install a downloaded copy of
   4DOS.
   
   [If you have a copy on diskette or CD, or a copy purchased
   electronically, we assume you have already completed the installation
   -- otherwise this file would not be visible!  If you need to
   reinstall your purchased copy of 4DOS, just restart
   the INSTALL program.]

   There are just three steps you need to take to install a downloaded
   copy of 4DOS:

      >> Extract all the files to a new directory (do not use a directory
         you're using for other products, or for a previous version of
         4DOS).

      >> Start 4DOS.  It will "install itself".

      >> If you are upgrading a previously purchased copy, "brand"
         4DOS with your name and serial number if necessary.

   Additional details on these steps are below.  For branding and manual
   installation instructions see the sections with those headings below.

   When installing 4DOS under Windows 95/98 we recommend that you do NOT
   use a "long" directory name.  If you do, you will have to know and
   use the equivalent short name to load 4DOS in CONFIG.SYS; it's easier
   to simply use a short name from the beginning.  (This is not a 4DOS
   limitation.  It's because DOS, which starts before Windows 95/98 and
   loads the primary command processor, cannot handle long file names.
   While 4DOS fully supports long file names, to make DOS work well it
   is best to install 4DOS in a directory with a short name.)

   You can view any of the documentation files on-line, or copy them to
   your printer.  The Introduction and Installation Guide in 4DOS.TXT
   has page breaks, headers and footers; all other documentation files
   are unpaginated ASCII text.


"Self-Installation"

   To begin the installation process, simply run the program file,
   4DOS.COM, from a prompt.  This will automatically start the
   self-installation file _4INST.BTM, which will finish installing
   4DOS on your system.

   _4INST will prompt you for permission before each installation step,
   and no system files will be modified without asking you first.
   
   When the installation is finished, _4INST renames itself to _4INST.BTX,
   so that it will not be run again.  (If installation terminates
   abnormally, _4INST may not be renamed, and will then run again the
   next time you start the program.)


Manual Installation

   For complete manual installation instructions, see the Introduction
   and Installation Guide in the file 4DOS.TXT.  The detailed
   instructions for 4DOS are in Chapter 5.
   
   If you choose to perform a manual installation you should rename the
   automated installation file, _4INST.BTM (we recommend renaming it to
   _4INST.BTX, which is what the file itself does when it's finished).
   If you don't, it will start automatically the first time you run
   4DOS.


Branding

   If you are upgrading from a previously purchased copy you may need to
   run the "branding" program to install your name and serial number
   in 4DOS (trial version users can skip this section).

   In most cases the self-installation process will run the brand program
   for you if it is available.  If it is not, or you need to perform the
   branding manually for another reason, use the instructions here.

   Your previous version's brand program may be the older version named
   BRAND.EXE, or the newer version named BR4DOS.EXE.  In order to
   perform the branding you will need the brand program, and the
   registered name, serial number, and validation code which came with
   your purchased copy of 4DOS.

   (If you have lost your branding information it is replaceable, but a
   small charge is required as recreating the codes is a multi-step
   process.  To request replacement codes contact JP Software at
   sales@jpsoft.com, or at one of the addresses listed at the start of
   this file.  (We are currently hoping to make replacement brand codes
   available through the support area of our web site at
   http://www.jpsoft.com/, but we do not yet know when this service will
   be available.)

   To run either BRAND.EXE or BR4DOS.EXE use these simple steps:

      (1) Start 4DOS or go to an existing 4DOS prompt.

      (2) Change to the directory where 4DOS.COM is installed, and
      verify that both 4DOS.COM and BRAND.EXE or BR4DOS.EXE are in
      that directory.

      (3) Locate the branding information which came with your copy of
      4DOS (information from other products won't work!).

      (4) Enter the command BRAND or BR4DOS and follow the instructions
      on the screen.

   If you attempt to brand your downloaded copy and receive the message
   "brand installation error 4" or "contact JP Software or your dealer
   for an upgrade", then you are not eligible to download and brand a
   trial version of this release -- instead, you must purchase the
   upgrade.  See UPGFORM.TXT for an order form.  Once you have ordered
   an upgrade you can continue to use the trial version until your
   upgrade arrives.

   If you receive any other error, you have probably mistyped your name,
   serial number, or validation code, or are attempting to use the brand
   program or codes from one product with another product, which will
   not work.


NEW VERSION OVERVIEW
--------------------

Since the last major release we've added 150 new features to
4DOS!

Here are a very small number of major highlights; for complete details,
see the online help:

   >> Extended Directory Searches allow you to change to a directory
      anywhere on your system by entering only part of its name.  See
      Directory Navigation in the help for complete details.

   >> New commands include:

          OPTION:  Offers complete configuration adjustment in all
          products, either through interactive dialogs or on the command
          line.

          SWITCH:  Provides for "case" statements in batch files.

          TOUCH:  Adjusts file dates and times.

          TREE:  Displays the directory tree, with or without file
          names, in a variety of formats.

   >> New file exclusion ranges provide a convenient way to exclude
      files from any internal command -- faster and more flexible than
      using EXCEPT.

   >> The new batch file debugger can execute each line step by step,
      process or trace into additional batch files, and display
      variables, aliases, and expanded commands at each step.

See the online help for 150+ additional new command line tools,
switches, variables, functions and other enhancements!


REVISION INFORMATION
--------------------

This update fixes a range of minor problems in version 6.01.  For
complete details on all bug fixes and compatibility enhancements see
the "What's New" topic in the online help.


FILES INCLUDED
--------------

   The following files are included with 4DOS 6.02:

         4DOS.COM          4DOS program file
         4HELP.EXE         4DOS help program
         4DOS.HLP          4DOS help text
         4DOS.TXT          ASCII copy of the 4DOS, 4OS2, and 4NT
                              Introduction and Installation Guide
         _4INST.BTM        *Self-installation batch file
         4DOS.ICO          Icon file for 4DOS under Windows 3.x
         4DOSOS2.ICO       Icon file for 4DOS under OS/2
         4DOS.PIF          PIF file for running 4DOS under Windows 3.x
         BATCOMP.EXE       Batch file compression utility
         BR4DOS.EXE        #Brand program for registered users to
                              install name and product serial number
         EXAMPLES.BTM      Sample batch file demonstrating the use of
                              variable functions and internal variables
         FILE_ID.DIZ       *Description file for BBS and other uploads
         HELPCFG.EXE       Color configuration program for the help
                              system
         INSTHELP.EXE      Installation helper used when installing 4DOS
                              under DOS and Windows 95/98
         INTERNAT.TXT      Shipping rates and dealer list for
                              international customers
         JP4DOS.INF        Setup file to install 4DOS registry extensions
                              under Windows 95/98
         JPOS2INS.CMD      Installation helper used when installing 4DOS
                              under OS/2
         JP4DOSSC.INF      Setup file to install JP Software program
                              group and shortcuts under Windows 95/98
         KSTACK.COM        Memory-resident program used by the KEYSTACK
                              command
         LICENSE.TXT       License agreement
         OPTION.EXE        Program used by the OPTION command
         ORDFORM.TXT       New customer order form
         PRODUCTS.TXT      Product and ordering information
         README.TXT        This file
         TRIAL.TXT         Explanation of trial version
         UPGFORM.TXT       Upgrade order form
         VENDOR.TXT        *Information for shareware vendors

      * Included with downloaded trial copies only.
      # Included with purchased copies only.


TECHNICAL SUPPORT
-----------------

   Complete details on technical support are in the Introduction and
   Installation Guide, in the file 4DOS.TXT.

   Often the quickest way to reach us for support is electronically.
   Current options are listed below; any additional options offered or
   changes made in the future will be listed on our web site at
   http://www.jpsoft.com/.

   In brief, your support options are:

      >> Internet email to support@jpsoft.com.

      >> JP Software's CompuServe area, accessible via the Quick
         Reference Word "JPSOFT" or directly in section 2 of the
         "CVTHREE" forum.  For support leave a message in this area
         addressed to user ID 75300,1215.

      >> Telephone support on a one business day callback basis (though
         often we return calls much more quickly than that).

         From the US and Canada, the quickest method to reach our
         support staff by phone is to leave a message on our support
         voice message line at 781-646-0798.  Messages are checked
         regularly throughout the day.  Do not use this line for sales,
         customer service, or shipping questions, and please call our
         main number at 781-646-3975 if you find yourself playing
         "telephone tag" with the support staff.

      >> Fax support at 781-646-0904 (however, you will almost always
         receive a faster response to email than to faxed support
         questions).

   When possible, we also read messages posted on the Usenet
   comp.os.msdos.4dos newsgroup.  This newsgroup offers valuable
   information and discussions with other users, but is not managed by
   JP Software, and is not an official support channel.

   To download JP Software files, including upgdates and trial versions,
   visit:

      >> Our web site at http://www.jpsoft.com/

      >> Our ftp site at ftp.jpsoft.com

      >> Our CompuServe file area, accessible via the Quick Reference
         Word "JPSOFT" or directly in library 2 of the "CVTHREE" forum


LEGAL STUFF
-----------

Copyright 1999, JP Software Inc., All Rights Reserved.  Published by JP
Software Inc., P.O. Box 1470, E. Arlington, MA 02474, U.S.A., phone
781-646-3975, fax 781-646-0904.

Take Command and 4DOS are registered trademarks of JP Software Inc.
4DOS, 4OS2, and 4NT are JP Software Inc.'s trademarks for its family of
character-mode command processors.  JP Software, jpsoft.com, and all JP
Software designs and logos are also trademarks of JP Software Inc. Other
product and company names are trademarks of their respective owners.

All prices and terms subject to change without notice.

