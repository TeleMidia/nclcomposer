//data carousel contents

#ifndef _header_dsmcc_dcc_
#define _header_dsmcc_dcc_

#include "ents.h"
#include "exps.h"
#include "list.h"


typedef struct _DsmccDcc
{
  DsmccEnts *ents;
  DsmccEnt  *ent;
  DsmccExps *exps;
  DsmccList *dirs;
  DsmccList *objs;
  DsmccList *code;
} DsmccDcc;


DsmccDcc *
dsmcc_dcc_new (DsmccEnts *ents,
               DsmccEnt  *ent,
               DsmccExps *exps);


unsigned int
dsmcc_dcc_ents (DsmccDcc *dcc);


void
dsmcc_dcc_free (DsmccDcc *dcc);


void
dsmcc_dcc_show (DsmccDcc *dcc);


unsigned int
dsmcc_dcc_dump_write (DsmccDcc *dcc,
                      int       fd);


DsmccDcc *
dsmcc_dcc_dump_read (int fd);


#endif
