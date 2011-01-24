//filesystem entity

#ifndef _header_dsmcc_ent_
#define _header_dsmcc_ent_

#ifndef WIN32
  #ifdef _MSC_VER
    #define WIN32
  #endif
#endif

#include "ext.h"
#include "list.h"
#include "util.h"


#define DSMCC_ENT_MAX_SIZE  0x0FC24BE2
#define DSMCC_ENT_UNDEF_TAG       0xFF


typedef struct _DsmccEnt
{
  char             *name;
  char             *rpath;
  char             *path;
  ObjectType        type;
  unsigned int      size;
  unsigned int      rlen;
  DsmccList        *ents;
  unsigned int      tid;
  unsigned char     ctag;
  unsigned int      key;
  unsigned int      mid;
  unsigned int      cid;
  DsmccList        *info;
  unsigned char     bptag;
  unsigned char     npttag;
  unsigned char     nptbase;
  unsigned char     stetag;
  unsigned int      dsec;
  unsigned int      dmsec;
  unsigned char     dmask;
  DsmccExt         *ext;
  unsigned int      crc;
  unsigned char     exist;
  unsigned char     update;
  struct _DsmccEnt *upent;
  unsigned char     dcroot;
  unsigned char     srg;
} DsmccEnt;


DsmccEnt *
dsmcc_ent_new (char *name);


void
dsmcc_ent_free (DsmccEnt *ent);


unsigned int
dsmcc_ent_dump_write (DsmccEnt *ent,
                      int       fd);


DsmccEnt *
dsmcc_ent_dump_read (int fd);


#endif
