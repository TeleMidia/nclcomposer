// stream identifier descriptor

#include "../../../include/mpeg/dsmcc/sid.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccSid *
dsmcc_sid_new (unsigned char ctag)
{
  DsmccSid      *ret    = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;

  ret = new DsmccSid;
  if (!ret)
    {
      DSMCC_MERR ("(0x%02x)", ctag);
      return 0;
    }

  ret->size = DSMCC_SID_SIZE;

  ret->data = (unsigned char*)malloc (ret->size);
  if (!ret->data)
    {
      DSMCC_MERR ("(0x%02x)", ctag);

      free (ret);
      return 0;
    }

  uc_tmp = DSMCC_SID_TAG;
  dsmcc_write_1 (ret->data, &pos, uc_tmp);

  uc_tmp = (ret->size - 2);
  dsmcc_write_1 (ret->data, &pos, uc_tmp);

  uc_tmp = ctag;
  dsmcc_write_1 (ret->data, &pos, uc_tmp);

  return ret;
}


void
dsmcc_sid_free (DsmccSid *sid)
{
  if (sid->data)
    {
      free (sid->data);
    }

  free (sid);
}
