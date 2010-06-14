//filesystem entity management

#ifndef _header_dsmcc_ents_
#define _header_dsmcc_ents_

#include "ent.h"
#include "list.h"


typedef struct _DsmccEnts
{
  DsmccEnt  *root;
  DsmccList *dirs;
  DsmccList *objs;
  DsmccList *code;
} DsmccEnts;


DsmccEnts *
dsmcc_ents_new (char *name);


void
dsmcc_ents_free (DsmccEnts *ents);


#endif
