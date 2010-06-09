// module section management

#include "../../../include/mpeg/dsmcc/msec.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/conf.h"
#include "../../../include/mpeg/dsmcc/ddb.h"
#include "../../../include/mpeg/dsmcc/sec.h"


DsmccMsec *
dsmcc_msec_new (DsmccMod *mod)
{
  DsmccMsec *ret = 0;

  ret = (DsmccMsec*)malloc (sizeof (DsmccMsec));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->secs = dsmcc_list_new ();
  if (!ret->secs)
    {
      DSMCC_ERR ();
      free (ret);

      return 0;
    }

  ret->mod = mod;

  return ret;
}


void
dsmcc_msec_free (DsmccMsec *msec)
{
  if (msec->secs)
    {
      dsmcc_list_free (msec->secs, (FreeFunc) dsmcc_sec_free);
    }

  free (msec);
}


unsigned int 
dsmcc_msec_encode (DsmccMsec *msec)
{
  unsigned int    pos    = 0;
  unsigned short  dlen   = 0;
  unsigned short  nr     = 0;
  unsigned char  *ddb    = 0;
  unsigned int    ddblen = 0;
  DsmccSec       *sec    = 0;
  unsigned short  num    = 0;
  unsigned char  *mdata  = 0;
  char           *mpath  = 0;
  unsigned int    off    = 0;
  unsigned int    olen   = 0;
  unsigned int    odiff  = 0;
  unsigned char  *fhdr   = 0;

  num = msec->mod->size / DSMCC_DDB_MAX_SIZE;
  if (msec->mod->size % DSMCC_DDB_MAX_SIZE)
    {
      num++;
    }

  if (msec->mod->sfil)
    {
      mpath = msec->mod->sfil->path;
      odiff = (msec->mod->sfil->size - msec->mod->sfil->len);
      fhdr  = msec->mod->sfil->fil;
    }

  while (pos < msec->mod->size)
    {
      dlen = DSMCC_DDB_MAX_SIZE;
      if ((pos + dlen) > msec->mod->size)
        {
          dlen = msec->mod->size - pos;
        }

      mdata = 0;
      if (!msec->mod->sfil)
        {
          mdata = &msec->mod->data[pos];
        }

      ddb = dsmcc_ddb_new (msec->mod->did,
                           msec->mod->mid,
                           msec->mod->ver,
                           nr,
                           mdata,
                           dlen,
                           &ddblen);
      if (!ddb)
        {
          DSMCC_MERR ("(0x%04x) (%d) (%d) (%d)", msec->mod->mid, pos, num, nr);

          dsmcc_list_clear (msec->secs, free);
          return 0;
        }

      if (msec->mod->sfil)
        {
          olen = dlen;
          if (nr == 0)
            {
              olen -= odiff;
            }
          else
            {
              fhdr  = 0;
              odiff = 0;
            }
        }

      sec = dsmcc_sec_new (ot_fil,
                           msec->mod->mid,
                           (msec->mod->ver & 0x1F),
                           (nr % 256),
                           (num % 256) - 1,
                           ddb,
                           ddblen,
                           fhdr,
                           odiff,
                           mpath,
                           off,
                           olen);

      if (msec->mod->sfil)
        {
          off += olen;
        }

      if (!msec->mod->sfil)
        {
          free (ddb);
        }

      if (!sec)
        {
          DSMCC_MERR ("(0x%04x) (%d) (%d) (%d)", msec->mod->mid, pos, num, nr);

          dsmcc_list_clear (msec->secs, free);
          return 0;
        }

      dsmcc_list_push_back (msec->secs, sec);

      nr++;
      pos += dlen;
    }

  free (msec->mod->data);
  msec->mod->data = 0;

  DSMCC_MSG (" + created %d sections from module-id <0x%04x>.\n",
             dsmcc_list_size (msec->secs), msec->mod->mid);

  return 1;
}
