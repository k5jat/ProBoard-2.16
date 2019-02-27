
/* ------------------------------------------------------------
 * Filename ............... Timer.Cpp
 *
 * General Purpose ........ Timer class
 * ------------------------------------------------------------
 * First date ............. 12-04-94 09:00pm
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

#include "timer.hpp"

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : Timer()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Timer::Timer()
{
   stopVal = 0L;
   start();
}

/*
 * Routine   : get()
 * Purpose   : get the current value
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : value returning the current number of clockticks
 *
 */

DwdT Timer::get()
{
   return *((DwdT FAR * )0x46C);
}

/*
 * Routine   : start()
 * Purpose   : start the timer
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Timer::start()
{
   stopped  = False;
   startVal = get();
}

/*
 * Routine   : stop()
 * Purpose   : stop the timer
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Timer::stop()
{
   stopVal = get();
   stopped = True;
}

/*
 * Routine   : elapsed()
 * Purpose   : return the number of elapsed clockticks
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : number of elapsed clock ticks
 *
 */

DwdT Timer::elapsed()
{
   if(!isStopped())
      stopVal = get();

   return(stopVal - startVal);
}

/*
 * Routine   : isStopped()
 * Purpose   : check if the timer is stopped
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if stopped, False if NOT
 *
 */

FlgT Timer::isStopped()
{
   return(stopped);
}

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : sleepClock()
 * Purpose   : let the program wait a number of clockticks
 * ------------------------------------------------------------------------
 * Parameters: clockticks to wait
 * Return    : None
 *
 */

void sleepClock(IntT ticks)
{
   Timer timer;
   while(timer.elapsed() < ticks)
   {
   }
}

/*---------------------------] END OF THE CODE [---------------------------*/














