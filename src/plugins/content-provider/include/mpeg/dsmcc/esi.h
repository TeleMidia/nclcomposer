//elementary stream info

#ifndef _header_dsmcc_esi_
#define _header_dsmcc_esi_

#include "asd.h"
#include "cid.h"
#include "dbid.h"
#include "esis.h"
#include "sid.h"


#define DSMCC_ESI_SIZE       0x05
#define DSMCC_ESI_PID_MASK 0xE000
#define DSMCC_ESI_ESI_MASK 0xF000
#define DSMCC_ESI_ST_OC      0x0B
#define DSMCC_ESI_ST_STE     0x0C
#define DSMCC_ESI_ST_AIT     0x05


typedef struct _DsmccEsi
{
  unsigned short   pid;
  DsmccStreamType  type;
  unsigned int     size;
  unsigned char   *data;
  DsmccAsd        *asd;
  DsmccDbid       *dbid;
  DsmccSid        *sid;
  DsmccCid        *cid;
} DsmccEsi;


DsmccEsi *
dsmcc_esi_new (DsmccEsis *esis);


void
dsmcc_esi_free (DsmccEsi *esi);


unsigned int
dsmcc_esi_add_app (DsmccEsi *esi,
                   DsmccAsd *asd);


unsigned int
dsmcc_esi_add_id (DsmccEsi  *esi,
                  DsmccDbid *dbid);


unsigned int
dsmcc_esi_add_sid (DsmccEsi *esi,
                   DsmccSid *sid);


unsigned int
dsmcc_esi_add_cid (DsmccEsi *esi,
                   DsmccCid *cid);


unsigned int
dsmcc_esi_encode (DsmccEsi *esi);


#endif
