#include "pb_sdk.h"

bool
find_string(char *search,char *text)
{
   int slen,l,i;

   slen = strlen(search);

   l = strlen(text);

   for(i=0;i<=l-slen;i++)
      if(!memicmp(&text[i],search,slen))
      {
         return TRUE;
      }

   return FALSE;
}

void
showbusy()
{
   static int x = 0;
   static char s[] = "|\\-/";

   putchar('\b');
   putchar(s[x]);

   x = ++x % 4;
}

void
main(int argc,char *argv[])
{
   int i,result;
   MESSAGE msg;
   MSGAREA ma;
   char search[30];
   int area = CurUser->msgArea;
   char *text;

   search[0] = '\0';


   if(argc > 1) area = atoi(argv[1]);

   if(argc > 2)
      strncpy(search,argv[2],29);

   search[29] = 0;

   if(area)
   {
      if(!ReadMsgArea(area,&ma))
      {
         Log(LOG_FRIEND,"Can't read message area %d",area);
         return;
      }
   }
   else
   {
      printf("\n\n\6Text search not available in combined mode!"
            "\n\n\7Press \3[Enter]\7 to continue.\t");

      return;
   }

   printf("\n\f\n\6Text Search (\3%s\6)\n"
               "\7컴컴컴컴컴�\n\n",ma.name);

   if(search[0] == '\0')
   {
      printf("\7Search string: \3");

      Input(search,29,INPUT_UPALL);
   }

   if(search[0] == '\0') return;

   UnMarkAllMessages();

   printf("\n\n\7Press [\3S\7] to abort search. Searching for '\3%s\7'...\6  ",search);

   text = malloc(4000);

   result = FirstMessage(&msg,area,1,1);

   for(i=0;result;)
   {
      showbusy();

      CreateMessageTextString(&msg,text,4000);

      if(toupper(PeekChar()) == 'S') break;

      if(find_string(search,text))
      {
         MarkMessage(msg.area,msg.id);

         i++;
      }

      result = NextMessage(&msg,area,1);
   }

   free(text);

   if(i)
   {
      char c;

      printf("\n\n\3%d \7messages with matching text found & marked.\n\n",i);

      for(;;)
      {
         printf("\7Action? \6[\3R\6]ead - [\3L\6]ist - [\3Q\6]uit\7 : ");

         c = WaitKeys("RLQ\r");

         putchar(c);
         putchar('\n');

         if(c == 'Q') break;
         if(c == 'R')
         {
            ReadMarkedMessages();
            break;
         }
         if(c == 'L')
         {
            ListMarkedMessages();
         }

         putchar('\n');
      }
   }
   else
   {
      puts("\n\n\6No messages found. \7Press \3[Enter]\7 to continue.\t");
   }
}


