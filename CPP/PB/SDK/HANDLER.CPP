#include "pb_sdk.h"

extern "C" int
hangup()
{
 Log(0,"Pex handler activated!");

 return HANDLED;
}

void
main()
{
 int i;

 InstallHandler(HANDLER_HANGUP,hangup);

 for(i=0;i<500;i++)
   {
    EnableStop();
    printf("test...");
   }

 ExitTSR();
 //RemoveHandler(HANDLER_HANGUP);
}

