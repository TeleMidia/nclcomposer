//biop binding

#ifndef _header_dsmcc_bind_
#define _header_dsmcc_bind_

#include "ext.h"
#include "util.h"


#define DSMCC_BIND_SIZE              0x04
#define DSMCC_BIND_TYPE_OBJECT       0x01
#define DSMCC_BIND_TYPE_CONTEXT      0x02
#define DSMCC_BIND_NAME_COMP_COUNT   0x01
#define DSMCC_BIND_OBJ_INFO_LEN    0x0000


typedef struct _DsmccBind
{
  ObjectType     type;
  unsigned int   tid;
  unsigned short atag;
  unsigned int   key;
  unsigned short mid;
  unsigned int   cid;
  unsigned char *bind;
  unsigned int   size;
} DsmccBind;


DsmccBind *
dsmcc_bind_new (ObjectType      type,
                char           *name,
                unsigned int    tid,
                unsigned short  atag,
                unsigned int    key,
                unsigned short  mid,
                unsigned int    cid,
                DsmccExt       *ext);



void
dsmcc_bind_free (DsmccBind *bind);


#endif
