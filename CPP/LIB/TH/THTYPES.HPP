
/* ------------------------------------------------------------
 * Filename ............... ThTypes.Hpp
 *
 * General Purpose ........ TheHouse type declarations
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

/*-------------------------------------------------------------------------*/

#define   CDC   cdecl
#define   PAS   pascal
#define   NEAR  near
#define   FAR   far

#undef NULL
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#    define   NULL  (0)
#else
#    define   NULL  (0L)
#endif

/*-------------------------------------------------------------------------*/

#ifndef Uses_TurboVision
#  undef min
#  define min(a,b)    (((a) < (b)) ? (a) : (b))

#  undef max
#  define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif                                 // ifndef Uses_TurboVision

/*--] Portability and shorthand notation [---------------------------------*/

#define  CON      const
#define  CTYP     PAS
#define  GBL      extern
#define  LCL      static
#define  REG      register
#define  SIZ(c)   sizeof(c)
#define  TYP      typedef
#define  UNS      unsigned
#define  VOID     void
#define  VOL      volatile
#define  NOARG

#undef FALSE
#define  FALSE (0)

#undef TRUE
#define  TRUE  (1)

#define _ClassType(name)   typedef class name   name##T; \
                           typedef class name  *name##P; \
                           typedef class name **name##I; \
                           typedef class name  &name##R;

#define _StructType(name)  typedef struct name   name##T; \
                           typedef struct name  *name##P; \
                           typedef struct name **name##I; \
                           typedef struct name  &name##R;

/*--] standard object, pointer and indirect types [------------------------*/

//                Object    Pointer     Indirect
TYP   VOID                  *AnyP,      **AnyI;     //  anonymous void pointer
TYP   VOID FAR              *AnyFP;                 //  ... far-pointer
TYP   char        ChrT,     *ChrP,      **ChrI;     //  8-bit char
TYP   ChrT FAR              *ChrFP;                 //  ... far-pointer
TYP   ChrT                  *StrP,      **StrI;     //  NulTerm string
TYP   UNS char    BytT,     *BytP,      **BytI;     //  8-bit unsigned byte
TYP   BytT FAR              *BytFP;                 //  ... far-pointer
TYP   BytT        FlgT,     *FlgP,      **FlgI;     //  8-bit Boolean flag
TYP   short       ShrtT,    *ShrtP,     **ShrtI;
TYP   UNS short   UShrtT,   *UShrtP,    **UShrtI;
TYP   int         IntT,     *IntP,      **IntI;     //  16-bit signed int
TYP   IntT FAR              *IntFP;                 //  ... far-pointer
TYP   UNS int     WrdT,     *WrdP,      **WrdI;     //  16-bit unsigned word
TYP   WrdT FAR              *WrdFP;                 //  ... far-pointer
TYP   long        LngT,     *LngP,      **LngI;     //  32-bit signed long
TYP   LngT FAR              *LngFP;                 //  ... far-pointer
TYP   UNS long    DwdT,     *DwdP,      **DwdI;     //  32-bit unsigned dword
TYP   DwdT FAR              *DwdFP;                 //  ... far-pointer
TYP   float       FltT,     *FltP,      **FltI;     //  32-bit real
TYP   double      DblT,     *DblP,      **DblI;     //  64-bit real
TYP   WrdT        LenT,     *LenP,      **LenI;     //  alias for ANSI size_t
TYP   LngT        PosT,     *PosP,      **PosI;     //  alias for ANSI fpos_t
TYP   WrdT        NdxT,     *NdxP,      **NdxI;     //  unsigned array-indices
TYP   IntT        ArgT;                             //  bytes/chars as ints

/*---------------------------] END OF THE CODE [---------------------------*/

