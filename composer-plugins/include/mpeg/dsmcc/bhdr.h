// biop generic object message header

#ifndef _header_dsmcc_bhdr_
#define _header_dsmcc_bhdr_

#include "util.h"


#define DSMCC_BHDR_SIZE               0x19
#define DSMCC_BHDR_MAGIC        0x42494F50
#define DSMCC_BHDR_VER_MAJOR          0x01
#define DSMCC_BHDR_VER_MINOR          0x00
#define DSMCC_BHDR_BYTE_ORDER         0x00
#define DSMCC_BHDR_MSG_TYPE           0x00
#define DSMCC_BHDR_MSG_LEN_OFF        0x0D
#define DSMCC_BHDR_OBJ_KEY_LEN        0x04
#define DSMCC_BHDR_OBJ_KIND_LEN 0x00000004


unsigned char *
dsmcc_bhdr_new (unsigned int  key,
                ObjectType    type,
                unsigned int  mlen,
                unsigned int *len);


#endif
