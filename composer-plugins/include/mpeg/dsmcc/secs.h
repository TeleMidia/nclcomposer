// dsmcc private section management

#ifndef _header_dsmcc_secs_
#define _header_dsmcc_secs_

#include "diis.h"
#include "dsi.h"
#include "mods.h"
#include "pes.h"


typedef struct _DsmccSecs
{
  DsmccList *msecs;
  DsmccList *dsecs;
  DsmccList *ssecs;
  DsmccDsi  *dsi;
  DsmccDiis *diis;
  DsmccMods *mods;
  DsmccPes  *pes;
} DsmccSecs;


DsmccSecs *
dsmcc_secs_new (DsmccDsi  *dsi,
                DsmccDiis *diis,
                DsmccMods *mods);


void
dsmcc_secs_free (DsmccSecs *secs);


unsigned int
dsmcc_secs_encode (DsmccSecs *secs);


unsigned int
dsmcc_secs_encode (DsmccSecs *secs);


unsigned int
dsmcc_secs_encode_pes (DsmccSecs *secs,
                       DsmccPes  *pes);


#endif
