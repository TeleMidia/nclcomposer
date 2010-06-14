// module label descriptor

#include "../../../include/mpeg/dsmcc/mld.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_mld_new (char         *label,
               unsigned int *len)
{
  unsigned char *ret    = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;
  unsigned int   lablen = 0;

  (*len) = 0;

  if (!label)
    {
      return 0;
    }

  lablen = strlen (label) + 1;
  if (lablen > DSMCC_MLD_MAX_LEN)
    {
      DSMCC_MERR ("module-label %s too long, max. length 0%02x.",
                  label, DSMCC_MLD_MAX_LEN);
      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_MLD_SIZE + lablen);
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  uc_tmp = DSMCC_MLD_TAG;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = (DSMCC_MLD_SIZE + lablen - 2);
  dsmcc_write_1 (ret, &pos, uc_tmp);

  memcpy (&ret[pos], label, lablen);
  pos += lablen;

  (*len) = pos;

  return ret;
}
