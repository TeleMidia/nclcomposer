// cos naming name component

#ifndef _header_dsmcc_cnam_
#define _header_dsmcc_cnam_

#include "util.h"


#define DSMCC_CNAM_SIZE               0x0C
#define DSMCC_CNAM_OBJ_KIND_LEN 0x00000004


typedef struct _DsmccCnam
{
  ObjectType     type;
  unsigned char *cnam;
  unsigned int   size;
  char          *name;
} DsmccCnam;


DsmccCnam *
dsmcc_cnam_new (ObjectType  type,
                char       *name);


void
dsmcc_cnam_free (DsmccCnam *cnam);


#endif
