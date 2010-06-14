// mhp data broadcast id descriptor

#include "../../../include/mpeg/dsmcc/dbid.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccDbid *
dsmcc_dbid_new (unsigned short id,
                AppType        type)
{
  DsmccDbid      *ret    = 0;
  unsigned int    pos    = 0;
  unsigned char   uc_tmp = 0;
  unsigned short  us_tmp = 0;

  ret = new DsmccDbid;
  if (!ret)
    {
      DSMCC_MERR ("(0x%04x)", id);
      return 0;
    }

  ret->size = DSMCC_DBID_SIZE;

  ret->data = (unsigned char*)malloc (ret->size);
  if (!ret->data)
    {
      DSMCC_MERR ("(0x%04x)", id);

      free (ret);
      return 0;
    }

  uc_tmp = DSMCC_DBID_TAG;
  dsmcc_write_1 (ret->data, &pos, uc_tmp);

  uc_tmp = (ret->size - 2);
  dsmcc_write_1 (ret->data, &pos, uc_tmp);

  us_tmp = id;
  dsmcc_write_2 (ret->data, &pos, us_tmp);

  us_tmp = type;
  dsmcc_write_2 (ret->data, &pos, us_tmp);

  return ret;
}


void
dsmcc_dbid_free (DsmccDbid *dbid)
{
  if (dbid->data)
    {
      free (dbid->data);
    }

  free (dbid);
}
