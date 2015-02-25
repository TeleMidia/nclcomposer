// servicegateway

#ifndef _header_dsmcc_srg_
#define _header_dsmcc_srg_

#include "util.h"


#define DSMCC_SRG_SIZE        0x04
#define DSMCC_SRG_DTAP_COUNT  0x00
#define DSMCC_SRG_SCL_COUNT   0x00
#define DSMCC_SRG_MAX_USERLEN 0x0D00


unsigned char *
dsmcc_srg_new (ObjectType      type,
               unsigned int    tid,
               unsigned short  atag,
               unsigned int    key,
               unsigned short  mid,
               unsigned int    cid,
               unsigned int   *len);


#endif
