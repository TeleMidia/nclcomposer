// biop module

#include "../../../include/mpeg/dsmcc/bmod.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccBmod *
dsmcc_bmod_new (unsigned short  mid,
                unsigned int    msize,
                unsigned char   mver,
                unsigned char   ctag,
                unsigned char  *mld,
                unsigned int    mldlen,
                unsigned char  *mcpd,
                unsigned char   mcpdlen)
{ 
  DsmccBmod      *bmod   = 0;
  unsigned char  *ret    = 0;
  unsigned int    pos    = 0;
  unsigned char   uc_tmp = 0;
  unsigned short  us_tmp = 0;
  unsigned int    ui_tmp = 0;

  bmod = (DsmccBmod*)malloc (sizeof (DsmccBmod));
  if (!bmod)
    {
      DSMCC_MERR ("(0x%04x)", mid);
      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_BMOD_SIZE + mldlen + mcpdlen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%04x)", mid);
      return 0;
    }

  us_tmp = mid;
  dsmcc_write_2 (ret, &pos, us_tmp);

  ui_tmp = msize;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = mver;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_BMOD_INFO_LEN + mldlen + mcpdlen;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  ui_tmp = DSMCC_BMOD_INFO_MOD_TIMEOUT;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = DSMCC_BMOD_INFO_BLOCK_TIMEOUT;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = DSMCC_BMOD_INFO_MIN_BLOCK_TIME;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = DSMCC_BMOD_INFO_TAPS_COUNT;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  us_tmp = DSMCC_BMOD_INFO_TAP_ID;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = DSMCC_BMOD_INFO_TAP_USE;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = ctag;
  dsmcc_write_2 (ret, &pos, us_tmp);

  uc_tmp = DSMCC_BMOD_INFO_TAP_SEL_LEN;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = mldlen + mcpdlen;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  if (mldlen > 0)
    {
      memcpy (&ret[pos], mld, mldlen);
      pos += mldlen;
    }

  if (mcpdlen > 0)
    {
      memcpy (&ret[pos], mcpd, mcpdlen);
      pos += mcpdlen;
    }

  bmod->bmod = ret;
  bmod->size = pos;
  bmod->mid  = mid;
  bmod->len  = msize;
  bmod->ver  = mver;
  bmod->ctag = ctag;

  return bmod;
}


void
dsmcc_bmod_free (DsmccBmod *bmod)
{
  if (bmod->bmod)
    {
      free (bmod->bmod);
    }

  free (bmod);
}


unsigned int
dsmcc_bmod_dump_write (DsmccBmod *bmod,
                       int        fd)
{
  if (write (fd, &bmod->mid, 2) != 2)
    {
      return 0;
    }

  if (write (fd, &bmod->len, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &bmod->ver, 1) != 1)
    {
      return 0;
    }

  if (write (fd, &bmod->ctag, 1) != 1)
    {
      return 0;
    }

  return 1;
}


DsmccBmod *
dsmcc_bmod_dump_read (int fd)
{
  unsigned short mid  = 0;
  unsigned int   len  = 0;
  unsigned char  ver  = 0;
  unsigned char  ctag = 0;

  if (read (fd, &mid, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &len, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &ver, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &ctag, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  return dsmcc_bmod_new (mid, len, ver, ctag, 0, 0, 0, 0);
}
