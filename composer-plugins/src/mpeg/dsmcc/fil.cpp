// biop file message

#include "../../../include/mpeg/dsmcc/fil.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/bhdr.h"


DsmccFil *
dsmcc_fil_new (char          *path,
               unsigned int   key,
               unsigned char *data,
               unsigned int   dlen,
               unsigned int   sin)
{
  DsmccFil       *ret     = 0;
  unsigned char  *fil     = 0;
  unsigned int    pos     = 0;
  unsigned char   uc_tmp  = 0;
  unsigned short  us_tmp  = 0;
  unsigned int    ui_tmp  = 0;
  unsigned int    bhdrlen = 0;
  unsigned char  *bhdr    = 0;
  unsigned int    size    = dlen;

  if (sin)
    {
      size = 0;
    }

  bhdr = dsmcc_bhdr_new (key, ot_fil, DSMCC_FIL_SIZE + dlen, &bhdrlen);
  if (!bhdr)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      return 0;
    }

  fil = (unsigned char*)malloc (DSMCC_FIL_SIZE + bhdrlen + size);
  if (!fil)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      free (bhdr);

      return 0;
    }

  memcpy (&fil[pos], bhdr, bhdrlen);
  pos += bhdrlen;

  us_tmp = DSMCC_FIL_OBJ_INFO_LEN;
  dsmcc_write_2 (fil, &pos, us_tmp);

  ui_tmp = 0;
  dsmcc_write_4 (fil, &pos, ui_tmp);

  ui_tmp = dlen;
  dsmcc_write_4 (fil, &pos, ui_tmp);

  uc_tmp = DSMCC_FIL_SCL_COUNT;
  dsmcc_write_1 (fil, &pos, uc_tmp);

  ui_tmp = dlen + DSMCC_FIL_DATA_OFF;
  dsmcc_write_4 (fil, &pos, ui_tmp);

  ui_tmp = dlen;
  dsmcc_write_4 (fil, &pos, ui_tmp);

  if (dlen > 0 && !sin)
    {
      memcpy (&fil[pos], data, dlen);
    }

  pos += dlen;

  free (bhdr);

  ret = (DsmccFil*)malloc (sizeof (DsmccFil));
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      free (fil);

      return 0;
    }

  ret->key  = key;
  ret->fil  = fil;
  ret->size = pos;
  ret->len  = dlen;
  ret->path = path;

  return ret;
}


void
dsmcc_fil_free (DsmccFil *fil)
{
  if (fil->fil)
    {
      free (fil->fil);
    }

  if (fil->path)
    {
      free (fil->path);
    }

  free (fil);
}
