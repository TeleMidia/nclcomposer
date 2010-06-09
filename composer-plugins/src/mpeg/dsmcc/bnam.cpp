// biop name

#include "../../../include/mpeg/dsmcc/bnam.h"

#include <malloc.h>
#include <string.h>


unsigned char *
dsmcc_bnam_new (ObjectType    type,
                char         *name,
                unsigned int *len)
{
  unsigned char *ret    = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;
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

  ret = (unsigned char*)malloc (DSMCC_BNAM_SIZE + n_len);
  if (!ret)
    {
      DSMCC_MERR ("(%s) (%s)", dsmcc_type_string (type), name);
      return 0;
    }

  uc_tmp = n_len;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  memcpy (&ret[pos], name, n_len);
  pos += n_len;

  uc_tmp = DSMCC_BNAM_OBJ_KIND_LEN;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  ui_tmp = o_type;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  (*len) = pos;

  return ret;
}
