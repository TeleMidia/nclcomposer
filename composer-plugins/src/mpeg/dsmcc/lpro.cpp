// biop lite options profile body

#include "../../../include/mpeg/dsmcc/lpro.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/sloc.h"


unsigned char *
dsmcc_lpro_new (ObjectType      type,
                char           *path,
                unsigned int    cid,
                unsigned short  onid,
                unsigned short  tsid,
                unsigned short  sid,
                unsigned int   *len)
{
  unsigned char  *ret     = 0;
  unsigned int    pos     = 0;
  unsigned char   uc_tmp  = 0;
  unsigned int    ui_tmp  = 0;
  unsigned char  *sloc    = 0;
  unsigned int    sloclen = 0;

  sloc = dsmcc_sloc_new (type, path, cid, onid, tsid, sid, &sloclen);
  if (!sloc)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (0x%08x) (0x%04x) (%s) (%s)",
                  cid, onid, tsid, sid, dsmcc_type_string (type), path);
      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_LPRO_SIZE + sloclen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (0x%08x) (0x%04x) (%s) (%s)",
                  cid, onid, tsid, sid, dsmcc_type_string (type), path);
      free (sloc);
      return 0;
    }

  ui_tmp = DSMCC_LPRO_TAG;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = sloclen + 2;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = DSMCC_LPRO_BYTE_ORDER;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_LPRO_LITE_COUNT;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  memcpy (&ret[pos], sloc, sloclen);
  pos += sloclen;

  free (sloc);

  (*len) = pos;

  return ret;
}
