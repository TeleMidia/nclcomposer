// module section management

#ifndef _header_dsmcc_msec_
#define _header_dsmcc_msec_

#include "list.h"
#include "mod.h"


typedef struct _DsmccMsec
{
  DsmccList *secs;
  DsmccMod  *mod;
} DsmccMsec;


DsmccMsec *
dsmcc_msec_new (DsmccMod *mod);


void
dsmcc_msec_free (DsmccMsec *msec);


unsigned int
dsmcc_msec_encode (DsmccMsec *msec);


#endif
