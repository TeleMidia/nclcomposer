// representation of an external object

#include "../../../include/mpeg/dsmcc/ext.h"

#include <malloc.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccExt *
dsmcc_ext_new (char           *orig,
               char           *path,
               unsigned int    cid,
               unsigned short  onid,
               unsigned short  tsid,
               unsigned short  sid)
{
  DsmccExt *ret = 0;

  ret = (DsmccExt*)malloc (sizeof (DsmccExt));
  if (!ret)
    {
      DSMCC_MERR ("(%s) (0x%08x) (0x%04x) (0x%04x) (0x%04x)",
                  path, cid, onid, tsid, sid);
      return 0;
    }

  ret->orig = orig;
  ret->path = path;
  ret->cid  = cid;
  ret->onid = onid;
  ret->tsid = tsid;
  ret->sid  = sid;

  return ret;
}


void
dsmcc_ext_free (DsmccExt *ext)
{
  if (ext->path)
    {
      free (ext->path);
    }

  free (ext);
}
