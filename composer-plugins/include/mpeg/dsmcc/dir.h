//biop directory message

#ifndef _header_dsmcc_dir_
#define _header_dsmcc_dir_

#include "list.h"


#define DSMCC_DIR_SIZE           0x09
#define DSMCC_DIR_OBJ_INFO_LEN 0x0000
#define DSMCC_DIR_SCL_COUNT      0x00
#define DSMCC_DIR_MAX_BINDS    0x0200

typedef struct _DsmccDir
{
  unsigned int   key;
  unsigned char *dir;
  unsigned int   size;
  char          *path;
  unsigned char  srg;
} DsmccDir;


DsmccDir *
dsmcc_dir_new (char          *path,
               unsigned int   key,
               DsmccList     *binds,
               unsigned char  srg);


void
dsmcc_dir_free (DsmccDir *dir);


#endif
