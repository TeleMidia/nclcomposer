// servicegateway

#include "../../../include/mpeg/dsmcc/srg.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/ior.h"


unsigned char *
dsmcc_srg_new (ObjectType      type,
               unsigned int    tid,
               unsigned short  atag,
               unsigned int    key,
               unsigned short  mid,
               unsigned int    cid,
               unsigned int   *len)
{
  unsigned char  *ret    = 0;
  unsigned int    pos    = 0;
  unsigned char   uc_tmp = 0;
  unsigned short  us_tmp = 0;
  unsigned int    iorlen = 0;
  unsigned char  *ior    = 0;
  unsigned short  inflen = 0;

  if (dsmcc_conf_get_dsi_info () && dsmcc_conf_get_dsi_info_len ())
    {
      inflen = dsmcc_conf_get_dsi_info_len ();
    }

  ior = dsmcc_ior_new (type, tid, atag, key, mid, cid, &iorlen);
  if (!ior)
    {
      DSMCC_MERR ("(0x%08x) (%s) (0x%04x)",
                  key, dsmcc_type_string (type), mid);
      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_SRG_SIZE + iorlen + inflen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (%s) (0x%04x)",
                  key, dsmcc_type_string (type), mid);
      free (ior);

      return 0;
    }

  memcpy (&ret[pos], ior, iorlen);
  pos += iorlen;

  uc_tmp = DSMCC_SRG_DTAP_COUNT;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_SRG_SCL_COUNT;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  us_tmp = inflen;
  dsmcc_write_2 (ret, &pos, us_tmp);

  if (inflen > 0)
    {
      memcpy (&ret[pos], dsmcc_conf_get_dsi_info(), inflen);
      pos += inflen;
    }

  (*len) = pos;

  free (ior);

  return ret;
}
