// stream event descriptor

#include "../../../include/mpeg/dsmcc/sted.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccSted *
dsmcc_sted_new (unsigned short  id,
                unsigned int    npt,
                unsigned char  *data,
                unsigned int    dlen)
{
  DsmccSted      *ret    = 0;
  unsigned char  *sted   = 0;
  unsigned int    pos    = 0;
  unsigned char   uc_tmp = 0;
  unsigned short  us_tmp = 0;
  unsigned int    ui_tmp = 0;

  ret = (DsmccSted*)malloc (sizeof (DsmccSted));
  if (!ret)
    {
      DSMCC_MERR ("(0x%04x) (0x%08x)", id, npt);

      return 0;
    }

  ret->id  = id;
  ret->npt = npt;

  sted = (unsigned char*)malloc (DSMCC_STED_SIZE + dlen);
  if (!sted)
    {
      DSMCC_MERR ("(0x%04x) (0x%08x)", id, npt);

      free (ret);
      return 0;
    }

  uc_tmp = DSMCC_STED_TAG;
  dsmcc_write_1 (sted, &pos, uc_tmp);

  uc_tmp = DSMCC_STED_SIZE + dlen - 2;
  dsmcc_write_1 (sted, &pos, uc_tmp);

  us_tmp = id;
  dsmcc_write_2 (sted, &pos, us_tmp);

  ui_tmp = DSMCC_STED_RESERVED;
  dsmcc_write_4 (sted, &pos, ui_tmp);

  ui_tmp = npt;
  dsmcc_write_4 (sted, &pos, ui_tmp);

  if (data)
    {
      memcpy (&sted[pos], data, dlen);
      pos += dlen;
    }

  ret->sted = sted;
  ret->size = pos;

  return ret;
}


void
dsmcc_sted_free (DsmccSted *sted)
{
  if (sted->sted)
    {
      free (sted->sted);
    }

  free (sted);
}
