#ifdef PEX
  #define NO_KEY_DEF
  #include <pb_sdk.h>
#else
  #include <ctype.h>
#endif

#include <tswin.hpp>

static bool volatile inhelp = FALSE;

#ifdef __OS2__
   KEY tsw_bgetch();
#else
   extern "C"
      {
         KEY tsw_bgetch();
      }
#endif

KEY
KeyBoard::uget()
{
   KEY x = get();

   return (x < 0x100) ? KEY(toupper(x)) : x;
}

KEY
KeyBoard::get()
{
   if(inhelp) return tsw_bgetch();

   if(pushed)
   {
      last   = pushed;

      pushed = 0;
   }
   else
   {
      last = tsw_bgetch();
   }

   if(last == KEY_F1 && help != NULLFUNC && !inhelp)
   {
      inhelp = TRUE;

      (*help)();

      inhelp = FALSE;
   }

   return last;
}

bool
KeyBoard::hit()
{
   return (check_key() || pushed) ? TRUE : FALSE;
}

void
KeyBoard::clear()
{
   word far *hp = (word far *) 0x41A;
   word far *tp = (word far *) 0x41C;

   *tp = *hp;

//   pushed = 0;
}

char
KeyBoard::choice(const char *s)
{
   for(;;)
   {
      int  i = 0;
      char c = char(uget());

      while(s[i])
         if(c == s[i++])
            return c;
   }
}

bool
KeyBoard::ask()
{
   return (bool)!!(choice("YNJ")-'N');
}

