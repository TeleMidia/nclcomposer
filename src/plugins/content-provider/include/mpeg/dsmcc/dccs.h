//data carousel contents  management

#ifndef _header_dsmcc_dccs_
#define _header_dsmcc_dccs_

#include "ents.h"


typedef struct _DsmccDccs
{
  DsmccEnts *ents;
  DsmccList *dccs;
} DsmccDccs;


DsmccDccs *
dsmcc_dccs_new (DsmccEnts *ents);


unsigned int
dsmcc_dccs_ents (DsmccDccs *dccs);


void
dsmcc_dccs_free (DsmccDccs *dccs);


void
dsmcc_dccs_show (DsmccDccs *dccs);


#endif
