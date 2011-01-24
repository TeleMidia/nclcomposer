// dsm service location

#ifndef _header_dsmcc_sloc_
#define _header_dsmcc_sloc_

#include "util.h"


#define DSMCC_SLOC_SIZE          0x0E
#define DSMCC_SLOC_TAG     0x49534F46
#define DSMCC_SLOC_CTX_LEN 0x00000000


unsigned char *
dsmcc_sloc_new (ObjectType      type,
                char           *path,
                unsigned int    cid,
                unsigned short  onid,
                unsigned short  tsid,
                unsigned short  sid,
                unsigned int   *len);


#endif
