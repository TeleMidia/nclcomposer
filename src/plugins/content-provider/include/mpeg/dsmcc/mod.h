// representation of a module

#ifndef _header_dsmcc_mod_
#define _header_dsmcc_mod_

#include "ent.h"
#include "fil.h"
#include "list.h"
#include "stdb.h"

#include "../../functions.h"
using namespace ::org::isdtv::util;

typedef struct _DsmccMod
{
  DsmccList     *ents;
  unsigned int   mid;
  unsigned int   cid;
  unsigned int   did;
  unsigned int   tid;
  unsigned char  ver;
  unsigned char  ctag;
  unsigned int   size;
  unsigned char *data;
  DsmccFil      *sfil;
  unsigned char *lab;
  unsigned int   llen;
  unsigned char *prio;
  unsigned int   plen;
  unsigned char  upd;
} DsmccMod;


DsmccMod *
dsmcc_mod_new (unsigned int   mid,
               unsigned int   cid,
               unsigned int   did,
               unsigned int   tid,
               unsigned char  ver,
               unsigned char  ctag,
               char          *lab,
               unsigned char  prio,
               unsigned char  lev);


void
dsmcc_mod_free (DsmccMod *mod);


unsigned int
dsmcc_mod_add (DsmccMod *mod,
               DsmccEnt *ent);


unsigned int
dsmcc_mod_encode (DsmccMod  *mod,
                  DsmccStdb *stdb);


unsigned int
dsmcc_mod_dump_write (DsmccMod *mod,
                      int       fd);


DsmccMod *
dsmcc_mod_dump_read (int fd);


#endif
