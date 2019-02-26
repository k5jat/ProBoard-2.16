struct _stamp
{
  struct
  {
    unsigned int da : 5;
    unsigned int mo : 4;
    unsigned int yr : 7;
  } date;

  struct
  {
    unsigned int ss : 5;
    unsigned int mm : 6;
    unsigned int hh : 5;
  } time;
};


/* Structure for USER.BBS and LASTUSER.BBS */

/* NOTE:  This structure is semi-stable.  Although it is still compatible  *
 * with the old Opus 1.03 structure, don't expect it to stay that way      *
 * for long.  In a future version, Maximus will be using a dymaic-sized    *
 * user record, making it possible to make additions without breaking      *
 * preexisting software.  You can start to code for this now in your       *
 * software, as the usr.struct_len variable indicates the length of the    *
 * current user structure, divided by twenty.  This allows us to build up  *
 * a base of utilities, and be able to switch to a new format (while still *
 * not breaking anything) in the future.  Also, if usr.sruct_len==0, then  *
 * you MUST assume that the length of the structure is actually 180 bytes  *
 * long, as Opus (and Maximus v1.00 only) did not use this field.  In      *
 * other words:                                                            *
 *                                                                         *
 * len_of_struct=(usr.struct_len ? (usr.struct_len*20) : 180)              *
 *                                                                         *
 * In addition, you can assume that all user records in the user file are  *
 * the SAME size...  ie. You can just read the first user record out of    *
 * the file, and you are assured that the rest of the records in the file  *
 * area also the same size.                                                *
 *                                                                         *
 *                                                                         *
 * Example for reading in the dynamic-sized user structure:                *
 *                                                                         *
 *    {                                                                    *
 *      struct _usr users[MAX_USERS];                                      *
 *                                                                         *
 *      int x,                                                             *
 *          userfile,                                                      *
 *          s_len;                                                         *
 *                                                                         *
 *      if ((userfile=open(ufile_name,O_RDONLY | O_BINARY))==-1)           *
 *        Error();                                                         *
 *                                                                         *
 *      read(userfile,&users[0],sizeof(struct _usr));                      *
 *                                                                         *
 *      s_len=users[0].struct_len ? users[0].struct_len*20 : 180;          *
 *                                                                         *
 *      for (x=0;x < MAX_USERS;x++)                                        *
 *      {                                                                  *
 *        lseek(userfile,(long)x*(long)s_len,SEEK_SET);                    *
 *        read(userfile,&users[x],sizeof(struct _usr));                    *
 *      }                                                                  *
 *                                                                         *
 *      close(userfile);                                                   *
 *    }                                                                    *
 *                                                                         *
 * If anything is added to the user structure, it will be appended to the  *
 * END of the structure, so you can be assured that the offsets of each    *
 * individual variable will NOT change.                                    *
 *                                                                         *
 * Also, when ADDING or DELETING users, certain special operations have    *
 * to be performed, mainly those related to the lastread pointers.  When   *
 * adding a user, the procedure is fairly simple; just make sure that      *
 * usr.lastread_ptr is a unique number, different from all others in       *
 * USER.BBS.  Although Max uses a somewhat complicated algorithm to        *
 * fill gaps in the user file, most utility programs can just read through *
 * USER.BBS, and keep a running tally of the HIGHEST usr.struct_len        *
 * variable.  Once you have that, increment it by one, and stuff it into   *
 * the usr.struct_len of the user to be added.                             *
 *                                                                         *
 * When DELETING users, you must go through the process of "cleansing"     *
 * the lastread pointers for the user you deleted.  The procedure for this *
 * is simple:  For every area listed in AREAS.CTL, open the LASTREAD.BBS   *
 * file for that area, and seek to the offset...                           *
 *                                                                         *
 *    usr.lastread_ptr*(long)sizeof(int)                                   *
 *                                                                         *
 * ...and write *two* NUL bytes (ASCII 00).                                *
 *                                                                         *
 * Please note that you do NOT need to do anything special to sort the     *
 * user file...  Since the lastread offset is stored in usr.lastread_ptr,  *
 * you can sort the user file with impunity, and even use old Opus 1.03    *
 * sort utilities.                                                         */



/* Masks for usr.bits1, below */

#define BITS_HOTKEYS     0x0001 /* Hotkeys, independent of HOTFLASH level   */
#define BITS_NOTAVAIL    0x0002 /* If set, user is NOT normally available   *
                                 * for chat.                                */
#define BITS_FSR         0x0004 /* Full-screen reading in msg areas         */
#define BITS_NERD        0x0008 /* Yelling makes no noise on sysop console  */
#define BITS_NOULIST     0x0010 /* Don't display name in userlist           */
#define BITS_TABS        0x0020 /* Reserved                                 */
#define BITS_BIT6        0x0040 /* Reserved                                 */
#define BITS_BIT7        0x0080 /* Reserved                                 */
#define BITS_BIT8        0x0100 /* Used to be 'usr.msg'                     */
#define BITS_BIT9        0x0200 /* Used to be 'usr.msg'                     */
#define BITS_BITA        0x0400 /* Used to be 'usr.msg'                     */
#define BITS_BITB        0x0800 /* Used to be 'usr.msg'                     */
#define BITS_BITC        0x1000 /* Used to be 'usr.msg'                     */
#define BITS_BITD        0x2000 /* Used to be 'usr.msg'                     */
#define BITS_BITE        0x4000 /* Used to be 'usr.msg'                     */
#define BITS_BITF        0x8000 /* Used to be 'usr.msg'                     */


/* Masks for usr.bits2, below */

#define BITS2_BADLOGON   0x0001 /* MAX: if user's last logon attempt was bad*/
#define BITS2_IBMCHARS   0x0002 /* MAX: if user can receive high-bit chars  */
#define BITS2_RSVD1      0x0004 /* MAX: *obsolete* 1.02 avatar flag         */
#define BITS2_BORED      0x0008 /* Use the line-oriented editor             */
#define BITS2_MORE       0x0010 /* Wants the "MORE?" prompt                 */
#define BITS2_RSVD2      0x0020 /* OPUS: set=wants Ansi                     */
#define BITS2_CONFIGURED 0x0040 /* OPUS: set=used Maximus before            */
#define BITS2_CLS        0x0080 /* OPUS: set=transmit ^L, clear=ignore ^L   */
#define BITS2_BIT8       0x0100 /* used to be 'usr.keys'                    */
#define BITS2_BIT9       0x0200 /* used to be 'usr.keys'                    */
#define BITS2_BITA       0x0400 /* used to be 'usr.keys'                    */
#define BITS2_BITB       0x0800 /* used to be 'usr.keys'                    */
#define BITS2_BITC       0x1000 /* used to be 'usr.keys'                    */
#define BITS2_BITD       0x2000 /* used to be 'usr.keys'                    */
#define BITS2_BITE       0x4000 /* used to be 'usr.keys'                    */
#define BITS2_BITF       0x8000 /* used to be 'usr.keys'                    */


/* Masks for usr.delflag, below */

#define UFLAG_DEL   0x01
#define UFLAG_PERM  0x02

/* Masks for usr.xp_flag, below */

#define XFLAG_EXPDATE    0x0001 /* Use the xp_date to control access        */
#define XFLAG_EXPMINS    0x0002 /* Use the xp_mins number to control access */
#define XFLAG_DEMOTE     0x0004 /* Demote user to priv level in usr.xp_priv */
#define XFLAG_AXE        0x0008 /* Just hang up on user                     */

/* Constants for usr.video, below */

#define GRAPH_TTY         0x00 /* The current user's graphics setting...    */
#define GRAPH_ANSI        0x01
#define GRAPH_AVATAR      0x02


struct   maximus_usr
   {
      byte name[36];        /* Caller's name                                */
      byte city[36];        /* Caller's location                            */

      byte alias[21];       /* MAX: user's alias (handle)                   */
      byte phone[15];       /* MAX: user's phone number                     */

      word lastread_ptr;    /* MAX: a num which points to offset in LASTREAD*/
                            /* file -- Offset of lastread pointer will be   */
                            /* lastread_ptr*sizeof(int).                    */

      word timeremaining;   /* MAX: time left for current call (xtern prog) */

      byte pwd[16];         /* Password                                     */
      word times;           /* Number of previous calls to this system      */
      byte help;            /* Help level                                   */
/**/  byte rsvd1[2];        /* Reserved by Maximus for future use           */
      byte video;           /* user's video mode (see GRAPH_XXXX)           */
      byte nulls;           /* Number of Nulls (delays) after <cr>          */

      byte bits;            /* Bit flags for user (number 1)                */

/**/  word rsvd2;           /* Reserved by Maximus for future use           */

      word bits2;           /* Bit flags for user (number 2)                */

      word priv;           /* Access level                                 */
/**/  byte rsvd3[19];       /* Reserved by Maximus for future use           */
      byte struct_len;      /* len of struct, divided by 20. SEE ABOVE!     */
      word time;            /* Time on-line so far today                    */

      word delflag;         /* Used to hold baud rate for O)utside command  */
                            /* In USER.BBS, usr.flag uses the constants     */
                            /* UFLAG_xxx, defined earlier in this file.     */

/**/  byte rsvd4[8];        /* Reserved by Maximus for future use           */

      byte width;           /* Width of the caller's screen                 */
      byte len;             /* Height of the caller's screen                */
      word credit;          /* Matrix credit, in cents                      */
      word debit;           /* Current matrix debit, in cents               */

      word  xp_priv;        /* Priv to demote to, when time or minutes run  */
                            /* out.                                         */

      _stamp xp_date;  /* Bit-mapped date of when user expires.  */
                                  /* If zero, then no expiry date.          */

      dword xp_mins;        /* How many minutes the user has left before    *
                             * expiring.                                    */

      byte  xp_flag;        /* Flags for expiry.  See above XFLAG_XXX defs. */
      byte  xp_rsvd;

      _stamp ludate;   /* Bit-mapped date of user's last call    */

      dword xkeys;          /* User's keys (all 32 of 'em)                  */
      byte  lang;           /* The user's current language #                */
      byte  def_proto;      /* Default file-transfer protocol               */

      dword up;             /* K-bytes uploaded, all calls                  */
      dword down;           /* K-bytes downloaded, all calls                */
      dword downtoday;      /* K-bytes downloaded, today                    */

      byte msg[10];   /* User's last msg area (string)                */
      byte files[10]; /* User's last file area (string)               */

      byte compress;        /* Default compression program to use           */

/**/  byte rsvd5;
      dword extra;
   };
