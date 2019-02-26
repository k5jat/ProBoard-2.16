#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <stdio.h>
#endif

#include <tslib.hpp>

extern "C"
   {
      int f_first(void *,const char *,int);
      int f_next (void *);
   }

struct _find
   {
      char reserved[21];
      char attr;
      word time;
      word date;
      long size;
      char name[13];
   };

DirScan::DirScan()
{
   ok   = 0;
   data = new _find;
}

DirScan::DirScan(const char *wildcard,int attr)
{
   ok   = 0;
   data = new _find;

   first(wildcard,attr);
}

DirScan::DirScan(const char *wildcard)
{
   ok   = 0;
   data = new _find;

   first(wildcard);
}

DirScan::~DirScan()
{
   delete ((_find *)data);
}

int
DirScan::first(const char *wildcard,int attr)
{
   return (ok = !f_first(data,wildcard,attr));
}

int
DirScan::first(const char *wildcard)
{
   return (ok = !f_first(data,wildcard,0));
}

int
DirScan::next()
{
   return (ok = !f_next(data));
}

char *
DirScan::name() const
{
   return ((_find *)data)->name;
}

long
DirScan::size() const
{
   return ((_find *)data)->size;
}

int
DirScan::flags() const
{
   return ((_find *)data)->attr;
}

Date
DirScan::date() const
{
   Date d;

   d.fileDate(((_find *)data)->date);

   return d;
}

Time
DirScan::time() const
{
   Time t;

   t.fileTime(((_find *)data)->time);

   return t;
}
