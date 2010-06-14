// module management

#ifndef _header_dsmcc_mods_
#define _header_dsmcc_mods_

#include "dcc.h"
#include "list.h"
#include "mod.h"
#include "stdb.h"


#define DSMCC_MODULE_SIZE 0xFFFF


typedef struct _DsmccMods
{
  DsmccMod  *srg;
  DsmccList *mods;
  DsmccDcc  *dcc;
  DsmccStdb *stdb;
} DsmccMods;


DsmccMods *
dsmcc_mods_new (DsmccDcc *dcc);


void
dsmcc_mods_free (DsmccMods *mods);


unsigned int
dsmcc_mods_ents (DsmccMods *mods);


unsigned int
dsmcc_mods_encode (DsmccMods *mods);


unsigned int
dsmcc_mods_dump_write (DsmccMods *mods,
                       int        fd);


DsmccList *
dsmcc_mods_dump_read (int fd);


#endif
