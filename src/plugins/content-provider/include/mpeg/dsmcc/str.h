// biop stream message

#ifndef _header_dsmcc_str_
#define _header_dsmcc_str_

#include "list.h"


#define DSMCC_STR_SIZE      0x08
#define DSMCC_STR_SCL_COUNT 0x00


typedef struct _DsmccStr
{
  unsigned int   key;
  unsigned char  bptag;
  unsigned char  npttag;
  unsigned char  nptbase;
  unsigned char *str;
  unsigned int   size;
  char          *path;
} DsmccStr;


DsmccStr *
dsmcc_str_new (char          *path,
               unsigned int   key,
               unsigned char *info,
               unsigned int   ilen,
               unsigned int   dsec,
               unsigned int   dmsec,
               unsigned char  dmask,
               unsigned char  bptag,
               unsigned char  npttag,
               unsigned char  nptbase);


void
dsmcc_str_free (DsmccStr *str);


#endif
