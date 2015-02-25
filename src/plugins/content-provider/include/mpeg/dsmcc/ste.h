// biop stream event message

#ifndef _header_dsmcc_ste_
#define _header_dsmcc_ste_

#include "list.h"


#define DSMCC_STE_SIZE      0x0B
#define DSMCC_STE_SCL_COUNT 0x00


typedef struct _DsmccSte
{
  unsigned int    key;
  unsigned char   bptag;
  unsigned char   npttag;
  unsigned char   nptbase;
  unsigned char   stetag;
  unsigned char  *ste;
  unsigned int    size;
  char           *path;
} DsmccSte;


DsmccSte *
dsmcc_ste_new (char           *path,
               unsigned int    key,
               unsigned char  *info,
               unsigned int    ilen,
               unsigned int    dsec,
               unsigned int    dmsec,
               unsigned char   dmask,
               unsigned char   bptag,
               unsigned char   npttag,
               unsigned char   nptbase,
               unsigned char   stetag,
               DsmccList      *names,
               unsigned short *ids);


void
dsmcc_ste_free (DsmccSte *ste);


#endif
