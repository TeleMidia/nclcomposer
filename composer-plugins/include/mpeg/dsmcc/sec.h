// dsmcc private section

#ifndef _header_dsmcc_sec_
#define _header_dsmcc_sec_

#include "util.h"


#define DSMCC_SEC_HEADER_SIZE   0x08
#define DSMCC_SEC_MAX_SIZE    0x0FF4
#define DSMCC_SEC_SIZE_MASK   0xB000
#define DSMCC_SEC_VER_MASK      0xC1
#define DSMCC_SEC_TID_MSG       0x3B
#define DSMCC_SEC_TID_DDB       0x3C
#define DSMCC_SEC_TID_STR       0x3D
#define DSMCC_SEC_MAX_VER       0xFF


typedef struct _DsmccSec
{
  unsigned char   tid;
  unsigned short  tide;
  unsigned char   ver;
  unsigned char   num;
  unsigned char   last;
  unsigned short  size;
  unsigned int    crc;
  unsigned char  *data;
  unsigned int    dlen;
  unsigned char  *fhdr;
  unsigned int    flen;
  ObjectType      type;
  unsigned int    len;
  char           *path;
  unsigned int    off;
  unsigned int    olen;
} DsmccSec;


DsmccSec *
dsmcc_sec_new (ObjectType      type,
               unsigned short  tide,
               unsigned char   ver,
               unsigned char   num,
               unsigned char   last,
               unsigned char  *data,
               unsigned int    dlen,
               unsigned char  *fhdr,
               unsigned int    flen,
               char           *path,
               unsigned int    off,
               unsigned int    olen);

void
dsmcc_sec_free (DsmccSec *sec);


DsmccSec *
dsmcc_sec_complete (DsmccSec *sec);


void
dsmcc_sec_update (DsmccSec *sec);


#endif
