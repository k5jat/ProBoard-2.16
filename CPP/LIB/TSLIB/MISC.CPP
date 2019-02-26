#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <string.h>
#endif

#include <tslib.hpp>

void
append_backspace(char *s)
{
    strip_linefeed(s);
    strip_trailing(s);

    if(s[strlen(s)-1] != '\\') strcat(s,"\\");
}

void
strip_linefeed(char *s)
{
    if(s[strlen(s)-1] == '\n') s[strlen(s)-1] = '\0';
}

void
pas2c(char *s)
{
    int i = byte(s[0]);

    memmove(s,s+1,i);

    s[i] = '\0';
}

void
c2pas(char *s)
{
    int i = strlen(s);

    memmove(s+1,s,i);

    *s = byte(i);
}

void
strip_leading(char *s)
{
    char *p;

    for(p = s ; *p ; p++)
        if(*p != ' ' && *p != '\t' && *p != '\n') break;

    if(s != p)
    {
        while(*p) *s++ = *p++;
        *s = '\0';
    }
}

void
strip_trailing(char *s)
{
    if(*s == '\0') return;

    char *p = s + strlen(s);

    for(--p ; p >= s ; p--)
    {
        if(*p == ' ' || *p == '\n' || *p == '\t')
        {
            *p = '\0';
        }
        else
        {
            break;
        }
    }
}

void
strip_all(char *s)
{
    strip_leading(s);
    strip_trailing(s);
}


#ifndef __OS2__
void
sleep_clock()
{
    dword ticks = clockticks();

    while(!clockdiff(ticks)) {}

    ticks = clockticks();

    while(!clockdiff(ticks)) {}
}
#endif
