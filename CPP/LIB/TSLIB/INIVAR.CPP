#ifdef PEX
  #include <pb_sdk.h>
#else
  #include <stdio.h>
  #include <string.h>
#endif

#include <tslib.hpp>

static void near
SplitVarLine(char *line,char *varname,char *var)
{
   char *sp , *dp;

   bool eq_found = FALSE;
   bool in_quote = FALSE;

   var[0] = varname[0] = 0;

   strip_all(line);

   if(strlen(line) < 3 || line[0] == ';' || line[0] == '%') return;

   for(sp = line , dp = varname ; *sp ; sp++)
   {
      switch(*sp)
      {
         case '\"' : in_quote = !in_quote;
                     break;

         case '='  : if(!eq_found)
                     {
                        *dp = 0;
                        dp = var;
                        eq_found = TRUE;
                        break;
                     }

         case '\t' :
         case ' '  : if(!eq_found || dp == var && !in_quote) break;

         default   : *dp++ = *sp;
                     break;
      }
   }

   *dp = '\0';
}

bool
get_ini_var(const char *fn,const char *varname,char *var,int max)
{
   FileName fname(fn);
   fname.changeExt("INI");

   var[0] = 0;

   File f(fname,fmode_read | fmode_text);

   if(!f.opened()) return FALSE;

   char *str         = new char[256];
   char *tmp_varname = new char[256];
   char *tmp_var     = new char[256];

   while(f.readLine(str,255))
   {
      SplitVarLine(str,tmp_varname,tmp_var);

      if(!stricmp(varname,tmp_varname))
      {
         tmp_var[max-1] = 0;
         strcpy(var,tmp_var);

         break;
      }
   }

   delete [] str;
   delete [] tmp_var;
   delete [] tmp_varname;

   return !!var[0];
}

static void near
WriteVar(File &f,const char *varname,const char *var)
{
   char *tmp_str = new char[strlen(var) + 1];

   strcpy(tmp_str,var);

   strip_all(tmp_str);

   if(strlen(tmp_str) != strlen(var))
   {
      f << varname << " = \"" << var << "\"\n";
   }
   else
   {
      f << varname << " = " << var << '\n';
   }

   delete [] tmp_str;
}


bool
set_ini_var(const char *fn,const char *varname,const char *var)
{
   File fi,fo;
   bool replaced = FALSE;
   FileName ifname(fn);
   FileName ofname(fn);

   ifname.changeExt("INI");
   ofname.changeExt("I$I");

   fi.open(ifname,fmode_read | fmode_text);

   if(!fo.open(ofname,fmode_create | fmode_text)) return FALSE;

   char *str         = new char[256];
   char *tmp_varname = new char[256];
   char *tmp_var     = new char[256];

   while(fi.readLine(str,255))
   {
      SplitVarLine(str,tmp_varname,tmp_var);

      if(!stricmp(varname,tmp_varname))
      {
         if(var[0]) WriteVar(fo,tmp_varname,var);
         replaced = TRUE;
      }
      else
      {
         if(str[0]) fo << str << '\n';
      }
   }

   if(!replaced && var[0]) WriteVar(fo,varname,var);

   delete [] str;
   delete [] tmp_var;
   delete [] tmp_varname;

   fi.close();
   fo.close();

   unlink(ifname);
   rename(ofname,ifname);

   return TRUE;
}
