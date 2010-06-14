//biop lite options profile body

#ifndef _header_dsmcc_lpro_
#define _header_dsmcc_lpro_

#include "list.h"
#include "util.h"


#define DSMCC_LPRO_SIZE             0x0A
#define DSMCC_LPRO_TAG        0x49534F05
#define DSMCC_LPRO_BYTE_ORDER       0x00
#define DSMCC_LPRO_LITE_COUNT       0x01


unsigned char *
dsmcc_lpro_new (ObjectType      type,
                char           *path,
                unsigned int    cid,
                unsigned short  onid,
                unsigned short  tsid,
                unsigned short  sid,
                unsigned int   *len);


#endif
