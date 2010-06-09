// module content type descriptor

#include "../../../include/mpeg/dsmcc/mctd.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_mctd_new (char         *type,
                unsigned int *len)
{
  unsigned char *ret    = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;
  unsigned int   typlen = 0;

  ret = (unsigned char*)malloc (DSMCC_MCTD_SIZE);
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (type)
    {
      typlen = strlen (type) + 1;
    }

  uc_tmp = DSMCC_MCTD_TAG;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = (DSMCC_MCTD_SIZE + typlen - 2);
  dsmcc_write_1 (ret, &pos, uc_tmp);

  if (typlen)
    {
      memcpy (&ret[pos], type, typlen);
      pos += typlen;
    }

  (*len) = pos;

  return ret;
}
