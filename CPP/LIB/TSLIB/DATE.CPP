#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <ctype.h>
  #include <time.h>
  #include <dos.h>
  #include <string.h>
#endif

#include <tslib.hpp>

const long JULIAN_ADJUSTMENT = 1721425L;

struct ftime
   {
      word tsec  : 5;   /* Two second interval */
      word min   : 6;   /* Minutes */
      word hour  : 5;   /* Hours */
   };

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
#ifdef PEX
   return int (l_div((l_mul(month , 3057) - 3007) , 100));
#else
   return int ((long(month) * 3057 - 3007) / 100);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// Calculates the number of days in a given number of years since 1-1-0001 //
/////////////////////////////////////////////////////////////////////////////

static long
years_to_days(int yr)
{
#ifdef PEX
   return l_mul(yr , 365L) + yr / 4 - yr / 100 + yr / 400;
#else
   return yr * 365L + yr / 4 - yr / 100 + yr / 400;
#endif
}


Date::Date()
{
   _d = _m = _y = 0;
}

Date::Date(int d,int m,int y)
{
   _d = byte(d);
   _m = byte(m);
   _y = byte(y);
}

Date::Date(const Date& d)
{
   _d = d._d;
   _m = d._m;
   _y = d._y;
}

void
Date::operator=(const Date& d)
{
   _d = d._d;
   _m = d._m;
   _y = d._y;
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

   _d = byte(tm_struct->tm_mday);
   _m = byte(tm_struct->tm_mon + 1);
   _y = byte(tm_struct->tm_year);
}

void
Date::set( int day,
           int month,
           int year )
{
   _d  =  byte( day );
   _m  =  byte( month );



   //--------  
   // Y2K BUG
   //--------  

   _y  =  byte( ( year > 1900 ) 
                   ?  ( year - 1900 ) 
                   :  year );        
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
Date::compare(const Date& dt) const
{
   if(_y - dt._y) return (_y - dt._y);
   if(_m - dt._m) return (_m - dt._m);
   if(_d - dt._d) return (_d - dt._d);

   return 0;
}

int
Date::operator<(const Date& d) const
{
   return (compare(d) < 0);
}

int
Date::operator>(const Date& d) const
{
   return (compare(d) > 0);
}

int
Date::operator==(const Date& d) const
{
   return (compare(d) == 0);
}

int
Date::operator!=(const Date& d) const
{
   return compare(d);
}

int
Date::operator>=(const Date& d) const
{
   return (compare(d) >= 0);
}

int
Date::operator<=(const Date& d) const
{
   return (compare(d) <= 0);
}

bool
Date::ok() const
{
     return  bool( ! ( _d < 1   || 
                       _m < 1   || 
                       _m > 12  || 
                       _y < 0   || 
                       _d > daysInMonth() ) );
}

bool
Date::leap() const
{
   int yr = _y + 1900;

   return bool( yr%4 == 0 && yr%100 != 0 || yr%400 == 0 );
}

int
Date::weekDay() const
{
#ifdef PEX
   return int( l_mod((long(*this)-1) , 7));
#else
   return int((long(*this)-1) % 7);
#endif
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

#ifdef PEX
   for ( n = int(l_div(l_mul(julian , 400L) , 146097L)); years_to_days(n) < julian;) n++;
#else
   for ( n = int((julian * 400L) / 146097L); years_to_days(n) < julian;) n++;
#endif

   _y = byte(n - 1900);

   n = int(julian - years_to_days(n-1));

   if( n > 59 )
   {
      n += 2;

      if( leap() )  n -= (n > 62) ? 1 : 2;
   }

#ifdef PEX
   _m = byte(l_div((l_mul(n , 100) + 3007) , 3057));
#else
   _m = byte((long(n) * 100 + 3007) / 3057);
#endif

   _d = byte(n - months_to_days(_m));

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
Date::operator-(const Date &d) const
{
   return int(long(*this) - long(d));
}

void
Date::fileDate(word x)
{
    fdate fd;

    *((word *)(&fd)) = x;

    _d = byte(fd.day);
    _m = byte(fd.month);
    _y = byte(fd.year+80);
}


void Date::parse_key(String& key) const
{
     int wd = weekDay();


     if      ( key == "D"    )  key = form("%d",_d);
     else if ( key == "DD"   )  key = form("%02d",_d);
     else if ( key == "WW"   )  key = s_weekdays[wd];
     else if ( key == "WWW"  )  key = weekdays[wd];
     else if ( key == "WWWW" )  key = l_weekdays[wd];
     else if ( key == "M"    )  key = form("%d",_m);
     else if ( key == "MM"   )  key = form("%02d",_m);
     else if ( key == "MMM"  )  key = months[_m-1];
     else if ( key == "MMMM" )  key = l_months[_m-1];


     else if ( key == "YY"   )  key  =  form( "%02d", _y % 100    );       
     else if ( key == "CC"   )  key  =  form( "%02d", 19 + _y/100 );
   
     //------------------------------------------  
     // JDR - Y2K: Added to support 4-digit years
     //------------------------------------------  

     else if ( key == "YYYY" )
     {
          if ( _y == 0 )
          {
               key  =  form( "0000" );
          }
          else
          {
               key  =  form( "%04d", _y + 1900   );
          }
     }
}



String Date::format( const char *fmt ) const
{
     String result;
     String key;
     char   last = *fmt;


     for ( ;  *fmt;  fmt++ )
     {
          if ( last != *fmt )
          {
               parse_key( key );

               result << key;

               key = *fmt;
          }
          else
          {
               key << *fmt;
          }


          last = *fmt;
     }


     parse_key( key );

     result << key;

     return result;
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

int
Date::age(const Date& birthdate)
{
   int years = year() - birthdate.year();

   if((*this) < Date(birthdate.day(),birthdate.month(),year()))
      years--;

   return years;
}


Time::Time()
{
   _h = _m = _s = 0;
}

Time::Time(const Time& t)
{
   _h = t._h;
   _m = t._m;
   _s = t._s;
}

void
Time::operator=(const Time& t)
{
   _h = t._h;
   _m = t._m;
   _s = t._s;
}

void
Time::set(int h,int m,int s)
{
    _h = byte(h);
    _m = byte(m);
    _s = byte(s);
}

void
Time::now()
{
    time_t tm_val = time(NULL);

    struct tm *tm_struct = localtime(&tm_val);

    _h = byte(tm_struct->tm_hour);
    _m = byte(tm_struct->tm_min);
    _s = byte(tm_struct->tm_sec);
}

Time::operator long() const
{
#ifdef PEX
   return (l_mul(_h , 3600L)) + (l_mul(_m , 60L)) + long(_s);
#else
   return (long(_h) * 3600L) + (long(_m) * 60L) + long(_s);
#endif
}

void
Time::fileTime(word t)
{
    ftime ft;

    *((word *)(&ft)) = t;

    _h = byte(ft.hour);
    _m = byte(ft.min);
    _s = byte(ft.tsec*2);
}

long
Time::operator-(const Time& t) const
{
   return (long(*this) - long(t));
}

int
Time::operator<(const Time& t) const
{
    return ((*this - t) < 0);
}

int
Time::operator>(const Time& t) const
{
    return ((*this - t) > 0);
}

int
Time::operator==(const Time& t) const
{
    return ((*this - t) == 0);
}

int
Time::operator!=(const Time& t) const
{
    return ((*this - t) != 0);
}

int
Time::operator>=(const Time& t) const
{
    return ((*this - t) >= 0);
}

int
Time::operator<=(const Time& t) const
{
    return ((*this - t) <= 0);
}

char&
Time::operator[](int i)
{
   switch(i)
   {
      case 0 : return _h;
      case 1 : return _m;
      default: return _s;
   }
}

void
Time::operator+=(const Time& t)
{
#ifdef PEX
   (*this) = l_mod(long(*this) + long(t) , 86400L);
#else
   (*this) = (long(*this) + long(t)) % 86400L;
#endif
}

void
Time::operator-=(const Time& t)
{
#ifdef PEX
   (*this) = l_mod(long(*this) - long(t) + 86400L , 86400L);
#else
   (*this) = (long(*this) - long(t) + 86400L) % 86400L;
#endif
}

void
Time::operator=(long l)
{
#ifdef PEX
   _h = byte(l_div(l , 3600L));
   _m = byte(l_div(l_mod(l , 3600L) , 60L));
   _s = byte(l_mod(l , 60L));
#else
   _h = byte(l / 3600L);
   _m = byte((l % 3600L) / 60L);
   _s = byte(l % 60L);
#endif
}

void
Time::parse_key(String& key) const
{
   if(!strcmp(key,"H"))     key = form("%d",_h);
   else
   if(!strcmp(key, "HH"))   key = form("%02d",_h);
   else
   if(!strcmp(key,"h"))     key = form("%d",((_h+11) % 12) + 1);
   else
   if(!strcmp(key, "hh"))   key = form("%02d",((_h+11) % 12) + 1);
   else
   if(!strcmp(key, "MM"))   key = form("%02d",_m);
   else
   if(!strcmp(key, "SS"))   key = form("%02d",_s);
   else
   if(!strcmp(key, "PP"))   key = (_h >= 12) ? "PM":"AM";
   else
   if(!strcmp(key, "pp"))   key = (_h >= 12) ? "pm":"am";
   else
   if(!strcmp(key, "AA"))   key = (_h >= 12) ? "PM":"";
   else
   if(!strcmp(key, "aa"))   key = (_h >= 12) ? "pm":"";
   else
   if(!strcmp(key, "P"))    key = (_h >= 12) ? "P":"A";
   else
   if(!strcmp(key, "p"))    key = (_h >= 12) ? "p":"a";
   else
   if(!strcmp(key, "A"))    key = (_h >= 12) ? "P":"";
   else
   if(!strcmp(key, "a"))    key = (_h >= 12) ? "p":"";
}

String
Time::format(const char *fmt) const
{
   String result , key;
   char last = *fmt;

   for( ; *fmt ; fmt++)
   {
      if(last != *fmt)
      {
         parse_key(key);

         result << key;

         key = *fmt;
      }
      else
      {
         key << *fmt;
      }

      last = *fmt;
   }

   parse_key(key);

   result << key;

   return result;
}



//**************************************************************************
//
// Make sure a year value is ProBoard Y2K compliant (i.e., 1900 + 0-127)
//
//     Prototype:  int NormalizeYear( int iYearVal );
//
//    Parameters:  iYearVal ... Input year value to adjust
//
//       Returns:  The "normalized" year value that is Y2K compliant
//
//       Remarks:
//
// The year value that this routine returns is between 0 and 127, to
// indicate the years 1900 through 2027.  2027 will only be returned if the
// macro TWO_DIGIT_CROSSOVER is set to 28.  Right now, it's set to 10, which
// will result in a return value of 0 to 109, indicating 1900 - 2009.
//
// We use this macro to allow the programmer to customize the cutoff of two-
// digit year values to suit their application's needs.
// 
// This routine performs quite a number of checks on the year value.  If the
// year value is negative, it converts it to positive and goes from there.
//
// If the value is a two-digit value, it checks the crossover value to
// figure out what century it's in.  If it's a three-digit value, it
// considers it a freaky value and tries the best it can (it MOD's the value
// by 100, and treats the final date as a two-year value).
//
// We also check for typos here.  We only consider a simple typo of the year
// 3XXX.  If we find a year like this, we'll treat it as a 2XXX year.
// Anything else is treated verbatim as a legitimate 4-digit year.  Anything
// after 2000+TWO_DIGIT_CROSSOVER-1 wraps over to the 1900 century.
//
// -------------------------------------------------------------------------
//
//    Created on:  06/17/99 (Jeff Reeder)
// Last modified:  06/17/99 (Jeff Reeder)   Created
//
//**************************************************************************

int NormalizeYear( int iYearVal )
{
     //----------------------------------------------------------
     // First, check for really outrageous year values.  This
     // means years from 100..1899 and 2000+TWO_DIGIT_CROSSOVER..
     //----------------------------------------------------------

     if ( iYearVal < 0 )
     {
          //---------------------------------------
          // We somehow have a negative year value.
          // Convert it to a positive value.
          //---------------------------------------

          iYearVal *= -1;
     }


     //---------------------------------  
     // Check for outlandish year values
     //---------------------------------  

     if ( iYearVal  >= 3000  &&
          iYearVal  <  3000 + TWO_DIGIT_CROSSOVER )
     {
          //-------------------------------------------------------  
          // User must have hit 3 when they meant 2 - subtract 1000
          //-------------------------------------------------------  

          iYearVal -= 1000;
     }


     if ( 
          iYearVal  >= 100  &&
          (
            iYearVal  <   1900  ||
            iYearVal  >=  2000 + TWO_DIGIT_CROSSOVER 
          )
        )
     {
          //------------------------------------------------
          // The year is more than two digits, and it's not
          // in the range of 1900 - 2000+TWO_DIGIT_CROSSOVER
          //
          // This is a weird input ... do our best by
          // converting it to a two-digit year and proceed.
          //------------------------------------------------

          iYearVal %= 100;
     }
     

     //--------------------------------------------  
     // Now we can proceed with year normaliziation
     //--------------------------------------------  

     if ( iYearVal < 100 )
     {
          //-----------  
          // Short year
          //-----------  

          if ( iYearVal < TWO_DIGIT_CROSSOVER )
          {
               //--------------------------------------------  
               // Should be 2000...2000+TWO_DIGIT_CROSSOVER-1
               //--------------------------------------------  

               iYearVal += 100;
          }
          else
          {
               //-----------------------------------------------
               // This year must be in the 1900's - The value is
               // already in the proper value range.  Do nothing
               //-----------------------------------------------
          }
     }
     else 
     {
          //---------------------------------------------------  
          // The year must be 1900...2000+TWO_DIGIT_CROSSOVER-1
          //---------------------------------------------------  

          if ( iYearVal  <  2000 + TWO_DIGIT_CROSSOVER )
          {
               //-----------------------------  
               // Year is between 1900 .. 1999
               //-----------------------------  
               
               iYearVal -= 1900;
          }
     }    


     return iYearVal;
}

