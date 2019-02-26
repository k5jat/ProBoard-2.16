
(BC31P1) Patch to Borland C++ Version 3.1
=====================================================================

This patch fixes a problem with the NEW operator, giving assertion
errors and aborting the program.

Run Time Libraries
==================
To patch the Borland C++ Libraries:

    - Place all files extracted from bc31p1.zip with the exception of
      this one in the same directory as your run-time libraries
      (normally \BC\LIB).

    - Change your working directory (cd) to your run-time library 
      directory (normally \BC\LIB)

    - Ensure that TLIB.EXE is in your path or place TLIB.EXE in the same
      directory as the above files.

    - Run bc31p1.bat

gbl

