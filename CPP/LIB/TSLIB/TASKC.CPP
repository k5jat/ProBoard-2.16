#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <stdio.h>
  #include <stdlib.h>
#endif

#include <tslib.hpp>

#define STACKSIZE 1024

struct task
   {
      word ss;
      word sp;

      task *next;
      task *prev;

      word *base;
   };

static task *maintask;
static task *curtask;

extern "C"
   {
      void _switch_task(task *,task *);
      void _set_stack(task *);
   }

inline word
far_seg(void _far *p)
{
   return word(long(p) >> 16);
}

inline unsigned
far_off(void _far *p)
{
   return word(long(p) & 0xFFFF);
}

void
init_tasking()
{
   curtask = maintask = new task;

   maintask->base = 0;

   maintask->next = maintask->prev = maintask;
}

void
launch_task(void (*func)())
{
   task *newtask = new task;
   int x = 0;

   newtask->next       = curtask->next;
   newtask->prev       = curtask;
   curtask->next->prev = newtask;
   curtask->next       = newtask;

   newtask->base = (word *)malloc(STACKSIZE * 2);

#pragma option -w-rch
#pragma option -w-ccc

   if(sizeof(func)>2) newtask->base[STACKSIZE-(++x)]  = far_seg(kill_task);
                      newtask->base[STACKSIZE-(++x)]  = far_off(kill_task);
   if(sizeof(func)>2) newtask->base[STACKSIZE-(++x)]  = far_seg(func);
                      newtask->base[STACKSIZE-(++x)]  = far_off(func);
                      newtask->base[STACKSIZE-(++x)]  = 0;
                      newtask->base[STACKSIZE-(++x)]  = 0;
                      newtask->base[STACKSIZE-(++x)]  = 0;
#pragma option -wrch
#pragma option -wccc

   newtask->ss   = far_seg(newtask->base);
   newtask->sp   = far_off(&(newtask->base[STACKSIZE-x]));

   curtask = newtask;

   _switch_task(curtask->prev,curtask);
}

void
kill_task()
{
   static task *newtask;
   static task scratchtask;

   if(curtask == maintask)
   {
#ifdef PB_SDK
      exit();
#else
      exit(0);
#endif
   }

   newtask = curtask->next;

   curtask->prev->next = curtask->next;
   curtask->next->prev = curtask->prev;

   free(curtask->base);     // Now we don't have a stack!

   delete curtask;          // We're running in empty air!
                            //   but... Who cares!
   curtask = newtask;

   _switch_task(&scratchtask,curtask);
}

void
pause()
{
   curtask = curtask->next;

   _switch_task(curtask->prev,curtask);
}
