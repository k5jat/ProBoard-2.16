#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <string.h>
#endif

#include <tslib.hpp>

FileName::FileName(const char *p,const char *f,const char *e) : String(p)
{
   if(f) (*this) << f;
   if(e) (*this) << e;
}

void
FileName::operator()(const char *p)
{
   clear();

   if(p) (*this) << p;
}

void
FileName::operator()(const char *p,const char *f)
{
   clear();
   if(p) (*this) << p;
   if(f) (*this) << f;
}

void
FileName::operator()(const char *p,const char *f,const char *e)
{
   clear();
   if(p) (*this) << p;
   if(f) (*this) << f;
   if(e) (*this) << e;
}

void
FileName::stripPath()
{
   int l=len();
   for(int i=l-1;i>=0;i--)
   {
      if(s[i]=='\\' || s[i]=='/' || s[i]==':') break;
   }

   if(i >= 0) del(0,i+1);
}

void
FileName::appendBS()
{
   int l=len();

   if(s[l-1]!='\\') (*this) << '\\';
}

void
FileName::stripExt()
{
   int l=len();

   for(int i=l-1 ; i>=0 ; i--)
   {
      if(s[i] == '\\' || s[i] == ':')
         break;

      if(s[i] == '.')
      {
         del(i,l-i);
         break;
      }
   }
}

void
FileName::stripName()
{
   int l=len();

   for(int i=l-1;i>=0;i--)
      if(s[i]=='\\' || s[i]=='/' || s[i]==':')
      {
         del(i+1,l-i-1);
         break;
      }
}

void
FileName::changeExt(const char *ext)
{
   stripExt();

   (*this) << '.' << ext;
}

void
FileName::changePath(const char *p)
{
   stripPath();

   FileName tmp = *this;

   *this = p;
   appendBS();

   *this << tmp;
}

void
FileName::operator=(const char *s)
{
   String::operator=(s);
}

void
FileName::operator=(const FileName& fn)
{
   String::operator=((String)fn);
}

void
FileName::operator=(char c)
{
   String::operator=(c);
}

void
FileName::operator=(int i)
{
   String::operator=(i);
}

void
FileName::operator=(long l)
{
   String::operator=(l);
}

void
FileName::fit(int size)
{
    if(len() > size)
    {
        String p(s);
        int pos = p.find("\\");
        p.insert(pos + 1, "...\\");

        pos += 4;
        while(p.len() > size)
        {
            for(int i = 1; p[pos + i] != '\\' && p[pos + i] != 0; i++);

            if(p[pos + i] == 0)
                break;

            if(p[pos + i] == '\\')
                p.del(pos + 1, i);
        }

        (*this) = p;
    }
}
