// download info indication

#include "../../../include/mpeg/dsmcc/dii.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/bmod.h"
#include "../../../include/mpeg/dsmcc/ddb.h"
#include "../../../include/mpeg/dsmcc/mhdr.h"
#include "../../../include/mpeg/dsmcc/util.h"


DsmccDii *
dsmcc_dii_new (unsigned int  ver,
               unsigned int  id,
               unsigned int  up,
               unsigned int  did,
               DsmccList    *mods)
{
  DsmccDii       *dii     = 0;
  unsigned char  *ret     = 0;
  unsigned int    pos     = 0;
  unsigned char   uc_tmp  = 0;
  unsigned short  us_tmp  = 0;
  unsigned int    ui_tmp  = 0;
  unsigned int    tid     = 0;
  unsigned int    mhdrlen = 0;
  unsigned char  *mhdr    = 0;
  unsigned short  modslen = 0;
  DsmccBmod      *bmod    = 0;
  DsmccLink      *link    = 0;

  if (dsmcc_list_size (mods) == 0)
    {
      DSMCC_ERR ();
      return 0;
    }

  dsmcc_list_foreach (mods, link)
    {
      bmod = (DsmccBmod*)link->data;
      modslen += bmod->size;
    }

  tid = dsmcc_tid (ver, id, up);

  mhdr = dsmcc_mhdr_new (DSMCC_DII_MSID, tid, DSMCC_DII_SIZE + modslen, &mhdrlen);
  if (!mhdr)
    {
      DSMCC_MERR ("(0x%08x) (0x%08x)", tid, did);
      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_DII_SIZE + mhdrlen + modslen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%08x)", tid, did);
      free (mhdr);

      return 0;
    }

  memcpy (&ret[pos], mhdr, mhdrlen);
  pos += mhdrlen;

  ui_tmp = did;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  us_tmp = DSMCC_DDB_MAX_SIZE;
  dsmcc_write_2 (ret, &pos, us_tmp);

  uc_tmp = DSMCC_DII_WIN_SIZE;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_DII_ACK_PERIOD;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  ui_tmp = DSMCC_DII_TC_DOWN_WIN;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = DSMCC_DII_TC_DOWN_SCEN;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  us_tmp = DSMCC_DII_COMPAT_DESC;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = dsmcc_list_size (mods);
  dsmcc_write_2 (ret, &pos, us_tmp);

  dsmcc_list_foreach (mods, link)
    {
      bmod = (DsmccBmod*)link->data;
      memcpy (&ret[pos], bmod->bmod, bmod->size);
      pos += bmod->size;
    }

  us_tmp = DSMCC_DII_PRIV_LEN;
  dsmcc_write_2 (ret, &pos, us_tmp);

  free (mhdr);

  dii = (DsmccDii*)malloc (sizeof (DsmccDii));
  if (!dii)
    {
      DSMCC_MERR ("(0x%08x) (0x%08x)", tid, did);
      free (ret);

      return 0;
    }

  dii->dii  = ret;
  dii->size = pos;
  dii->ver  = ver;
  dii->id   = id;
  dii->up   = up;
  dii->did  = did;
  dii->mods = mods;
  dii->mlen = 0;
  dii->upd  = 0;

  return dii;
}


void
dsmcc_dii_free (DsmccDii *dii)
{
  if (dii->dii)
    {
      free (dii->dii);
    }

  free (dii);
}


unsigned int
dsmcc_dii_dump_write (DsmccDii *dii,
                      int       fd)
{
  DsmccLink      *link   = 0;
  unsigned short  us_tmp = dsmcc_list_size (dii->mods);

  if (write (fd, &dii->ver, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &dii->id, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &dii->up, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &dii->did, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &us_tmp, 2) != 2)
    {
      return 0;
    }

  dsmcc_list_foreach (dii->mods, link)
    {
      if (!dsmcc_bmod_dump_write ((DsmccBmod*)link->data, fd))
        {
          return 0;
        }
    }

  return 1;
}


DsmccDii *
dsmcc_dii_dump_read (int fd)
{
  DsmccList      *mods = dsmcc_list_new ();
  unsigned int    ver  = 0;
  unsigned int    id   = 0;
  unsigned int    up   = 0;
  unsigned int    did  = 0;
  unsigned short  num  = 0;
  DsmccBmod      *mod  = 0;
  int             i    = 0;

  if (read (fd, &ver, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &id, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &up, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &did, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &num, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  for (i = 0; i < num; i++)
    {
      mod = dsmcc_bmod_dump_read (fd);
      if (!mod)
        {
          DSMCC_ERR ();
          return 0;
        }

      dsmcc_list_push_back (mods, mod);
    }

  return dsmcc_dii_new (ver, id, up, did, mods);
}
