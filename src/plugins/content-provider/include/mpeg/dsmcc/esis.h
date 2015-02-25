//elementary stream info specification

#ifndef _header_dsmcc_esis_
#define _header_dsmcc_esis_

#include "apps.h"


typedef short DsmccStreamType;

static const short st_oc = 0x0B;
static const short st_ste = 0x0C;
static const short st_ait = 0x05;

typedef struct _DsmccEsis
{
  unsigned short  pid;
  DsmccStreamType type;
  unsigned char   ctag;
  unsigned int    cid;
  AppType         atype;
} DsmccEsis;


DsmccEsis *
dsmcc_esis_new (DsmccStreamType  type,
                char            *spec);


void
dsmcc_esis_print (DsmccEsis *esis);


void
dsmcc_esis_free (DsmccEsis *esis);


#endif
