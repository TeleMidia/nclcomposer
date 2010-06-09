// dsm stream info

#include "../../../include/mpeg/dsmcc/stri.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_stri_new (unsigned char *data,
                unsigned int   dlen,
                unsigned int   dsec,
                unsigned int   dmsec,
                unsigned char  dmask,
                unsigned int  *len)
{
  unsigned char *ret    = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;
  unsigned int   ui_tmp = 0;

  ret = (unsigned char*)malloc (DSMCC_STRI_SIZE + dlen);
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  uc_tmp = dlen;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  if (data && dlen > 0)
    {
      memcpy (&ret[pos], data, dlen);
      pos += dlen;
    }

  ui_tmp = dsec;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = dmsec;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = (dmask & DSMCC_STRI_AUDIO);
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = ((dmask & DSMCC_STRI_VIDEO) >> 1);
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = ((dmask & DSMCC_STRI_DATA) >> 2);
  dsmcc_write_1 (ret, &pos, uc_tmp);

  (*len) = pos;

  return ret;
}
