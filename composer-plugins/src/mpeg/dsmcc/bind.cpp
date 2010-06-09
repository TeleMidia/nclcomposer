//biop binding

#include "../../../include/mpeg/dsmcc/bind.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/ior.h"
#include "../../../include/mpeg/dsmcc/bnam.h"


DsmccBind *
dsmcc_bind_new (ObjectType      type,
                char           *name,
                unsigned int    tid,
                unsigned short  atag,
                unsigned int    key,
                unsigned short  mid,
                unsigned int    cid,
                DsmccExt       *ext)
{
  DsmccBind      *bind    = 0;
  unsigned char  *ret     = 0;
  unsigned int    pos     = 0;
  unsigned char   uc_tmp  = 0;
  unsigned short  us_tmp  = 0;
  unsigned char   b_type  = 0;
  unsigned int    bnamlen = 0;
  unsigned char  *bnam    = 0;
  unsigned int    iorlen  = 0;
  unsigned char  *ior     = 0;

  switch (type)
    {
      case ot_dir:
        b_type = DSMCC_BIND_TYPE_CONTEXT;
        break;
      case ot_fil:
      case ot_str:
      case ot_ste:
        b_type = DSMCC_BIND_TYPE_OBJECT;
        break;
      default:
        DSMCC_MERR ("(0x%08x) (%s) (%s)", key, dsmcc_type_string (type), name);
        return 0;
    }

  bnam = dsmcc_bnam_new (type, name, &bnamlen);
  if (!bnam)
    {
      DSMCC_MERR ("(0x%08x) (%s) (%s)", key, dsmcc_type_string (type), name);
      return 0;
    }

  if (ext)
    {
      ior = dsmcc_ior_new_ext (type, ext, &iorlen);
    }
  else
    {
      ior = dsmcc_ior_new (type, tid, atag, key, mid, cid, &iorlen);
    }

  if (!ior)
    {
      DSMCC_MERR ("(0x%08x) (%s) (%s)", key, dsmcc_type_string (type), name);
      free (bnam);

      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_BIND_SIZE + bnamlen + iorlen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (%s) (%s)", key, dsmcc_type_string (type), name);
      free (ior);
      free (bnam);

      return 0;
    }

  uc_tmp = DSMCC_BIND_NAME_COMP_COUNT;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  memcpy (&ret[pos], bnam, bnamlen);
  pos += bnamlen;

  uc_tmp = b_type;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  memcpy (&ret[pos], ior, iorlen);
  pos += iorlen;

  us_tmp = DSMCC_BIND_OBJ_INFO_LEN;
  dsmcc_write_2 (ret, &pos, us_tmp);

  free (ior);
  free (bnam);

  bind = (DsmccBind*)malloc (sizeof (DsmccBind));
  if (!bind)
    {
      DSMCC_MERR ("(0x%08x) (%s) (%s)", key, dsmcc_type_string (type), name);
      return 0;
    }

  bind->bind = ret;
  bind->size = pos;
  bind->type = type;
  bind->tid  = tid;
  bind->atag = atag;
  bind->key  = key;
  bind->mid  = mid;
  bind->cid  = cid;

  return bind;
}


void
dsmcc_bind_free (DsmccBind *bind)
{
  if (bind->bind)
    {
      free (bind->bind);
    }

  free (bind);
}
