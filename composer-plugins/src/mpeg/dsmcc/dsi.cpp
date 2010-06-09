// download server initiate

#include "../../../include/mpeg/dsmcc/dsi.h"

#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/mhdr.h"
#include "../../../include/mpeg/dsmcc/srg.h"
#include "../../../include/mpeg/dsmcc/util.h"


DsmccDsi *
dsmcc_dsi_new (unsigned int   ver,
               unsigned int   id,
               unsigned int   up,
               unsigned short atag,
               unsigned int   key,
               unsigned short mid,
               unsigned int   cid,
               unsigned int   rtid)
{
  DsmccDsi       *dsi     = 0;
  unsigned char  *ret     = 0;
  unsigned int    pos     = 0;
  unsigned char   uc_tmp  = 0;
  unsigned short  us_tmp  = 0;
  unsigned int    tid     = 0;
  unsigned int    i       = 0;
  unsigned int    mhdrlen = 0;
  unsigned char  *mhdr    = 0;
  unsigned int    srglen  = 0;
  unsigned char  *srg     = 0;

  tid = dsmcc_tid (ver, id, up);

  srg = dsmcc_srg_new (ot_srg, rtid, atag, key, mid, cid, &srglen);
  if (!srg)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", key, mid);
      return 0;
    }

  mhdr = dsmcc_mhdr_new (DSMCC_DSI_MSID, tid, DSMCC_DSI_SIZE + srglen, &mhdrlen);
  if (!mhdr)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", key, mid);
      free (srg);

      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_DSI_SIZE + mhdrlen + srglen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", key, mid);
      free (mhdr);
      free (srg);

      return 0;
    }

  memcpy (&ret[pos], mhdr, mhdrlen);
  pos += mhdrlen;

  uc_tmp = DSMCC_DSI_SERVER_ID_BYTE;
  for (i = 0; i < DSMCC_DSI_SERVER_ID_LEN; i++)
    {
      dsmcc_write_1 (ret, &pos, uc_tmp);
    }

  us_tmp = DSMCC_DSI_COMPAT_DESC;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = srglen;
  dsmcc_write_2 (ret, &pos, us_tmp);

  memcpy (&ret[pos], srg, srglen);
  pos += srglen;

  free (mhdr);
  free (srg);

  dsi = (DsmccDsi*)malloc (sizeof (DsmccDsi));
  if (!dsi)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", key, mid);
      free (ret);

      return 0;
    }

  dsi->dsi  = ret;
  dsi->size = pos;
  dsi->ver  = ver;
  dsi->id   = id;
  dsi->up   = up;
  dsi->atag = atag;
  dsi->key  = key;
  dsi->mid  = mid;
  dsi->cid  = cid;

  return dsi;
}


void
dsmcc_dsi_free (DsmccDsi *dsi)
{
  if (dsi->dsi)
    {
      free (dsi->dsi);
    }

  free (dsi);
}


unsigned int
dsmcc_dsi_dump_write (DsmccDsi *dsi,
                      int       fd,
                      int       root)
{
  if (write (fd, &root, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (!root)
    {
      return 1;
    }

  if (write (fd, &dsi->ver, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (write (fd, &dsi->id, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (write (fd, &dsi->up, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (write (fd, &dsi->atag, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (write (fd, &dsi->key, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (write (fd, &dsi->mid, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (write (fd, &dsi->cid, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  return 1;
}


DsmccDsi *
dsmcc_dsi_dump_read (int fd)
{
  DsmccDsi      *dsi  = 0;
  unsigned int   ver  = 0;
  unsigned int   id   = 0;
  unsigned int   up   = 0;
  unsigned short atag = 0;
  unsigned int   key  = 0;
  unsigned short mid  = 0;
  unsigned int   cid  = 0;
  unsigned int   root = 0;

  if (read (fd, &root, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (!root)
    {
      dsi = dsmcc_dsi_new (ver, id, up, atag, key, mid, cid, 0);
      dsi->size = 0;
      return dsi;
    }

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

  if (read (fd, &atag, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &key, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &mid, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &cid, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  return dsmcc_dsi_new (ver, id, up, atag, key, mid, cid, 0);
}
