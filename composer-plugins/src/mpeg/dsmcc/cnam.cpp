// cos naming name component

#include "../../../include/mpeg/dsmcc/cnam.h"

#include <malloc.h>
#include <string.h>


DsmccCnam *
dsmcc_cnam_new (ObjectType  type,
                char       *name)
{
  DsmccCnam     *cnam   = 0;
  unsigned char *ret    = 0;
  unsigned int   pos    = 0;
  unsigned int   ui_tmp = 0;
  unsigned int   o_type = 0;
  unsigned int   n_len  = strlen (name) + 1;

  switch (type)
    {
      case ot_fil:
        o_type = DSMCC_DATA_FIL;
        break;
      case ot_dir:
        o_type = DSMCC_DATA_DIR;
        break;
      case ot_str:
        o_type = DSMCC_DATA_STR;
        break;
      case ot_ste:
        o_type = DSMCC_DATA_STE;
        break;
      default:
        DSMCC_MERR ("(%s) (%s)", dsmcc_type_string (type), name);
        return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_CNAM_SIZE + n_len);
  if (!ret)
    {
      DSMCC_MERR ("(%s) (%s)", dsmcc_type_string (type), name);
      return 0;
    }

  ui_tmp = n_len;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  memcpy (&ret[pos], name, n_len);
  pos += n_len;

  ui_tmp = DSMCC_CNAM_OBJ_KIND_LEN;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = o_type;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  cnam = (DsmccCnam*)malloc (sizeof (DsmccCnam));
  if (!cnam)
    {
      DSMCC_MERR ("(%s) (%s)", dsmcc_type_string (type), name);
      return 0;
    }

  cnam->type = type;
  cnam->cnam = ret;
  cnam->name = name;
  cnam->size = pos;

  return cnam;
}


void
dsmcc_cnam_free (DsmccCnam *cnam)
{
  if (cnam->cnam)
    {
      free (cnam->cnam);
    }

  if (cnam->name)
    {
      free (cnam->name);
    }

  free (cnam);
}
