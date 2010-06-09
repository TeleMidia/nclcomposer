//representation of an external object

#ifndef _header_dsmcc_ext_
#define _header_dsmcc_ext_


typedef struct _DsmccExt
{
  char           *orig;
  char           *path;
  unsigned int    cid;
  unsigned short  onid;
  unsigned short  tsid;
  unsigned short  sid;
} DsmccExt;


DsmccExt *
dsmcc_ext_new (char           *orig,
               char           *path,
               unsigned int    cid,
               unsigned short  onid,
               unsigned short  tsid,
               unsigned short  sid);


void
dsmcc_ext_free (DsmccExt *ext);


#endif
