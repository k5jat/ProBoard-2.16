#include <stdio.h>
#include <tswin.hpp>

main(int argc,char *argv[])
{
   char *choices[] = { "Item number 1", "And this is item #2", "And number 3",NULL};

   tsw_selbox(10,10,4,0x1F,choices,EXPLODE|SHADOW);

   //SCREEN.selBox(choices);
}
