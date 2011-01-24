// biop object location

#include "../../../include/mpeg/dsmcc/oloc.h"

#include <stdlib.h>
#include <string.h>


unsigned char *
dsmcc_oloc_new (unsigned int    key,
                unsigned short  mid,
                unsigned int    cid,
                unsigned int   *len)
{
  unsigned char  *ret    = 0;
  unsigned int    pos    = 0;
  unsigned char   uc_tmp = 0;
  unsigned short  us_tmp = 0;
  unsigned int    ui_tmp = 0;

  ret = (unsigned char*)malloc (DSMCC_OLOC_SIZE);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", key, mid);
      return 0;
    }

  ui_tmp = DSMCC_OLOC_ID_TAG;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = DSMCC_OLOC_SIZE - DSMCC_OLOC_NOFF;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  ui_tmp = cid;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  us_tmp = mid;
  dsmcc_write_2 (ret, &pos, us_tmp);

  uc_tmp = DSMCC_OLOC_VER_MAJOR;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_OLOC_VER_MINOR;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_OLOC_OBJ_KEY_LEN;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  ui_tmp = key;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  (*len) = pos;

  return ret;
}
