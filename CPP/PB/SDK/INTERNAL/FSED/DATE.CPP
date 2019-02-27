
/* ------------------------------------------------------------
 * Filename ............... Date.Cpp
 *
 * General Purpose ........ Date handling class
 * ------------------------------------------------------------
 * First date ............. 12-20-94 03:37pm
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

#include "date.hpp"

/*-------------------------------------------------------------------------*/

const LngT JulianAdjustment = 1721425L;

/*--] Code [-------------------------------------] [-----------------------*/

/*
 * Routine   : monthToDays()
 * Purpose   : Calculates number of days BEFORE the first of a given month
 * ------------------------------------------------------------------------
 * Parameters: month
 * Return    : number of days
 *
 */


LCL IntT monthToDays(IntT month)
{
   return IntT((LngT(month) * 3057 - 3007) / 100);
}

/*
 * Routine   : yearsToDays()
 * Purpose   : Calculates number of days in number of years since 1-1-0001
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : none
 *
 */


LCL LngT yearsToDays(IntT yr)
{
   return yr * 365L + yr / 4 - yr / 100 + yr / 400;
}

/*--] Code [-------------------------------------] Date [------------------*/

/*
 * Routine   : Date()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Date::Date()
{
	time_t tm_val = time(NULL);

	struct tm *tm_struct = localtime(&tm_val);

   _d = IntT(tm_struct->tm_mday);
   _m = IntT(tm_struct->tm_mon + 1);
   _y = IntT(tm_struct->tm_year);
}

/*
 * Routine   : Date()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: day. month, year
 * Return    : None
 *
 */

Date::Date(IntT d, IntT m, IntT y)
{
   _d = ChrT(d);
   _m = ChrT(m);
   _y = ChrT(y);
}

/*
 * Routine   : set()
 * Purpose   : set the date on  a specified day, month and year
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Date::set(IntT day, IntT month, IntT year)
{
   _d = WrdT(day);
   _m = WrdT(month);
   _y = WrdT((year>1900) ? (year-1900) : year);
}

/*
 * Routine   : compare
 * Purpose   : compare the date object with another one
 * ------------------------------------------------------------------------
 * Parameters: other date object
 * Return    : 0 if equal, <> 0 if different
 *
 */

IntT Date::compare(DateR dt)
{
	if(_y - dt._y) return (_y - dt._y);
	if(_m - dt._m) return (_m - dt._m);
	if(_d - dt._d) return (_d - dt._d);

	return 0;
}

/*
 * Routine   : isValid()
 * Purpose   : check if the current date is legal
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if OK, False if NOT
 *
 */

FlgT Date::isValid()
{
   return ((_d < 1 || _m < 1 || _m > 12 || _y < 0 || _d > daysInMonth()) ? False : True);
}

/*
 * Routine   : isLeap()
 * Purpose   : check if this is a isLeap year
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if LEAP, False if NOT
 *
 */

FlgT Date::isLeap()
{
   IntT yr = _y + 1900;

   return (( yr%4 == 0 && yr%100 != 0 || yr%400 == 0 ) ? True : False);
}

/*
 * Routine   : weekDay()
 * Purpose   : return the number of the weekday
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : number of the weekday (monday = 0)
 *
 */

IntT Date::weekDay()
{
   return IntT((LngT(*this) - 1) % 7);
}

/*
 * Routine   : dayNum()
 * Purpose   : calculate the number of the day this year
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : day number
 *
 */

IntT Date::dayNum()
{
   IntT days = monthToDays(_m);

	if(_m>2)
	{
      days -= isLeap() ? 1:2;
	}

	return days + _d;
}

/*
 * Routine   : operator <
 * Purpose   : 'compare' operator (less than)
 * ------------------------------------------------------------------------
 * Parameters: other date object
 * Return    : 1 if less than, 0 if not
 *
 */

IntT Date::operator < (DateR d)
{
	return (compare(d) < 0);
}

/*
 * Routine   : operator >
 * Purpose   : 'compare' operator (greather than)
 * ------------------------------------------------------------------------
 * Parameters: other date object
 * Return    : 1 if greather than, 0 if not
 *
 */

IntT Date::operator > (DateR d)
{
	return (compare(d) > 0);
}

/*
 * Routine   : operator ==
 * Purpose   : 'compare' operator (equal)
 * ------------------------------------------------------------------------
 * Parameters: other date object
 * Return    : 1 if equal, 0 if not
 *
 */

IntT Date::operator == (DateR d)
{
	return (compare(d) == 0);
}

/*
 * Routine   : operator !=
 * Purpose   : 'compare' operator (not equal to)
 * ------------------------------------------------------------------------
 * Parameters: other date object
 * Return    : 1 if different, 0 if not
 *
 */

IntT Date::operator != (DateR d)
{
	return compare(d);
}

/*
 * Routine   : operator >=
 * Purpose   : 'compare' operator (greather or equal to)
 * ------------------------------------------------------------------------
 * Parameters: other date object
 * Return    : 1 if greather or equal, 0 if not
 *
 */

IntT Date::operator >= (DateR d)
{
	return (compare(d) >= 0);
}

/*
 * Routine   : operator <=
 * Purpose   : 'compare' operator (less than or equal)
 * ------------------------------------------------------------------------
 * Parameters: other date object
 * Return    : 1 if less than or equal to, 0 if not
 *
 */

IntT Date::operator <= (DateR d)
{
	return (compare(d) <= 0);
}

/*
 * Routine   : operator LngT()
 * Purpose   : return the day number since 1 jan 0001
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : number of days
 *
 */

Date::operator LngT()
{
   return yearsToDays(_y+1899) + dayNum();
}

/*
 * Routine   : operator -
 * Purpose   : substract two dates
 * ------------------------------------------------------------------------
 * Parameters: other date object
 * Return    : difference between these two dates
 *
 */

IntT Date::operator - (DateR d)
{
   return IntT(LngT(*this) - LngT(d));
}

/*
 * Routine   : daysInMonth()
 * Purpose   : return the number of days in a month
 * ------------------------------------------------------------------------
 * Parameters: month number (0 for the current month)
 * Return    : number of days in a month
 *
 */

IntT Date::daysInMonth(IntT m)
{
   LCL IntT months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

   if(!m)
      m = _m;

   return (months[m-1] + (isLeap() && m==2));
}

/*
 * Routine   : daysInYear()
 * Purpose   : return the number of days in a year
 * ------------------------------------------------------------------------
 * Parameters: year number (0 for the current year)
 * Return    : number of days
 *
 */

IntT Date::daysInYear(IntT y)
{
   if(!y)
      y = _y;

   return 365 + Date(1,1,y).isLeap();
}

/*
 * Routine   : pack()
 * Purpose   : pack the current date in a WORD (scrambled!)
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : scrambled word
 *
 */

WrdT Date::pack()
{
   WrdT yr = _y - 90;
   WrdT ret_val  = 0;

   ret_val |= (((_d & (1     ))     ) << 13);
   ret_val |= (((_d & (1 << 1)) >> 1) <<  4);
   ret_val |= (((_d & (1 << 2)) >> 2) <<  9);
   ret_val |= (((_d & (1 << 3)) >> 3) <<  7);
   ret_val |= (((_d & (1 << 4)) >> 4) <<  1);

   ret_val |= (((_m & (1     ))     ) << 14);
   ret_val |= (((_m & (1 << 1)) >> 1) <<  5);
   ret_val |= (((_m & (1 << 2)) >> 2)      );
   ret_val |= (((_m & (1 << 3)) >> 3) << 11);

   ret_val |= (((yr & (1     ))     ) <<  8);
   ret_val |= (((yr & (1 << 1)) >> 1) <<  3);
   ret_val |= (((yr & (1 << 2)) >> 2) << 10);
   ret_val |= (((yr & (1 << 3)) >> 3) <<  6);

   return(ret_val);
}


/*
 * Routine   : unpack()
 * Purpose   : unpack the specified date
 * ------------------------------------------------------------------------
 * Parameters: packed date
 * Return    : none
 *
 */

void Date::unpack(WrdT packed)
{
   _d = _m = _y = 0;

   BytT yr = 0;

   _d |= (((packed & (1 << 13)) >> 13)     );
   _d |= (((packed & (1 <<  4)) >>  4) << 1);
   _d |= (((packed & (1 <<  9)) >>  9) << 2);
   _d |= (((packed & (1 <<  7)) >>  7) << 3);
   _d |= (((packed & (1 <<  1)) >>  1) << 4);

   _m |= (((packed & (1 << 14)) >> 14)     );
   _m |= (((packed & (1 <<  5)) >>  5) << 1);
   _m |= (((packed & (1      ))      ) << 2);
   _m |= (((packed & (1 << 11)) >> 11) << 3);

   yr |= (((packed & (1 <<  8)) >>  8)     );
   yr |= (((packed & (1 <<  3)) >>  3) << 1);
   yr |= (((packed & (1 << 10)) >> 10) << 2);
   yr |= (((packed & (1 <<  6)) >>  6) << 3);

   _y = yr + 90;
}
