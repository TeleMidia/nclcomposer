// carousel identifier descriptor

#include "../../../include/mpeg/dsmcc/cid.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/bhdr.h"
#include "../../../include/mpeg/dsmcc/ddb.h"
#include "../../../include/mpeg/dsmcc/mcd.h"
#include "../../../include/mpeg/dsmcc/util.h"


DsmccCid *
dsmcc_cid_new (unsigned int   cid,
               unsigned char  form,
               unsigned char  mver,
               unsigned short mid,
               unsigned int   msize,
               unsigned char  compr,
               unsigned int   osize,
               unsigned int   okey)
{
  DsmccCid       *ret    = 0;
  unsigned int    pos    = 0;
  unsigned char   uc_tmp = 0;
  unsigned short  us_tmp = 0;
  unsigned int    ui_tmp = 0;

  ret = new DsmccCid;
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%02x) (0x%04x) (0x%08x) (0x%08x) (0x%08x)",
                  cid, mver, mid, msize, osize, okey);
      return 0;
    }

  if (form)
    {
      ret->size = DSMCC_CID_SIZE;
    }
  else
    {
      ret->size = DSMCC_CID_NSIZE;
    }

  ret->data = (unsigned char*)malloc (ret->size);
  if (!ret->data)
    {
      DSMCC_MERR ("(0x%08x) (0x%02x) (0x%04x) (0x%08x) (0x%08x) (0x%08x)",
                  cid, mver, mid, msize, osize, okey);

      free (ret);
      return 0;
    }

  uc_tmp = DSMCC_CID_TAG;
  dsmcc_write_1 (ret->data, &pos, uc_tmp);

  uc_tmp = (ret->size - 2);
  dsmcc_write_1 (ret->data, &pos, uc_tmp);

  ui_tmp = cid;
  dsmcc_write_4 (ret->data, &pos, ui_tmp);

  if (form)
    {
      uc_tmp = DSMCC_CID_FORMID;
      dsmcc_write_1 (ret->data, &pos, uc_tmp);

      uc_tmp = mver;
      dsmcc_write_1 (ret->data, &pos, uc_tmp);

      us_tmp = mid;
      dsmcc_write_2 (ret->data, &pos, us_tmp);

      us_tmp = DSMCC_DDB_MAX_SIZE;
      dsmcc_write_2 (ret->data, &pos, us_tmp);

      ui_tmp = osize;
      dsmcc_write_4 (ret->data, &pos, ui_tmp);

      if (compr)
        {
          uc_tmp = DSMCC_MCD_METHOD;
          dsmcc_write_1 (ret->data, &pos, uc_tmp);

          ui_tmp = osize;
          dsmcc_write_4 (ret->data, &pos, ui_tmp);
        }
      else
        {
          uc_tmp = DSMCC_CID_NOCOMPR;
          dsmcc_write_1 (ret->data, &pos, uc_tmp);

          ui_tmp = msize;
          dsmcc_write_4 (ret->data, &pos, ui_tmp);
        }

      uc_tmp = DSMCC_CID_TIMEOUT;
      dsmcc_write_1 (ret->data, &pos, uc_tmp);

      uc_tmp = DSMCC_BHDR_OBJ_KEY_LEN;
      dsmcc_write_1 (ret->data, &pos, uc_tmp);

      ui_tmp = okey;
      dsmcc_write_4 (ret->data, &pos, ui_tmp);
    }
  else
    {
      uc_tmp = DSMCC_CID_NFORMID;
      dsmcc_write_1 (ret->data, &pos, uc_tmp);
    }

  return ret;
}


void
dsmcc_cid_free (DsmccCid *cid)
{
  if (cid->data)
    {
      free (cid->data);
    }

  free (cid);
}
