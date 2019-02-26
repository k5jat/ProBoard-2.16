#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tslib.hpp>
#include "convert.hpp"
#include "maximus.hpp"

char maximus_path[80];

int
convert_maximus()
{
/*
 File f;
 FileName fn(maximus_path,"USER.BBS");

 File f_users_bbs,
      f_usersxi_bbs,
      f_users_pb;


 if(!f.open(fn , fmode_read , 2048))
   {
    printf("Can't open %s\n",(char *)fn);
    return -1;
   }

 f_users_bbs  .open(FileName(cfg.msgpath,"USERS.BBS")   , fmode_create , 2048);
 f_usersxi_bbs.open(FileName(cfg.msgpath,"USERSXI.BBS") , fmode_create , 2048);
 f_users_pb   .open(FileName(cfg.msgpath,"USERS.PB")    , fmode_create , 2048);

 if(!f_users_bbs.opened())
 {
    printf("Unable to create %s\n",(char *)FileName(cfg.msgpath,"USERS.BBS"));
    return -1;
 }

 if(!f_usersxi_bbs.opened())
 {
    printf("Unable to create %s\n",(char *)FileName(cfg.msgpath,"USERSXI.BBS"));
    return -1;
 }

 if(!f_users_pb.opened())
 {
    printf("Unable to create %s\n",(char *)FileName(cfg.msgpath,"USERS.PB"));
    return -1;
 }

 maximus_usr max_u;

 if(f.read(&max_u,sizeof(max_u)) != sizeof(max_u)) printf("Error reading %s\n",(char *)fn);

 long reclen = max_u.struct_len * 20;

 UsersBBS   *ubbs   = new UsersBBS;
 UsersXiBBS *uxibbs = new UsersXiBBS;
 UsersPbBBS *upbbbs = new UsersPbBBS;

 _User *user = new _User;

 for(long rec = 0;;rec++)
 {
    f.seek(rec * reclen);

    if(f.read(&max_u,sizeof(max_u)) != sizeof(max_u)) break;

    user->clear();

    strcpy (user->name     , max_u.name       );
    strncpy(user->city     , max_u.city , 25  ); user->city[25] = '\0';
    strcpy (user->alias    , max_u.alias      );
    strncpy(user->telnr    , max_u.phone , 12 ); user->telnr[12] = '\0';
    strcpy (user->password , max_u.pwd        );

    user->timescalled = max_u.times;

    user->uflags = UFLAG_STACKING |
                   UFLAG_NOIBM    ;

    switch(max_u.video)
    {
        case GRAPH_TTY    : break;
        case GRAPH_ANSI   : user->uflags |= UFLAG_ANSI;
                            break;
        case GRAPH_AVATAR : user->uflags |= UFLAG_ANSI | UFLAG_AVATAR;
    }



    if(max_u.bits  &  BITS_HOTKEYS)  user->uflags &= dword(~UFLAG_STACKING);
    if(max_u.bits  &  BITS_NOULIST)  user->uflags |=        UFLAG_HIDDEN;
    if(max_u.bits2 & BITS2_IBMCHARS) user->uflags &= dword(~UFLAG_NOIBM);
    if(max_u.bits2 & BITS2_BORED)    user->uflags &= dword(~UFLAG_FSED);
    if(max_u.bits2 & BITS2_MORE)     user->uflags |=        UFLAG_PAUSE;
    if(max_u.bits2 & BITS2_CLS)      user->uflags |=        UFLAG_CLEAR;

    if(max_u.delflag == UFLAG_DEL)  user->uflags |= UFLAG_DELETED;
    if(max_u.delflag == UFLAG_PERM) user->uflags |= UFLAG_NOKILL;

    user->level = max_u.priv * 10 + 10;

    user->screenlines = max_u.len;

    user->credit = max_u.credit;

    user->expirlevel = max_u.xp_priv * 10 + 10;

    if(max_u.xp_date.date.da)
    {
        user->expir[0] = max_u.xp_date.date.da;
        user->expir[1] = max_u.xp_date.date.mo;
        user->expir[2] = max_u.xp_date.date.yr + 80;
    }

    user->lastdate[0] = max_u.ludate.date.da;
    user->lastdate[1] = max_u.ludate.date.mo;
    user->lastdate[2] = max_u.ludate.date.yr + 80;

    user->lasttime[0] = max_u.ludate.time.hh;
    user->lasttime[1] = max_u.ludate.time.mm;
    user->lasttime[2] = max_u.ludate.time.ss;

    user->todayk = max_u.downtoday;
    user->kdownloaded = max_u.down;
    user->kuploaded   = max_u.up;

    user->to_ra(*ubbs,*uxibbs,*upbbbs);

    ubbs->XIrecord = rec;

    f_users_bbs  .write( ubbs   , sizeof(*ubbs));
    f_usersxi_bbs.write( uxibbs , sizeof(*uxibbs));
    f_users_pb   .write( upbbbs , sizeof(*upbbbs));

 }

 delete ubbs;
 delete upbbbs;
 delete uxibbs;
*/

 return 0;
}

