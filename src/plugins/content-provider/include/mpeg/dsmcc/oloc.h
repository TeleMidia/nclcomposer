// biop object location

#ifndef _header_dsmcc_oloc_
#define _header_dsmcc_oloc_

#include "util.h"


#define DSMCC_OLOC_SIZE              0x12
#define DSMCC_OLOC_NOFF              0x05
#define DSMCC_OLOC_ID_TAG      0x49534F50
#define DSMCC_OLOC_VER_MAJOR         0x01
#define DSMCC_OLOC_VER_MINOR         0x00
#define DSMCC_OLOC_OBJ_KEY_LEN       0x04


unsigned char *
dsmcc_oloc_new (unsigned int    key,
                unsigned short  mid,
                unsigned int    cid,
                unsigned int   *len);


#endif
