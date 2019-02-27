
/* ------------------------------------------------------------
 * Filename ............... Setup.Cpp
 *
 * General Purpose ........ Setup class
 * ------------------------------------------------------------
 * First date ............. 12-19-94 07:30pm
 *
 * First in version ....... 2.00
 *
 * Written by ............. Alain Schellinck
 *
 * Copyright .............. Nephilim software development
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

#include "main.hpp"
#pragma  hdrstop

/*-------------------------------------------------------------------------*/

#include "setup.hpp"

/*-------------------------------------------------------------------------*/

SetupP TSetup = NULL;

/*-------------------------------------------------------------------------*/

struct Keywords
{
   ChrP  text;
   IntT  id;
};

/*-------------------------------------------------------------------------*/

enum domNums        // FROM domains must come first!!!
{
   dmFrom = From     ,
   dmTo              ,
   dmSubject         ,
   dmMsgNumber       ,
   dmArea            ,
   dmPrivateMsg      ,
   dmInsertMode      ,
   dmRawKeys         ,
   dmAbbrevs         ,
   dmEscForMenu      ,
   dmMenuBar         ,
   dmTextWindow      ,

   dmGeneral         ,
   dmShareware       ,
   dmImportFiles     ,
};

/*-------------------------------------------------------------------------*/

Keywords domains[] =
{
   {  "[FROM]"          ,  From          },
   {  "[TO]"            ,  To            },
   {  "[SUBJECT]"       ,  Subj          },
   {  "[MSGNUMBER]"     ,  Msg           },
   {  "[AREA]"          ,  Area          },
   {  "[PRIVATEMSG]"    ,  PvtMsg        },
   {  "[INSERTMODE]"    ,  Insert        },
   {  "[RAWKEYS]"       ,  Raw           },
   {  "[ABBREVS]"       ,  Abbrevs       },
   {  "[ESCFORMENU]"    ,  EscForMenu    },
   {  "[TEXTWINDOW]"    ,  TextWindow    },
   {  "[MENUBAR]"       ,  MenuBar       },
   {  "[GENERAL]"       ,  dmGeneral     },
   {  "[SHAREWARE]"     ,  dmShareware   },
   {  "[IMPORTFILES]"   ,  dmImportFiles },
   {  NULL              ,  0             }
};

/*-------------------------------------------------------------------------*/

enum varNums
{
   vlRow          = 0,  vlColumn          ,
   vlForeGround      ,  vlBackGround      ,
   vlLength          ,  vlStopBanner      ,
   vlRegisterKey     ,  vlFastScroll      ,
   vlQForeGround     ,  vlQBackGround     ,
   vlInsertMode      ,  vlAddTag          ,
   vlFullQuote       ,  vlBannerText      ,
   vlPrinter         ,  vlLogging         ,

   vlAltF1           ,  vlAltF2           ,
   vlAltF3           ,  vlAltF4           ,
   vlAltF5           ,  vlAltF6           ,
   vlAltF7           ,  vlAltF8           ,
   vlAltF9           ,  vlAltF10          ,
};

/*-------------------------------------------------------------------------*/

Keywords variables[] =
{
   {  "ADDTAG"          ,  vlAddTag          },
   {  "LOGGING"         ,  vlLogging         },
   {  "ROW"             ,  vlRow             },
   {  "COLUMN"          ,  vlColumn          },
   {  "FOREGROUND"      ,  vlForeGround      },
   {  "BACKGROUND"      ,  vlBackGround      },
   {  "QFOREGROUND"     ,  vlQForeGround     },
   {  "QBACKGROUND"     ,  vlQBackGround     },
   {  "LENGTH"          ,  vlLength          },
   {  "STOPBANNER"      ,  vlStopBanner      },
   {  "REGISTERKEY"     ,  vlRegisterKey     },
   {  "FASTSCROLL"      ,  vlFastScroll      },
   {  "INSERTMODE"      ,  vlInsertMode      },
   {  "ALLOWFULLQUOTE"  ,  vlFullQuote       },
   {  "BANNERTEXT"      ,  vlBannerText      },
   {  "PRINTER"         ,  vlPrinter         },
   {  "ALT-F1"          ,  vlAltF1           },
   {  "ALT-F2"          ,  vlAltF2           },
   {  "ALT-F3"          ,  vlAltF3           },
   {  "ALT-F4"          ,  vlAltF4           },
   {  "ALT-F5"          ,  vlAltF5           },
   {  "ALT-F6"          ,  vlAltF6           },
   {  "ALT-F7"          ,  vlAltF7           },
   {  "ALT-F8"          ,  vlAltF8           },
   {  "ALT-F9"          ,  vlAltF9           },
   {  "ALT-F10"         ,  vlAltF10          },
   {  NULL              ,  0                 }
};

/*--] Code [-------------------------------------] FormField [-------------*/

/*
 * Routine   : FormField()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

FormField::FormField()
{
   x      = 0;
   y      = 0;
   attr   = 0;
   length = 0;
   text   = NULL;
}

/*
 * Routine   : ~FormField()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

FormField::~FormField()
{
   if(text != NULL)
      free(text);
}

/*--] Code [-------------------------------------] Setup [-----------------*/

/*
 * Routine   : Setup()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Setup::Setup()
{
   fields = new FormFieldP[EndOfFields];
   for(IntT cnt = 0; cnt < EndOfFields; cnt++)
      fields[cnt] = new FormFieldT;

   for(cnt = 0; cnt < 10; cnt++)
      importFiles[cnt] = NULL;

   language = new LanguageT;

   // --- setup for PEX files ------------------------------------------
#ifdef PEX
   sysop    = ChrP(Config ->sysopname);
   bbs      = ChrP(Config ->bbsname  );
   user     = ChrP(CurUser->name     );
   userLang = ChrP(CurUser->language );
   bpsRate  = BaudRate;
   maxText  = WrdT( min( (MemAvail() - 60000L) / 2, 32767L));
#endif

   // --- setup for EXE files ------------------------------------------
#ifndef PEX
   sysop    = author;
   bbs      = home_bbs;
   user     = author;
   userLang = "ENGLISH";
   bpsRate  = 0L;
   maxText  = WrdT(32767L);
#endif

   isRegistered = False;
   maxRows      = WrdT(maxText / 16);
   isSysop      = (strcmp(user, sysop) == 0 ? True : False);
   stopBanner   = False;
   fastScroll   = False;
   insertMode   = True;
   fullQuote    = False;
   addTag       = True;
   bannerText   = NULL;
   ptrPort      = NULL;
   logging      = False;
   rawMode      = False;
   abbrevMode   = False;
}

/*
 * Routine   : ~Setup()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Setup::~Setup()
{
   if(fields != NULL)
   {
      for(IntT cnt = 0; cnt < EndOfFields; cnt++)
         delete fields[cnt];

      delete fields;
   }

   delete language;

   for(IntT cnt = 0; cnt < 10; cnt++)
      if(importFiles[cnt] != NULL)
         free(importFiles[cnt]);

   if(bannerText != NULL) free(bannerText);
   if(ptrPort    != NULL) free(ptrPort   );
}

/*
 * Routine   : findDomain()
 * Purpose   : find the specified keyword in the domain array
 * ------------------------------------------------------------------------
 * Parameters: keyword
 * Return    : domain number (or -1 if not found)
 *
 */

IntT Setup::findDomain(ChrP k)
{
   IntT ret_val = -1;
   for(IntT cnt = 0; domains[cnt].text != NULL && ret_val == -1; cnt++)
      if(stricmp(domains[cnt].text, k) == 0)
         ret_val = domains[cnt].id;

   return(ret_val);
}

/*
 * Routine   : findVar()
 * Purpose   : find the specified keyword in the variables array
 * ------------------------------------------------------------------------
 * Parameters: keyword
 * Return    : variable number (or -1 if not found)
 *
 */

IntT Setup::findVar(ChrP k)
{
   IntT ret_val = -1;
   for(IntT cnt = 0; variables[cnt].text != NULL && ret_val == -1; cnt++)
   {
      if(stricmp(variables[cnt].text, k) == 0)
         ret_val = variables[cnt].id;
   }

   return(ret_val);
}

/*
 * Routine   : yesNo()
 * Purpose   : Return TRUE for YES/ON or FALSE for NO/OFF
 * ------------------------------------------------------------------------
 * Parameters: value to compare, default
 * Return    : True or False
 *
 */

FlgT Setup::yesNo(ChrP yn, FlgT def)
{
   FlgT ret_val = def;
   if(stricmp(yn, "YES") == 0 || (stricmp(yn, "ON") == 0))
      ret_val = True;

   if(stricmp(yn, "OFF") == 0 || (stricmp(yn, "NO") == 0))
      ret_val = False;

   return(ret_val);
}

/*
 * Routine   : colorCode()
 * Purpose   : get the value for the specified color
 * ------------------------------------------------------------------------
 * Parameters: color
 * Return    : color value
 *
 */

BytT Setup::colorCode(ChrP attr)
{
   BytT ret_val = 0;
   ChrP colors[] = { "BLACK", "BLUE", "GREEN", "CYAN", "RED", "MAGENTA", "BROWN", "WHITE" };
   if(attr[0] == '+')
   {
      ret_val = 8;
      attr++;
   }

   FlgT found = False;
   for(IntT cnt = 0; cnt < 8 && found == False; cnt++)
   {
      if(stricmp(colors[cnt], attr) == 0)
      {
         ret_val += cnt;
         found = True;
      }
   }

   return(ret_val);
}

/*
 * Routine   : checkRegKey()
 * Purpose   : check the registration key
 * ------------------------------------------------------------------------
 * Parameters: The registration key
 * Return    : None
 *
 */

void Setup::checkRegKey(ChrP regKey)
{
   IntT merged[64];
   ChrT keyStr[64];

   strcpy(keyStr, protectKey);

   IntT sysopLen = strlen(sysop);
   IntT bbsLen   = strlen(bbs  );

   IntT keyLen   = strlen(keyStr           );
   IntT checkLen = max(sysopLen,bbsLen     );

   ShrtT kpos = 0;

   for(ShrtT cnt = 0; cnt < checkLen; cnt++)
   {
      BytT x = BytT(toupper(keyStr[kpos++]));
      if(kpos >= keyLen)
         kpos = 0;

      if(cnt < sysopLen)
         x ^= BytT(toupper(sysop[cnt]));

      if(cnt < bbsLen)
         x ^= BytT(toupper(bbs[cnt]));

      merged[cnt] = x;
   }

   ChrT key[132];
   key[0] = '\0';
   for(cnt = 0; cnt < checkLen; cnt++)
   {
      ChrT buffer[3];
      sprintf(buffer,"%X", merged[cnt]);
      strcat(key, buffer);
   }

   if(strcmp(key, regKey) == 0)
      isRegistered = True;
}

/*
 * Routine   : readMsgInfo()
 * Purpose   : read the message info structure
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Setup::readMsgInfo()
{
   ChrT buffer[80];
   FILE *handle = fopen("MSGINF", "rt");
   if(handle != NULL)
   {
      BytT counter = 1;
      while(!feof(handle) && counter < 7)
      {
         fgets(buffer, 79, handle);
         IntT len = strlen(buffer);
         for(IntT cnt = 0; cnt < len; cnt++)
         {
            if(buffer[cnt] == '_')
               buffer[cnt] = ' ';

            if(buffer[cnt] == '\n')
               buffer[cnt] = '\0';
         }

         switch(counter)
         {
            case 1:  fields[From  ]->text = strdup(buffer);  break;
            case 2:  fields[To    ]->text = strdup(buffer);  break;
            case 3:  fields[Subj  ]->text = strdup(buffer);  break;
            case 4:  fields[Msg   ]->text = strdup(buffer);  break;
            case 5:  fields[Area  ]->text = strdup(buffer);  break;
            case 6:
            {
               if(buffer[0] == 'Y')
                  fields[PvtMsg]->text = strdup(language->get(YesText));
               else
                  fields[PvtMsg]->text = strdup(language->get(NoText ));
            }
            break;
         }
         counter++;
      }

      fclose(handle);
   }
}

/*
 * Routine   : readConfig()
 * Purpose   : read the configuration file
 * ------------------------------------------------------------------------
 * Parameters: path, projectname
 * Return    : True if there was an ERROR, False if NOT
 *
 */

FlgT Setup::read(ChrP path, ChrP proj)
{
   ChrT buffer[256];

   language->read(path, userLang);

   FlgT errors = False;
   sprintf(buffer,"%s%s.INI", path, proj);

   FILE *handle = fopen(buffer, "rt");
   if(handle != NULL)
   {
      IntT domain   = -1;
      IntT variable = -1;

      while(!feof(handle))
      {
         if(fgets(buffer, 255, handle) == NULL)
            break;

         IntT length = strlen(buffer) - 1;
         while(isspace(buffer[length]))
            length--;

         buffer[length + 1] = '\0';


         if(buffer[0] != ';' && length > 1)
         {
            if(buffer[0] == '[')
            {
               domain = findDomain(buffer);
               if(domain == -1)
               {
                  errors = True;
                  printf("\nError: Unknown domain name: %s\n", buffer);
               }
            }
            else
            {
               ChrP varName  = buffer;
               ChrP varValue = NULL;

               IntT cnt = 0;
               while(!isspace(buffer[cnt]) && buffer[cnt] != '\0')
                  cnt++;                           // skip to the next space

               buffer[cnt++] = '\0';

               variable = findVar(varName);
               if(variable != -1)
               {
                  while(buffer[cnt] != '=' && buffer[cnt] != '\0')
                     cnt++;                           // skip to the equal sign

                  cnt++;

                  while(buffer[cnt] == ' ' && buffer[cnt] != '\0')
                     cnt++;                           // skip the spaces

                  varValue  = buffer + cnt;
                  if(domain != -1)
                  {
                     switch(domain)
                     {
                        // --- [GENERAL] ------------------------------------
                        case dmGeneral    :
                        {
                           switch(variable)
                           {
                              case vlStopBanner :  stopBanner = yesNo(varValue, False);   break;
                              case vlFastScroll :  fastScroll = yesNo(varValue, True );   break;
                              case vlInsertMode :  insertMode = yesNo(varValue, True );   break;
                              case vlFullQuote  :  fullQuote  = yesNo(varValue, True );   break;
                              case vlAddTag     :  addTag     = yesNo(varValue, True );   break;
                              case vlLogging    :  logging    = yesNo(varValue, False);   break;
                              case vlPrinter    :  ptrPort    = strdup(varValue      );   break;

                              case vlBannerText :
                              {
#ifdef PEX
                                 ChrT buffer[128];
                                 ParseStringVars(varValue, buffer, 127);
                                 bannerText = strdup(buffer);
#else
                                 bannerText = strdup(varValue);
#endif
                              }
                              break;
                           }
                        }
                        break;

                        // --- [SHAREWARE] ----------------------------------
                        case dmShareware  :
                        {
                           switch(variable)
                           {
                            case vlRegisterKey:  checkRegKey(varValue);                   break;
                           }
                        }
                        break;

                        // --- [IMPORTFILES] --------------------------------
                        case dmImportFiles   :
                        {
                           switch(variable)
                           {
                              case  vlAltF1  :  importFiles[0] = strdup(varValue); break;
                              case  vlAltF2  :  importFiles[1] = strdup(varValue); break;
                              case  vlAltF3  :  importFiles[2] = strdup(varValue); break;
                              case  vlAltF4  :  importFiles[3] = strdup(varValue); break;
                              case  vlAltF5  :  importFiles[4] = strdup(varValue); break;
                              case  vlAltF6  :  importFiles[5] = strdup(varValue); break;
                              case  vlAltF7  :  importFiles[6] = strdup(varValue); break;
                              case  vlAltF8  :  importFiles[7] = strdup(varValue); break;
                              case  vlAltF9  :  importFiles[8] = strdup(varValue); break;
                              case  vlAltF10 :  importFiles[9] = strdup(varValue); break;
                           }
                        }
                        break;

                        // --- FORM DOMAINS ---------------------------------
                        default           :
                        {
                           if(domain < EndOfFields)
                           {
                              FormFieldP field = fields[domain];
                              if(field != NULL)
                              {
                                 switch(variable)
                                 {
                                    case vlRow        :  field->y       = atoi(varValue);                                        break;
                                    case vlColumn     :  field->x       = atoi(varValue);                                        break;
                                    case vlLength     :  field->length  = atoi(varValue);                                        break;
                                    case vlForeGround :  field->attr    = (field->attr   & 0xF0) | (colorCode(varValue)     );   break;
                                    case vlBackGround :  field->attr    = (field->attr   & 0x0F) | (colorCode(varValue) << 4);   break;
                                    case vlQForeGround:  field->length  = (field->length & 0xF0) | (colorCode(varValue)     );   break;
                                    case vlQBackGround:  field->length  = (field->length & 0x0F) | (colorCode(varValue) << 4);   break;
                                 }
                              }
                           }
                        }
                        break;
                     }
                  }
               }
               else
               {
                  errors = True;
                  printf("\nError: Unknown variable name: %s\n", buffer);
               }
            }

         }
      }

      fclose(handle);
   }

   readMsgInfo();
   fields[EscForMenu]->text = strdup(language->get(PressEscape));

   if(!isRegistered)
   {
      addTag     = True;
      stopBanner = False;
      rawMode    = False;
   }
   else
   {
      rawMode = (bpsRate > 0);
   }


   return(errors);
}

/*---------------------------] END OF THE CODE [---------------------------*/
