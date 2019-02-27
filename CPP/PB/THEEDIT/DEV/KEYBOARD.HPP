
/* ------------------------------------------------------------
 * Filename ............... Keyboard.Hpp
 *
 * General Purpose ........ Keyboard class
 * ------------------------------------------------------------
 * First date ............. 12-07-94 11:20am
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

#ifndef __KEYBOARD_HPP
#  define __KEYBOARD_HPP

/*-------------------------------------------------------------------------*/

#  ifndef __QUEUE_HPP
#     include "queue.hpp"
#  endif

/*-------------------------------------------------------------------------*/

#  ifndef PEX
#    define KEY_F1      0x3B00
#    define KEY_F2      0x3C00
#    define KEY_F3      0x3D00
#    define KEY_F4      0x3E00
#    define KEY_F5      0x3F00
#    define KEY_F6      0x4000
#    define KEY_F7      0x4100
#    define KEY_F8      0x4200
#    define KEY_F9      0x4300
#    define KEY_F10     0x4400
#    define KEY_SF1     0x5400
#    define KEY_SF2     0x5500
#    define KEY_SF3     0x5600
#    define KEY_SF4     0x5700
#    define KEY_SF5     0x5800
#    define KEY_SF6     0x5900
#    define KEY_SF7     0x5A00
#    define KEY_SF8     0x5B00
#    define KEY_SF9     0x5C00
#    define KEY_SF10    0x5D00
#    define KEY_CF1     0x5E00
#    define KEY_CF2     0x5F00
#    define KEY_CF3     0x6000
#    define KEY_CF4     0x6100
#    define KEY_CF5     0x6200
#    define KEY_CF6     0x6300
#    define KEY_CF7     0x6400
#    define KEY_CF8     0x6500
#    define KEY_CF9     0x6600
#    define KEY_CF10    0x6700
#    define KEY_AF1     0x6800
#    define KEY_AF2     0x6900
#    define KEY_AF3     0x6A00
#    define KEY_AF4     0x6B00
#    define KEY_AF5     0x6C00
#    define KEY_AF6     0x6D00
#    define KEY_AF7     0x6E00
#    define KEY_AF8     0x6F00
#    define KEY_AF9     0x7000
#    define KEY_AF10    0x7100
#    define KEY_ALT1    0x7800
#    define KEY_ALT2    0x7900
#    define KEY_ALT3    0x7A00
#    define KEY_ALT4    0x7B00
#    define KEY_ALT5    0x7C00
#    define KEY_ALT6    0x7D00
#    define KEY_ALT7    0x7E00
#    define KEY_ALT8    0x7F00
#    define KEY_ALT9    0x8000
#    define KEY_ALT0    0x8100
#    define KEY_ALTA    0x1E00
#    define KEY_ALTB    0x3000
#    define KEY_ALTC    0x2E00
#    define KEY_ALTD    0x2000
#    define KEY_ALTE    0x1200
#    define KEY_ALTF    0x2100
#    define KEY_ALTG    0x2200
#    define KEY_ALTH    0x2300
#    define KEY_ALTI    0x1700
#    define KEY_ALTJ    0x2400
#    define KEY_ALTK    0x2500
#    define KEY_ALTL    0x2600
#    define KEY_ALTM    0x3200
#    define KEY_ALTN    0x3100
#    define KEY_ALTO    0x1800
#    define KEY_ALTP    0x1900
#    define KEY_ALTQ    0x1000
#    define KEY_ALTR    0x1300
#    define KEY_ALTS    0x1F00
#    define KEY_ALTT    0x1400
#    define KEY_ALTU    0x1600
#    define KEY_ALTV    0x2F00
#    define KEY_ALTW    0x1100
#    define KEY_ALTX    0x2D00
#    define KEY_ALTY    0x1500
#    define KEY_ALTZ    0x2C00
#    define KEY_CTLA    0x0001
#    define KEY_CTLB    0x0002
#    define KEY_CTLC    0x0003
#    define KEY_CTLD    0x0004
#    define KEY_CTLE    0x0005
#    define KEY_CTLF    0x0006
#    define KEY_CTLG    0x0007
#    define KEY_CTLH    0x0008
#    define KEY_CTLI    0x0009
#    define KEY_CTLJ    0x000A
#    define KEY_CTLK    0x000B
#    define KEY_CTLL    0x000C
#    define KEY_CTLM    0x000D
#    define KEY_CTLN    0x000E
#    define KEY_CTLO    0x000F
#    define KEY_CTLP    0x0010
#    define KEY_CTLQ    0x0011
#    define KEY_CTLR    0x0012
#    define KEY_CTLS    0x0013
#    define KEY_CTLT    0x0014
#    define KEY_CTLU    0x0015
#    define KEY_CTLV    0x0016
#    define KEY_CTLW    0x0017
#    define KEY_CTLX    0x0018
#    define KEY_CTLY    0x0019
#    define KEY_CTLZ    0x001A
#    define KEY_DEL     0x5300
#    define KEY_INS     0x5200
#    define KEY_HOME    0x4700
#    define KEY_END     0x4F00
#    define KEY_PGUP    0x4900
#    define KEY_PGDN    0x5100
#    define KEY_UP      0x4800
#    define KEY_DN      0x5000
#    define KEY_LT      0x4B00
#    define KEY_RT      0x4D00
#    define KEY_ESC     0x001B
#    define KEY_ENT     0x000D
#    define KEY_RET     0x000D
#    define KEY_TAB     0x0009
#    define KEY_STAB    0x0F00
#    define KEY_CPGUP   0x8400
#    define KEY_CPGDN   0x7600
#  endif

/*-------------------------------------------------------------------------*/

#  define KEY_CTL_LT    0x7300
#  define KEY_CTL_RT    0x7400
#  define KEY_BACK      0x0008

/*--] Classes [----------------------------------] Keyboard [--------------*/

   _ClassType(Keyboard)
   class Keyboard
   {
   protected:
      Queue       local;
      Queue       remote;
      FlgT        fromLocal;

   protected:
      WrdT        scan           (  BytT wait                           );
      void        handle         (                                      );

   public:
                  Keyboard       (                                      );
                  ~Keyboard      (                                      );

      WrdT        peek           (                                      );
      WrdT        get            (                                      );
      FlgT        isEmpty        (                                      );
      void        purge          (                                      );
      FlgT        poke           (  WrdT key, FlgT inLocal              );
      FlgT        isLocalKey     (                                      )  {  return fromLocal; }
   };

/*-------------------------------------------------------------------------*/

   extern KeyboardP  TKeyboard;

/*-------------------------------------------------------------------------*/

   IntT        LocalKeyHandler   (  WrdT key                            );

/*-------------------------------------------------------------------------*/

#endif                                 // __KEYBOARD_HPP

/*---------------------------] END OF THE CODE [---------------------------*/
