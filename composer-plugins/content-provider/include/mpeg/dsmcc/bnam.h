//biop name

#ifndef _header_dsmcc_bnam_
#define _header_dsmcc_bnam_

#include "util.h"


#define DSMCC_BNAM_SIZE         0x06
#define DSMCC_BNAM_OBJ_KIND_LEN 0x04


unsigned char *
dsmcc_bnam_new (ObjectType    type,
                char         *name,
                unsigned int *len);


#endif
