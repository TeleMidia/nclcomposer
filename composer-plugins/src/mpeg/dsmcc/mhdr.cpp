// message header

#include "../../../include/mpeg/dsmcc/mhdr.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_mhdr_new (unsigned short  msid,
                unsigned int    tid,
                unsigned short  mlen,
                unsigned int   *len)
{
  unsigned char  *ret    = 0;
  unsigned int    pos    = 0;
  unsigned char   uc_tmp = 0;
  unsigned short  us_tmp = 0;
  unsigned int    ui_tmp = 0;

  ret = (unsigned char*)malloc (DSMCC_MHDR_SIZE);
  if (!ret)
    {
      DSMCC_MERR ();
      return 0;
    }

  uc_tmp = DSMCC_MHDR_PROTOCOL;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_MHDR_DSMCC_TYPE;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  us_tmp = msid;
  dsmcc_write_2 (ret, &pos, us_tmp);

  ui_tmp = tid;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = DSMCC_MHDR_RESERVED;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_MHDR_ADAP_LEN;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  us_tmp = mlen;
  dsmcc_write_2 (ret, &pos, us_tmp);

  (*len) = pos;

  return ret;
}
