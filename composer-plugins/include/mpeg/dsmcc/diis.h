//download info indication management

#ifndef _header_dsmcc_diis_
#define _header_dsmcc_diis_

#include "mods.h"


typedef struct _DsmccDiis
{
  DsmccMods *mods;
  DsmccList *diis;
} DsmccDiis;


DsmccDiis *
dsmcc_diis_new (DsmccMods *mods);


void
dsmcc_diis_free (DsmccDiis *diis);


unsigned int
dsmcc_diis_encode (DsmccDiis *diis);


unsigned int
dsmcc_diis_dump_write (DsmccDiis *diis,
                       int        fd);


DsmccList *
dsmcc_diis_dump_read (int fd);


#endif
