
/* ------------------------------------------------------------
 * Filename ............... Th_Ini.Cpp
 *
 * General Purpose ........ Inifile handling
 * ------------------------------------------------------------
 * First date ............. 24 sep 1993
 *
 * First in version ....... 2.00
 *
 * Written by ............. Alain Schellinck
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

#ifndef PEX
#  include <string.h>
#  include <stdlib.h>
#endif

#define Uses_IniFile
#include <TheHouse.Hpp>

/*--] Code [-------------------------------------] IniVar [----------------*/

/*
 * Name       : IniVar()
 * Purpose    : constructor with parameters
 * -------------------------------------------------------------------------
 * Parameters : varname, value, comment
 * Return     : None
 */

IniVar::IniVar(ChrP var, ChrP val, ChrP com)
{
   name    = var;
   value   = val;
   comment = com;
}

/*
 * Name       : set()
 * Purpose    : set a new value
 * -------------------------------------------------------------------------
 * Parameters : value
 * Return     : None
 */

void IniVar::set(ChrP val)
{
   value = val;
}

/*
 * Name       : get()
 * Purpose    : get the value
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : value
 */

String IniVar::get()
{
   return(value);
}

/*
 * Name       : addComment()
 * Purpose    : add comment to the variable
 * -------------------------------------------------------------------------
 * Parameters : the comment
 * Return     : None
 */

void IniVar::addComment(ChrP com)
{
   comment = com;
}

/*
 * Name       : write()
 * Purpose    : write the variable to the specified file
 * -------------------------------------------------------------------------
 * Parameters : file handle
 * Return     : None
 */

void IniVar::write(File&  out)
{
   if(comment.len() != 0)
      out << "\n" << ChrP(comment);

   out << ChrP(name) << " = " << ChrP(value) << "\n";
}

/*
 * Name       : getName()
 * Purpose    : get the name of the variable
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : name
 */

String IniVar::getName()
{
   return(name);
}

/*--] Code [-------------------------------------] IniDomain [-------------*/

/*
 * Name       : IniDomain()
 * Purpose    : constructor with parameters
 * -------------------------------------------------------------------------
 * Parameters : domain name, comment
 * Return     : None
 */

IniDomain::IniDomain(ChrP dom, ChrP com)
{
   name    = dom;
   comment = com;

   vars = new LinkedList;
}

/*
 * Name       : ~IniDomain()
 * Purpose    : The destructor
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

IniDomain::~IniDomain()
{
   delete vars;
}

/*
 * Name       : set()
 * Purpose    : set the value for a variable (add if it doesn't exist)
 * -------------------------------------------------------------------------
 * Parameters : var, value
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT IniDomain::set(ChrP var, ChrP val)
{
   FlgT ret_val = FALSE;

   IniVarP entry = search(var);
   if(entry != NULL)
   {
      entry->set(val);
      ret_val = TRUE;
   }
   else
   {
      entry = new IniVarT(var, val);
      if(entry != NULL)
         ret_val = vars->add(entry);
   }

   return(ret_val);
}

/*
 * Name       : get()
 * Purpose    : get a the value for a specified variable
 * -------------------------------------------------------------------------
 * Parameters : var
 * Return     : value, or empty string if not found
 */

String IniDomain::get(ChrP var)
{
   IniVarP entry = search(var);
   if(entry != NULL)
      return(entry->get());
   else
      return("");
}

/*
 * Name       : search()
 * Purpose    : search for a specified variable
 * -------------------------------------------------------------------------
 * Parameters : varname
 * Return     : pointer to variable or NULL if not found
 */

IniVarP IniDomain::search(ChrP var)
{
   IniVarP ret_val = NULL;

   for(vars->top(); !vars->pastEnd() && ret_val == NULL; vars->next())
   {
      IniVarP entry = IniVarP(vars->get());
      if(entry->getName() == var)
         ret_val = entry;
   }

   return(ret_val);
}

/*
 * Name       : addComment()
 * Purpose    : add comment to the domain
 * -------------------------------------------------------------------------
 * Parameters : the comment
 * Return     : None
 */

void IniDomain::addComment(ChrP com)
{
   comment = com;
}

/*
 * Name       : write()
 * Purpose    : write the complete domain to a file
 * -------------------------------------------------------------------------
 * Parameters : file handler
 * Return     : None
 */

void IniDomain::write(File& out)
{
   if(comment.len() != 0)
      out << "\n" << ChrP(comment);

   if(name.len() != 0)
      out << "[" << ChrP(name) << "]\n";

   for(vars->top(); !vars->pastEnd(); vars->next())
   {
      IniVarP entry = IniVarP(vars->get());
      entry->write(out);
   }

   out << "\n\n";
}

/*
 * Name       : getName()
 * Purpose    : get the name of the domain
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : name
 */

String IniDomain::getName()
{
   return(name);
}

/*--] Code [-------------------------------------] IniFile [---------------*/

/*
 * Name       : IniFile()
 * Purpose    : constructor with parameters
 * -------------------------------------------------------------------------
 * Parameters : name of the inifile
 * Return     : None
 */

IniFile::IniFile(ChrP fn)
{
   fname = fn;
   domains = new LinkedList;

   inMemory = load();
   changed  = FALSE;
}

/*
 * Name       : ~IniFile()
 * Purpose    : the destructor
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : None
 */

IniFile::~IniFile()
{
   if(changed)
      save();

   delete domains;
}

/*
 * Name       : set()
 * Purpose    : set the specified variable. make domain and/or var
 *              if required.
 * -------------------------------------------------------------------------
 * Parameters : domain, var, value
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT IniFile::set(ChrP dom, ChrP var, ChrP val)
{
   FlgT ret_val = TRUE;

   IniDomainP entry = search(dom);
   if(entry == NULL)
   {
      entry = new IniDomainT(dom);
      ret_val = domains->add(entry);
   }

   if(ret_val == TRUE)
      if(strlen(var) != 0)
         if(entry->set(var, val) == TRUE)
            changed = TRUE;
         else
            ret_val = FALSE;

   return(ret_val);
}

/*
 * Name       : set()
 * Purpose    : set the specified variable. make domain and/or var
 *              if required.
 * -------------------------------------------------------------------------
 * Parameters : domain, var, value
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT IniFile::set(ChrP dom, ChrP var, UShrtT val)
{
   return(set(dom, var, form("%u", val)));
}

/*
 * Name       : set()
 * Purpose    : set the specified variable. make domain and/or var
 *              if required.
 * -------------------------------------------------------------------------
 * Parameters : domain, var, value
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT IniFile::set(ChrP dom, ChrP var, LngT val)
{
   return(set(dom, var, form("%ld", val)));
}

/*
 * Name       : set()
 * Purpose    : set the specified variable. make domain and/or var
 *              if required.
 * -------------------------------------------------------------------------
 * Parameters : domain, var, value
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT IniFile::set(ChrP dom, ChrP var, FlgT val)
{
   return(set(dom, var, ((val == TRUE) ? "YES" : "NO")));
}

/*
 * Name       : get()
 * Purpose    : get the value of a specified variable
 * -------------------------------------------------------------------------
 * Parameters : domain, var, string class to fill
 * Return     : TRUE if FOUND, FALSE if NOT
 */

FlgT IniFile::get(ChrP dom, ChrP var, String& val)
{
   FlgT ret_val = FALSE;

   IniDomainP entry = search(dom);
   if(entry != NULL)
   {
      val = entry->get(var);
      ret_val = TRUE;
   }

   return(ret_val);
}

/*
 * Name       : get()
 * Purpose    : get the value of a specified variable
 * -------------------------------------------------------------------------
 * Parameters : domain, var, UShrtT variable to fill
 * Return     : TRUE if FOUND, FALSE if NOT
 */

FlgT IniFile::get(ChrP dom, ChrP var, UShrtT& val)
{
   FlgT ret_val = FALSE;
   String v("");

   if(get(dom, var, v) == TRUE)
   {
      val = UShrtT(atoi(v));
      ret_val = TRUE;
   }

   return(ret_val);
}

/*
 * Name       : get()
 * Purpose    : get the value of a specified variable
 * -------------------------------------------------------------------------
 * Parameters : domain, var, LngT variable to fill
 * Return     : TRUE if FOUND, FALSE if NOT
 */

FlgT IniFile::get(ChrP dom, ChrP var, LngT& val)
{
   FlgT ret_val = FALSE;
   String v("");

   if(get(dom, var, v) == TRUE)
   {
      val = atol(v);
      ret_val = TRUE;
   }

   return(ret_val);
}

/*
 * Name       : get()
 * Purpose    : get the value of a specified variable
 * -------------------------------------------------------------------------
 * Parameters : domain, var, FlgT variable to fill
 * Return     : TRUE if FOUND, FALSE if NOT
 */

FlgT IniFile::get(ChrP dom, ChrP var, FlgT& val)
{
   FlgT ret_val = FALSE;
   String v("");

   if(get(dom, var, v) == TRUE)
   {
      if( (v == "YES") || (v == "Y") || (v == "ON")  )
      {
         val = TRUE;
         ret_val = TRUE;
      }

      if( (v == "NO")  || (v == "N") || (v == "OFF") )
      {
         val = FALSE;
         ret_val = TRUE;
      }
   }

   return(ret_val);
}

/*
 * Name       : search()
 * Purpose    : search for a specified domain
 * -------------------------------------------------------------------------
 * Parameters : domain name
 * Return     : pointer to the domain or NULL if not found
 */

IniDomainP IniFile::search(ChrP dom)
{
   IniDomainP ret_val = NULL;

   for(domains->top(); !domains->pastEnd() && ret_val == NULL; domains->next())
   {
      IniDomainP entry = IniDomainP(domains->get());
      if(entry->getName() == dom)
         ret_val = entry;
   }

   return(ret_val);
}

/*
 * Name       : write()
 * Purpose    : write the complete list to a specified file
 * -------------------------------------------------------------------------
 * Parameters : file handle
 * Return     : None
 */

void IniFile::write(File& out)
{
   for(domains->top(); !domains->pastEnd(); domains->next())
   {
      IniDomainP entry = IniDomainP(domains->get());
      entry->write(out);
   }
}

/*
 * Name       : load()
 * Purpose    : load the complete ini file
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT IniFile::load()
{
   FlgT ret_val = TRUE;
   File in;

   if(in.open(fname, fmode_read|fmode_text, defaultBufferSize) == TRUE)
   {
      String domain ("");
      String comment("");

      while(!in.eof() && ret_val == TRUE)
      {
         String line("");
         in >> line;

         switch(line[0])
         {
            case '[' :
            {
               domain = line;
               domain.trim();
               domain.delFirst('[');
               domain.delLast(']');

               if(set(domain) == TRUE)
               {
                  if(comment.len() != 0)
                  {
                     IniDomainP entry = search(domain);
                     entry->addComment(comment);
                     comment = "";
                  }
               }
               else
                  ret_val = FALSE;
            }
            break;

            case ';' :
            {
               comment << line;
            }
            break;

            default  :
            {
               String var(line);
               String val(line);

               ShrtT pos = var.find("=");
               if(pos != -1)
               {
                  var.del(pos, 0);
                  var.trim();

                  if(pos < line.len())
                  {
                     val.del(0, pos + 1);
                     val.trim();
                  }
                  else
                     val = "";

                  if(set(domain, var, val) == TRUE)
                  {
                     if(comment.len() != 0)
                     {
                        IniDomainP dm = search(domain);
                        IniVarP vr = dm->search(var);
                        vr->addComment(comment);
                        comment = "";
                     }
                  }
                  else
                     ret_val = FALSE;
               }
            }
            break;
         }
      }

      in.close();
   }

   return(ret_val);
}

/*
 * Name       : save()
 * Purpose    : save the complete ini file
 * -------------------------------------------------------------------------
 * Parameters : None
 * Return     : TRUE if OK, FALSE if NOT
 */

FlgT IniFile::save()
{
   FlgT ret_val = TRUE;
   File out;

   if(out.open(fname, fmode_text|fmode_create, defaultBufferSize) == TRUE)
   {
      write(out);
      changed = FALSE;
   }
   else
      ret_val = FALSE;

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
