// biop profile body

#include "../../../include/mpeg/dsmcc/bpro.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/conn.h"
#include "../../../include/mpeg/dsmcc/oloc.h"
#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_bpro_new (unsigned int    tid,
                unsigned short  atag,
                unsigned int    key,
                unsigned short  mid,
                unsigned int    cid,
                unsigned int   *len)
{
  unsigned char *ret     = 0;
  unsigned int   pos     = 0;
  unsigned char  uc_tmp  = 0;
  unsigned int   ui_tmp  = 0;
  unsigned int   oloclen = 0;
  unsigned char *oloc    = 0;
  unsigned int   connlen = 0;
  unsigned char *conn    = 0;

  oloc = dsmcc_oloc_new (key, mid, cid, &oloclen);
  if (!oloc)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", key, mid);
      return 0;
    }

  conn = dsmcc_conn_new (tid, atag, &connlen);
  if (!conn)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", key, mid);
      free (oloc);

      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_BPRO_SIZE + oloclen + connlen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", key, mid);
      free (oloc);
      free (conn);

      return 0;
    }

  ui_tmp = DSMCC_BPRO_ID_TAG;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = oloclen + connlen + DSMCC_BPRO_LITE_OFF;
  dsmcc_write_4 (ret, &pos, ui_tmp);
  
  uc_tmp = DSMCC_BPRO_BYTE_ORDER;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_BPRO_LITE_COUNT;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  memcpy (&ret[pos], oloc, oloclen);
  pos += oloclen;

  memcpy (&ret[pos], conn, connlen);
  pos += connlen;

  (*len) = pos;

  free (conn);
  free (oloc);

  return ret;
}
