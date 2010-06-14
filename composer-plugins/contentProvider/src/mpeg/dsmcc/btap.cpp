// biop tap

#include "../../../include/mpeg/dsmcc/btap.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_btap_new (unsigned int    tid,
                unsigned short  atag,
                unsigned int   *len)
{
  unsigned char  *ret    = 0;
  unsigned int    pos    = 0;
  unsigned char   uc_tmp = 0;
  unsigned short  us_tmp = 0;
  unsigned int    ui_tmp = 0;

  ret = (unsigned char*)malloc (DSMCC_BTAP_SIZE);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", tid, atag);
      return 0;
    }

  us_tmp = DSMCC_BTAP_ID;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = DSMCC_BTAP_USE;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = atag;
  dsmcc_write_2 (ret, &pos, us_tmp);

  uc_tmp = DSMCC_BTAP_SEL_LEN;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  us_tmp = DSMCC_BTAP_SEL_TYPE;
  dsmcc_write_2 (ret, &pos, us_tmp);

  ui_tmp = tid;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = DSMCC_BTAP_TIMEOUT;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  (*len) = pos;

  return ret;
}
