// biop generic object message header

#include "../../../include/mpeg/dsmcc/bhdr.h"

#include <malloc.h>
#include <string.h>


unsigned char *
dsmcc_bhdr_new (unsigned int  key,
                ObjectType    type,
                unsigned int  mlen,
                unsigned int *len)
{
  unsigned char *ret    = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;
  unsigned int   ui_tmp = 0;
  unsigned int   o_type = 0;

  switch (type)
    {
      case ot_fil:
        o_type = DSMCC_DATA_FIL;
        break;
      case ot_srg:
        o_type = DSMCC_DATA_SRG;
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
        DSMCC_MERR ("(0x%08x) (%s)", key, dsmcc_type_string (type));
        return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_BHDR_SIZE);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, dsmcc_type_string (type));
      return 0;
    }

  ui_tmp = DSMCC_BHDR_MAGIC;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = DSMCC_BHDR_VER_MAJOR;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_BHDR_VER_MINOR;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_BHDR_BYTE_ORDER;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_BHDR_MSG_TYPE;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  ui_tmp = mlen + DSMCC_BHDR_MSG_LEN_OFF;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = DSMCC_BHDR_OBJ_KEY_LEN;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  ui_tmp = key;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = DSMCC_BHDR_OBJ_KIND_LEN;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = o_type;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  (*len) = pos;

  return ret;
}
