//mhp data broadcast id descriptor

#ifndef _header_dsmcc_dbid_
#define _header_dsmcc_dbid_

#include "apps.h"


#define DSMCC_DBID_SIZE   0x06
#define DSMCC_DBID_TAG    0x66
#define DSMCC_DBID_PIPE 0x0001
#define DSMCC_DBID_ADS  0x0002
#define DSMCC_DBID_SSDS 0x0003
#define DSMCC_DBID_SDDS 0x0004
#define DSMCC_DBID_MPE  0x0005
#define DSMCC_DBID_DC   0x0006
#define DSMCC_DBID_OC   0x0007
#define DSMCC_DBID_ATM  0x0008


typedef struct _DsmccDbid
{
  unsigned int   size;
  unsigned char *data;
} DsmccDbid;


DsmccDbid *
dsmcc_dbid_new (unsigned short id,
                AppType        type);


void
dsmcc_dbid_free (DsmccDbid *dbid);


#endif
