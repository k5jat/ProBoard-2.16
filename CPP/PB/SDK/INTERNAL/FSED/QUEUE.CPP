
/* ------------------------------------------------------------
 * Filename ............... Queue.Cpp
 *
 * General Purpose ........ Queue class
 * ------------------------------------------------------------
 * First date ............. 12-07-94 10:51am
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

#include "queue.hpp"

/*--] Code [-------------------------------------] Queue [-----------------*/

/*
 * Routine   : Queue
 * Purpose   : The constructor
 * ------------------------------------------------------------------------
 * Parameters: size of the queue
 * Return    : None
 *
 */

Queue::Queue(BytT s)
{
   size = s;
   queue = new WrdT[size];
   purge();
}

/*
 * Routine   : ~Queue()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Queue::~Queue()
{
   delete queue;
}

/*
 * Routine   : peek()
 * Purpose   : look at the 1st word in the keyboard buffer
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : first word or 0 if queue is empty
 *
 */

WrdT Queue::peek()
{
   return(queue[0]);
}

/*
 * Routine   : poke()
 * Purpose   : poke a word at the end of the queue
 * ------------------------------------------------------------------------
 * Parameters: value to poke
 * Return    : True if OK, False if NOT
 *
 */

FlgT Queue::poke(WrdT val)
{
   FlgT ret_val = True;
   if(val != 0)
   {
      if(used < size)
         queue[used++] = val;
      else
         ret_val = False;
   }

   return(ret_val);
}

/*
 * Routine   : get()
 * Purpose   : get a word from the queue
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : word or 0 if queue is empty
 *
 */

WrdT Queue::get()
{
   WrdT ret_val = queue[0];
   if(used > 0)
   {
      if(used > 1)
         memmove(&queue[0], &queue[1], (used - 1) * sizeof(WrdT));

      used--;
      queue[used] = 0;
   }

   return(ret_val);
}

/*
 * Routine   : purge()
 * Purpose   : clear the whole queue
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Queue::purge()
{
   used = 0;
   queue[0] = 0;
}

/*
 * Routine   : isEmpty()
 * Purpose   : check if the queue is empty
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if empty, False if not
 *
 */

FlgT Queue::isEmpty()
{
   return(FlgT(used == 0));
}

/*---------------------------] END OF THE CODE [---------------------------*/
