// compressed module descriptor

#include "../../../include/mpeg/dsmcc/mcd.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_mcd_new (unsigned int  orig,
               unsigned int *len)
{
  unsigned char *ret    = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;
  unsigned int   ui_tmp = 0;

  ret = (unsigned char*)malloc (DSMCC_MCD_SIZE);
  if (!ret)
    {
      DSMCC_MERR ("(%d)", orig);
      return 0;
    }

  uc_tmp = DSMCC_MCD_TAG;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = (DSMCC_MCD_SIZE - 2);
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_MCD_METHOD;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  ui_tmp = orig;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  (*len) = pos;

  return ret;
}
