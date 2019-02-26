#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <stdlib.h>
  #include <stdio.h>
  #include <ctype.h>
  #include <string.h>
#endif

#include <tslib.hpp>

const int ALLOC_STEP = 16;

// -------------------------------------------------------------------------
// Aanpassing door Alain Schellinck                         | nieuwe functie
// -------------------------------------------------------------------------

word
countPexAttrib(char *str)
{
    word ret_val = 0;

    String attribCodes("\1\2\3\4\5\6\7\x11\x12\x13\x14\x15\x16\x17");

    while(*str)
      if(strchr(attribCodes , *str++) != NULL)
         ret_val++;

    return ret_val;
}

// -------------------------------------------------------------------------

static int near
ALLOC_SIZE(int len)
{
   return ((len + ALLOC_STEP - 1) / ALLOC_STEP * ALLOC_STEP) + 1;
}

String::String()
{
   size = ALLOC_SIZE(0);
   s    = (char *) malloc(size);
   *s   = 0;
}

String::~String()
{
   free(s);
}

void
String::clear()
{
   adjustSize(0);

   *s   = '\0';
}

String::String(const String& str)
{
   size = ALLOC_SIZE(str.len());

   s    = (char *)malloc(size);

   strcpy(s,str.s);
}

String::String(const char *str)
{
   size = ALLOC_SIZE(strlen(str));

   s    = (char *)malloc(size);

   strcpy(s,str);
}

String::String(const char c,int len)
{
   size = ALLOC_SIZE(len);

   s    = (char *)malloc(size);

   memset(s,c,len);

   s[len] = '\0';
}

void
String::operator=(const char *x)
{
   int l = strlen(x);

   adjustSize(l);

   memcpy(s,x,l+1);
}

void
String::operator=(char x)
{
   adjustSize(1);

   s[0] = x;
   s[1] = '\0';
}

void
String::operator=(const String& str)
{
   *this = str.s;
}

void
String::operator=(int i)
{
   *this = long(i);
}

void
String::operator=(long i)
{
   *this = form("%ld",i);
}

void
String::operator+=(const String& str)
{
   *this += str.s;
}

void
String::operator+=(const char *str)
{
   int l  = len() ,
       l1 = strlen(str);

   adjustSize(l+l1);

   memcpy(s+l,str,l1+1);
}

void
String::operator+=(char c)
{
   int l = len();

   adjustSize(l+1);

   s[l++] = c;
   s[l]   = '\0';
}

String&
String::operator<<(char c)
{
   *this += c;

   return *this;
}

String&
String::operator<<(const char *str)
{
   *this += str;

   return *this;
}

String&
String::operator<<(const String& str)
{
   *this += str;

   return *this;
}

String&
String::operator<<(int i)
{
   *this << long(i);

   return *this;
}

String&
String::operator<<(long i)
{
   char s[10];
   sprintf(s,"%ld",i);

   *this += s;

   return *this;
}

String
operator+(const String& s1,const String& s2)
{
   String tmp(s1);

   tmp += s2;

   return tmp;
}

String
operator+(const String& s1,const char *s2)
{
   String tmp(s1);

   tmp += s2;

   return tmp;
}

String
operator+(const char *s1,const String& s2)
{
   String tmp(s1);

   tmp += s2;

   return tmp;
}

int
String::operator==(const char *str) const
{
   return !stricmp(s,str);
}

int
String::operator==(const String& str) const
{
   return !stricmp(s,str);
}

int
String::operator!=(const char *str) const
{
   return stricmp(s,str);
}

int
String::operator!=(const String& str) const
{
   return stricmp(s,str);
}

int
String::len() const
{
   return strlen(s);
}

word
String::crc() const
{
   return ::crc(s);
}

word
String::upcrc() const
{
   return ::upcrc(s);
}

void
String::upperCase()
{
   strupr(s);
}

void
String::lowerCase()
{
   for( int i = len() - 1 ; i >= 0 ; i--) s[i] = char(tolower(s[i]));
}

void
String::del(int pos,int l)
{
   String tmp;

   for(int i=0;i<pos;i++) tmp << s[i];

   if(l != 0)
      for(i=pos+l;i<len();i++) tmp << s[i];

   *this=tmp;
}

void
String::insert(int pos,const char *str)
{
   String tmp;

   for(int i=0;i<pos;i++)     tmp << (char)((i<len()) ? s[i] : ' ');
   for(i=0;i<strlen(str);i++) tmp << str[i];
   for(i=pos;i<len();i++)     tmp << s[i];

   *this = tmp;
}

String
String::sub(int pos,int l) const
{
   String tmp;

   if(l == 0) l = len() - pos;

   for(int i=pos ; i<pos+l ; i++) tmp << s[i];

   return tmp;
}

char
String::lastChar() const
{
   return (*s != '\0') ? s[len()-1] : '\0';
}

void
String::delFirst(char c)
{
   if(firstChar() != '\0' && (c == '\0' || c == firstChar()))
   {
      String str(&s[1]);

      (*this) = str;
   }
}

void
String::delLast(char c)
{
   int l = len();

   if(l > 0 && (c == '\0' || c == lastChar()))
   {
      s[--l] = '\0';

      adjustSize(l);
   }
}


void
String::ltrim()
{
   char *orig_s = s ,
        *p      = s ;

   for( ; *p && strchr(" \n\t",*p); p++) {}

   if(s != p)
   {
      while(*p) *s++ = *p++;

      *s = *p;
   }

   s = orig_s;

   adjustSize();
}

void
String::rtrim()
{
   char *p = s + len();

   while(p-- > s)
   {
      if(strchr(" \n\t",*p))
      {
         *p = '\0';
      }
      else
      {
         break;
      }
   }

   adjustSize();
}

void
String::trim()
{
   ltrim();
   rtrim();
}

void
String::adjustSize()
{
   adjustSize(len());
}

void
String::adjustSize(int len)
{
   int newsize = ALLOC_SIZE(len);

   if(newsize != size)
   {
      size = newsize;
      s    = (char *) realloc(s,size);
   }
}

void
String::mixedCase()
{
   char last = ' ';
   char *p = s;

   for(; *p ; p++)
   {
      *p = char((isalpha(last)) ? tolower(*p) : toupper(*p));

      last = *p;
   }
}

// -------------------------------------------------------------------------
// Aanpassing door Alain Schellinck                | aanpassingen a/d funtie
// -------------------------------------------------------------------------

void
String::justify(justification j , int l)
{
   int pexLen = len() - countPexAttrib(s);

   if(l == 0) l = pexLen;

   trim();

   if(l > pexLen)
      switch(j)
      {
         case left    :
         {
            (*this) = s + String(' ',l - pexLen);
         }
         break;

         case right   :
         {
            (*this) = String(' ',l - pexLen) + s;
         }
         break;

         case centered:
         {
            (*this) = String(' ',(l-pexLen)/2) + s + String(' ',l - pexLen - (l-pexLen)/2);
         }
         break;
      }
}

int
String::replace(const char *search,const char *replace,bool sens,int maxcount)
{
   int count = 0;
   int search_len = strlen(search);
   int l = len() - search_len + 1;
   String result;

   for(int i = 0; i < l; i++)
   {
      if(   ( sens && !memcmp (&s[i] , search , search_len))
         || (!sens && !memicmp(&s[i] , search , search_len))
        )
      {
         result << replace;
         i += search_len - 1;

         if(++count >= maxcount && maxcount) break;
      }
      else
      {
         result << s[i];
      }
   }

   result << &s[i];

   (*this) = result;

   return count;
}


int
String::find(const char *search,bool sens) const
{
   int search_len = strlen(search);
   int l = len() - search_len + 1;

   for(int i = 0; i < l; i++)
   {
      if(   ( sens && !memcmp (&s[i] , search , search_len))
         || (!sens && !memicmp(&s[i] , search , search_len))
        )
      {
         return i;
      }
   }

   return -1;
}
