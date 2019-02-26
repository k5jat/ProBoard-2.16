
/* ------------------------------------------------------------
 * Filename ............... ThClass.Hpp
 *
 * General Purpose ........ TheHouse classes
 * ------------------------------------------------------------
 * First date ............. 14 sep 1993
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

/*--] Classes [----------------------------------] Object [----------------*/

#ifndef Used_Object
#  define Used_Object

   _ClassType(Object)
   class Object
   {
   public:
      virtual  ~Object           (                                      )
               {
               }
   };

#endif

/*--] Classes [----------------------------------] LinkedList [------------*/

#ifdef Uses_LinkedList
#  ifndef Used_LinkedList
#     define Used_LinkedList

      _ClassType(LListEntry)
      class LListEntry
      {
         friend class LinkedList;

      protected:
         LListEntryP prev;
         LListEntryP next;
         ObjectP     rec ;

      public:
                  LListEntry     (  ObjectP r                           );
                  ~LListEntry    (                                      );
      };

      _ClassType(LinkedList)
      class LinkedList
      {
      protected:
         LListEntryP _first;
         LListEntryP _last;
         LListEntryP _cur;
         ShrtT       _curNum;
         ShrtT       _listLen;
         FlgT        _pastEnd;

      public:
                  LinkedList     (                                      );
                  ~LinkedList    (                                      );

         void     release        (                                      );
         FlgT     add            (  ObjectP r                           );
         void     remove         (                                      );
         ShrtT    count          (                                      );
         ObjectP  get            (  ShrtT recno = -1                    );
         FlgT     pos            (  ShrtT recno                         );
         void     top            (                                      );
         void     bottom         (                                      );
         FlgT     next           (                                      );
         FlgT     prev           (                                      );
         FlgT     onTop          (                                      );
         FlgT     onBottom       (                                      );
         FlgT     pastEnd        (                                      );
         void     sort           (                                      );

         virtual  ShrtT    compare  (  ObjectP e1, ObjectP e2           )
                  {
                     return(0);
                  }
      };

#  endif
#endif

/*--] Classes [----------------------------------] MultiRecIO [------------*/

#ifdef Uses_MultiRecIO
#  ifndef Used_MultiRecIO
#     define Used_MultiRecIO

      _ClassType(MultiRecIO)
      class MultiRecIO : public File
      {
      protected:
         FileName fname;

      public:
         enum idleMode
         {
            idleOpen,
            idleClose,
            idleRead,
            idleWrite,
            idleSeek
         };

      public:
                  MultiRecIO     (                                      );
                  ~MultiRecIO    (                                      );

         void     setFname       (  ChrP path, ChrP name                );
         FileName getFname       (                                      );
         FlgT     open           (  IntT fmode,
                                    IntT bufSize = defaultBufferSize    );
         void     close          (                                      );
         FlgT     read           (  ShrtT p = -1                        );
         FlgT     write          (  ShrtT p = -1                        );
         FlgT     pos            (  ShrtT p                             );
         ShrtT    count          (                                      );

         virtual  FlgT     readRec  (                                   )
                  {
                     return(FALSE);
                  }

         virtual  FlgT     writeRec (                                   )
                  {
                     return(FALSE);
                  }

         virtual  ShrtT    recLen   (                                   )
                  {
                     return(0);
                  }

         virtual  void     idle     (  idleMode mode                    )
                  {
                  }
      };

#  endif
#endif

/*--] Classes [----------------------------------] FormTxt [---------------*/

#ifdef Uses_FormTxt
#  ifndef Used_FormTxt
#     define Used_FormTxt

      _ClassType(FormTxt)
      class FormTxt : public File
      {
      private:
         String   headerText;
         ShrtT    numLines;
         ShrtT    curLine;
         ShrtT    pageNum;
         ShrtT    headerSize;
         FlgT     headerDisabled;

      public:
                  FormTxt        (                                      );
                  FormTxt        (  ChrP name,
                                    WrdT mode = fmode_read | fmode_text,
                                    WrdT bufsize = defaultBufferSize    );

         void     setSize        (  ShrtT lines = 62                    );
         void     setHeader      (  ChrP text                           );
         void     addHeader      (  ChrP text                           );
         FlgT     onTop          (                                      );
         void     disableHeader  (                                      );
         void     newPage        (                                      );
         void     formFeed       (                                      );
         ShrtT    getPageNum     (                                      );
         void     header         (                                      );
         void     footer         (                                      );
         ShrtT    countLines     (  ChrP text                           );
         FormTxt& operator <<    (  ChrP text                           );
         FormTxt& operator <<    (  String& text                        )
                  {
                     return operator <<(ChrP(text));
                  }
      };

#  endif
#endif

/*--] Classes [----------------------------------] IniFile [---------------*/

#ifdef Uses_IniFile
#  ifndef Used_IniFile
#     define Used_IniFile

      _ClassType(IniVar)
      class IniVar : public Object
      {
      private:
         String   name   ;
         String   value  ;
         String   comment;

      public:
                  IniVar         (  ChrP  var, ChrP val ="", ChrP com ="");

         virtual  ~IniVar        (                                       )
                  {
                  }

         void     set            (  ChrP val                             );
         String   get            (                                       );
         void     addComment     (  ChrP com                             );
         void     write          (  File& out                            );
         String   getName        (                                       );
      };

      _ClassType(IniDomain)
      class IniDomain   : public Object
      {
      private:
         String      name;
         String      comment;
         LinkedList  *vars;

      public:
                  IniDomain      (  ChrP dm, ChrP com = ""               );
         virtual  ~IniDomain     (                                       );

         FlgT     set            (  ChrP var, ChrP val                   );
         String   get            (  ChrP var                             );
         IniVarP  search         (  ChrP var                             );
         void     addComment     (  ChrP com                             );
         void     write          (  File& out                            );
         String   getName        (                                       );
      };

      _ClassType(IniFile)
      class IniFile
      {
      private:
         FileName    fname   ;
         LinkedList  *domains;

         FlgT        inMemory;
         FlgT        changed ;

      public:
                  IniFile        (  ChrP fn                              );
                  ~IniFile       (                                       );

         FlgT     set            (  ChrP dm, ChrP vr = "", ChrP vl = ""  );
         FlgT     set            (  ChrP dom, ChrP var, UShrtT val       );
         FlgT     set            (  ChrP dom, ChrP var, LngT   val       );
         FlgT     set            (  ChrP dom, ChrP var, FlgT   val       );

         FlgT     get            (  ChrP dom, ChrP var, String& val      );
         FlgT     get            (  ChrP dom, ChrP var, UShrtT& val      );
         FlgT     get            (  ChrP dom, ChrP var, LngT  & val      );
         FlgT     get            (  ChrP dom, ChrP var, FlgT  & val      );

         IniDomainP  search      (  ChrP dom                             );
         void     addComment     (  ChrP com                             );
         void     write          (  File& out                            );
         FlgT     load           (                                       );
         FlgT     save           (                                       );
      };

#  endif
#endif

/*--] Classes [----------------------------------] ProKeng [---------------*/

#ifdef Uses_ProKeng
#  ifndef Used_ProKeng
#     define Used_ProKeng

      const UShrtT   regTableLen      = 32;  // a table has 32 bytes
      const UShrtT   regMaxTable      = 32;  // total number of tables = 32
      const UShrtT   randomLen        = 64 + (4 * regMaxTable) + 32 + 32 + 2 + 19 + 11;
      const UShrtT   regCheckLen      = randomLen * 2;

      const UShrtT   licAlphaTester   =  1;  // may run ALPHA versions
      const UShrtT   licBetaTester    =  2;  // may run BETA versions
      const UShrtT   licGammaTester   =  4;  // may run GAMMA versions
      const UShrtT   licCommercial    =  8;  // has commercial license
      const UShrtT   licDistribution  = 16;  // set for official distribution nodes
      const UShrtT   licRegisterSite  = 32;  // set for registration sites

      const UShrtT   erNoKeyFileFound =  2;  // never use '1' as an error code!
      const UShrtT   erWrongCrcCode   =  3;  // (because 1 == TRUE !)
      const UShrtT   erWrongCheckCode =  4;
      const UShrtT   erWrongName      =  5;
      const UShrtT   erWrongSystem    =  6;
      const UShrtT   erWrongLicense   =  7;


      _StructType(RndTables)
      struct RndTables
      {
         BytT  t1[regTableLen];           // first decoder table
         BytT  t2[regTableLen];           // second decoder table
      };


      _StructType(KeyRec)
      struct KeyRec
      {
         DwdT  checksum              ;    // checksum for the key
         ChrT  copyright[64         ];    // copyright message
         DwdT  keycodes [regMaxTable];    // reg. code for the tables
         ChrT  name     [32         ];    // name of the registered user
         ChrT  system   [32         ];    // name of the registered BBS
         WrdT  license               ;    // license flags
         ChrT  spare    [19         ];    // reserved for the future
         ChrT  created  [11         ];    // creation date (in ASCII)
         ChrT  random   [randomLen  ];    // to scramble the keyfile
      };


      _ClassType(ProKeng)
      class ProKeng
      {
      public:
         KeyRecT  data;
         FileName fname;

      public:
                  ProKeng        (                                      );

         void     setFname       (   ChrP path, ChrP fn                 );
         FlgT     read           (                                               );
         word     check          (   ChrP nm, ChrP sy, UShrtT li,
                                     BytP t1, BytP t2, UShrtT tn        );

         DwdT     calc           (   BytP t1, BytP t2                   );
         DwdT     crc            (                                      );
         void     scramble       (                                      );
      };


#     ifdef Uses_ProKengBuild

         _ClassType(ProKengRnd)
         class ProKengRnd
         {
         public:
             RndTablesT  rnd;             // random tables

             FileName    binaryFname ;    // name of the binary file
             File        binaryFile  ;    // file handle

             FileName    includeFname;    // name of the include file
             File        includeFile ;    // file handle

         public:
                     ProKengRnd     (  ChrP project                     );

             FlgT    exist          (                                   );
             void    fillRnd        (                                   );
             FlgT    create         (                                   );

             void    writeRnd       (   UShrtT Table                    );
             void    writeHpp       (   UShrtT table                    );
             void    writeHppHeader (                                   );
             void    writeHppFooter (                                   );

             FlgT    openTable      (                                   );
             void    readTable      (   UShrtT table                    );
         };

         _ClassType(ProKengBld)
         class ProKengBld : public ProKeng
         {
         public:
                     ProKengBld     (                                   );

             FlgT    create         (   ProKengRndP rnd                 );
             FlgT    write          (                                   );
         };

#     endif
#  endif
#endif

/*---------------------------] END OF THE CODE [---------------------------*/
