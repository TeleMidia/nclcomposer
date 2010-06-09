// stream event database

#ifndef _header_dsmcc_stdb_
#define _header_dsmcc_stdb_

#include "list.h"


typedef struct _DsmccStdbEnt
{
  unsigned short  id;
  char           *desc;
} DsmccStdbEnt;


typedef struct _DsmccStdb
{
  DsmccList *evs;
  DsmccList *secs;
} DsmccStdb;


DsmccStdb *
dsmcc_stdb_new (void);


void
dsmcc_stdb_free (DsmccStdb *stdb);


unsigned int
dsmcc_stdb_add (DsmccStdb      *stdb,
                unsigned short  id,
                char           *desc);


unsigned int
dsmcc_stdb_encode (DsmccStdb *stdb);


#endif
