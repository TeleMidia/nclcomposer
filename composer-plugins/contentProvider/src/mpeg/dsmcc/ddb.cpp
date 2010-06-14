// download data block

#include "../../../include/mpeg/dsmcc/ddb.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/mhdr.h"
#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_ddb_new (unsigned int    did,
               unsigned short  mid,
               unsigned char   mver,
               unsigned short  bnr,
               unsigned char  *data,
               unsigned int    dlen,
               unsigned int   *len)
{
  unsigned char  *ddb     = 0;
  unsigned int    pos     = 0;
  unsigned char   uc_tmp  = 0;
  unsigned short  us_tmp  = 0;
  unsigned int    mhdrlen = 0;
  unsigned char  *mhdr    = 0;
  unsigned int    size    = dlen;

  (*len) = 0;

  if (!data)
    {
      size = 0;
    }

  mhdr = dsmcc_mhdr_new (DSMCC_DDB_MSID, did, DSMCC_DDB_SIZE + dlen, &mhdrlen);
  if (!mhdr)
    {
      DSMCC_MERR ("(0x%04x)", mid);
      return 0;
    }

  ddb = (unsigned char*)malloc (DSMCC_DDB_SIZE + mhdrlen + size);
  if (!ddb)
    {
      DSMCC_MERR ("(0x%04x)", mid);
      free (mhdr);

      return 0;
    }

  memcpy (&ddb[pos], mhdr, mhdrlen);
  pos += mhdrlen;

  us_tmp = mid;
  dsmcc_write_2 (ddb, &pos, us_tmp);

  uc_tmp = mver;
  dsmcc_write_1 (ddb, &pos, uc_tmp);

  uc_tmp = DSMCC_DDB_RESERVED;
  dsmcc_write_1 (ddb, &pos, uc_tmp);

  us_tmp = bnr;
  dsmcc_write_2 (ddb, &pos, us_tmp);

  if (size)
    {
      memcpy (&ddb[pos], data, size);
      pos += size;
    }

  free (mhdr);

  (*len) = pos;

  return ddb;
}
