// dsm tap

#include "../../../include/mpeg/dsmcc/dtap.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_dtap_new (unsigned int    id,
                unsigned short  use,
                unsigned short  atag,
                unsigned int   *len)
{
  unsigned char  *ret    = 0;
  unsigned int    pos    = 0;
  unsigned char   uc_tmp = 0;
  unsigned short  us_tmp = 0;

  ret = (unsigned char*)malloc (DSMCC_DTAP_SIZE);
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  us_tmp = id;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = use;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = atag;
  dsmcc_write_2 (ret, &pos, us_tmp);

  uc_tmp = DSMCC_DTAP_SEL_LEN;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  (*len) = pos;

  return ret;
}
