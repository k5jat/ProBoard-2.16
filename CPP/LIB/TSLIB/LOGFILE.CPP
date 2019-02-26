#ifdef PEX
  #include <pb_sdk.h>
#endif

#include <tslib.hpp>

LogFile::LogFile()
{
   logStyle = LOGSTYLE_NORMAL;
}

bool
LogFile::open(const char *fn , byte style)
{
   name = fn;
   logStyle = style;

   return TRUE;
}
