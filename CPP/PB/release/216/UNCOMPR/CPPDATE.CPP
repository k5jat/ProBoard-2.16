#include <pb_sdk.h>
#include "cppdate.hpp"

struct fdate
   {
      word day   : 5;   /* Days */
      word month : 4;   /* Months */
      word year  : 7;   /* Year */
   };


/////////////////////////////////////////////////////////////////////////////
// Calculates the number of days BEFORE the first of a given month         //
/////////////////////////////////////////////////////////////////////////////

static int
months_to_days(int month)
{
   return int (l_div((l_mul(month , 3057) - 3007) , 100));
}


/////////////////////////////////////////////////////////////////////////////
// Calculates the number of days in a given number of years since 1-1-0001 //
/////////////////////////////////////////////////////////////////////////////

static long
years_to_days(int yr)
{
   return l_mul(yr , 365L) + yr / 4 - yr / 100 + yr / 400;
}


Date::Date()
{
   _d = _m = _y = 0;
}

Date::Date(int d,int m,int y)
{
   _d = d;
   _m = m;
   _y = y;
}

Date::Date(long val)
{
   if(val < 0)
   {
      today();
   }
   else
   {
      (*this) = val;
   }
}

char *Date::weekdays[7]   = { "Mon","Tue","Wed","Thu","Fri","Sat","Sun" };
char *Date::s_weekdays[7] = { "Mo","Tu","We","Th","Fr","Sa","Su" };
char *Date::l_weekdays[7] = { "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday" };
char *Date::months[12]    = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
char *Date::l_months[12]  = { "January","February","March","April","May","June","July","August","September","October","November","December" };

void
Date::today()
{
   time_t tm_val = time(NULL);

   struct tm *tm_struct = localtime(&tm_val);

   _d = tm_struct->tm_mday;
   _m = tm_struct->tm_mon + 1;
   _y = tm_struct->tm_year;
}

void
Date::set(int day,int month,int year)
{
   _d = day;
   _m = month;
   _y = (year>1900) ? (year-1900) : year;
}

char&
Date::operator[](int i)
{
   switch(i)
   {
      case 0 : return _d;
      case 1 : return _m;
      default: return _y;
   }
}

int
Date::compare(Date& dt) const
{
   if(_y - dt._y) return (_y - dt._y);
   if(_m - dt._m) return (_m - dt._m);
   if(_d - dt._d) return (_d - dt._d);

   return 0;
}

int
Date::operator<(Date& d) const
{
   return (compare(d) < 0);
}

int
Date::operator>(Date& d) const
{
   return (compare(d) > 0);
}

int
Date::operator==(Date& d) const
{
   return (compare(d) == 0);
}

int
Date::operator!=(Date& d) const
{
   return compare(d);
}

int
Date::operator>=(Date& d) const
{
   return (compare(d) >= 0);
}

int
Date::operator<=(Date& d) const
{
   return (compare(d) <= 0);
}

bool
Date::ok() const
{
   return !(_d<1 || _m<1 || _m>12 || _y<0 || _d>daysInMonth());
}

bool
Date::leap() const
{
   int yr = _y + 1900;

   return ( yr%4 == 0 && yr%100 != 0 || yr%400 == 0 );
}

int
Date::weekDay() const
{
   return int( l_mod((long(*this)-1) , 7));
}

int
Date::weekNum() const
{
   Date nyd(1,1,_y);

   int w = (dayNum() + nyd.weekDay() - 1)/7 + (nyd.weekDay() < 4);

   return ((w < 1) ? weeksInYear(_y-1) : w);
}

int
Date::weeksInYear(int y) const
{
   if(!y) y = _y;

   return Date(31,12,y).weekNum();
}


Date&
Date::operator=(long julian)
{
   int n;                /* compute inverse of years_to_days() */

   for ( n = int(l_div(l_mul(julian , 400L) , 146097L)); years_to_days(n) < julian;) n++;

   _y = n - 1900;

   n = int(julian - years_to_days(n-1));

   if( n > 59 )
   {
      n += 2;

      if( leap() )  n -= (n > 62) ? 1 : 2;
   }

   _m = int(l_div((l_mul(n , 100) + 3007) , 3057));

   _d = n - months_to_days(_m);

   return *this;
}

int
Date::dayNum() const
{
   int days = months_to_days(_m);

   if(_m>2)
   {
      days -= leap() ? 1:2;
   }

   return days + _d;
}

Date::operator long() const
{
   return years_to_days(_y+1899) + dayNum();
}

int
Date::operator-(Date &d) const
{
   return int(long(*this) - long(d));
}

void
Date::fileDate(word x)
{
    fdate fd;

    *((word *)(&fd)) = x;

    _d = fd.day;
    _m = fd.month;
    _y = fd.year+80;
}


int
Date::daysInMonth(int m) const
{
   static int months[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

   if(!m) m = _m;

   return months[m-1] + (leap() && m==2);
}

int
Date::daysInYear(int y) const
{
   if(!y) y = _y;

   return 365 + Date(1,1,y).leap();
}
