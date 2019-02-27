#include <pb_sdk.h>
#include <tswin.hpp>

/*** Demonstreert de terminal-eigenschappen van een Window object ***/
/***                                                              ***/
/***   In ProBoard: run pex met data "TEST <file>"                ***/


void
main(int,char *argv[])
{
 File f;

 Window w(10,4,70,20,0x3F);
 w.open();

 if(!f.open(argv[1],fmode_read)) return;

 for(;;)
 {
    int c = f.readbyte();

    if(c<0) break;

    w << char(c);
 }

 KB.get();

 // File wordt automatisch gesloten
}

