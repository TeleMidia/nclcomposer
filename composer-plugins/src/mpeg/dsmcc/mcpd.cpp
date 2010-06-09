// module caching priority descriptor

#include "../../../include/mpeg/dsmcc/mcpd.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_mcpd_new (unsigned char  prio,
                unsigned char  level,
                unsigned int  *len)
{
  unsigned char *ret    = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;

  (*len) = 0;

  if (level == DSMCC_MCPD_LEVEL_NONE)
    {
      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_MCPD_SIZE);
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  uc_tmp = DSMCC_MCPD_TAG;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = (DSMCC_MCPD_SIZE - 2);
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = prio;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = level;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  (*len) = pos;

  return ret;
}
