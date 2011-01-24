// interoperable object reference

#ifndef _header_dsmcc_ior_
#define _header_dsmcc_ior_

#include "ext.h"
#include "util.h"


#define DSMCC_IOR_SIZE              0x0C
#define DSMCC_IOR_TYPE_ID_LEN 0x00000004
#define DSMCC_IOR_PROF_COUNT  0x00000001


unsigned char *
dsmcc_ior_new (ObjectType      type,
               unsigned int    tid,
               unsigned short  atag,
               unsigned int    key,
               unsigned short  mid,
               unsigned int    cid,
               unsigned int   *len);


unsigned char *
dsmcc_ior_new_ext (ObjectType    type,
                   DsmccExt     *ext,
                   unsigned int *len);


#endif
