// biop stream event message

#include "../../../include/mpeg/dsmcc/ste.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/bhdr.h"
#include "../../../include/mpeg/dsmcc/dtap.h"
#include "../../../include/mpeg/dsmcc/stel.h"
#include "../../../include/mpeg/dsmcc/stri.h"


DsmccSte *
dsmcc_ste_new (char           *path,
               unsigned int    key,
               unsigned char  *info,
               unsigned int    ilen,
               unsigned int    dsec,
               unsigned int    dmsec,
               unsigned char   dmask,
               unsigned char   bptag,
               unsigned char   npttag,
               unsigned char   nptbase,
               unsigned char   stetag,
               DsmccList      *names,
               unsigned short *ids)
{
  DsmccSte       *ret       = 0;
  unsigned char  *ste       = 0;
  unsigned int    pos       = 0;
  unsigned char   uc_tmp    = 0;
  unsigned short  us_tmp    = 0;
  unsigned int    ui_tmp    = 0;
  unsigned int    bhdrlen   = 0;
  unsigned char  *bhdr      = 0;
  unsigned int    strilen   = 0;
  unsigned char  *stri      = 0;
  unsigned int    bptaplen  = 0;
  unsigned char  *bptap     = 0;
  unsigned int    npttaplen = 0;
  unsigned char  *npttap    = 0;
  unsigned int    stetaplen = 0;
  unsigned char  *stetap    = 0;
  unsigned char   tapcount  = 0;
  unsigned int    stellen   = 0;
  unsigned char  *stel      = 0;
  unsigned char   idslen    = dsmcc_list_size (names);
  unsigned int    i         = 0;

  stri = dsmcc_stri_new (info, ilen, dsec, dmsec, dmask, &strilen);
  if (!stri)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      return 0;
    }

  if (bptag > 0)
    {
      bptap = dsmcc_dtap_new (0, DSMCC_DTAP_USE_BIOP_PROG, bptag, &bptaplen);
      if (!bptap)
        {
          DSMCC_MERR ("(0x%08x) (%s)", key, path);
          free (stri);

          return 0;
        }

      tapcount++;
    }

  if (npttag > 0)
    {
      npttap = dsmcc_dtap_new (nptbase, DSMCC_DTAP_USE_STR_NPT, npttag, &npttaplen);
      if (!npttap)
        {
          DSMCC_MERR ("(0x%08x) (%s)", key, path);
          if (bptap)
            {
              free (bptap);
            }
          free (stri);

          return 0;
        }

      tapcount++;
    }

  if (stetag > 0)
    {
      stetap = dsmcc_dtap_new (0, DSMCC_DTAP_USE_STR_EV, stetag, &stetaplen);
      if (!stetap)
        {
          DSMCC_MERR ("(0x%08x) (%s)", key, path);
          if (bptap)
            {
              free (bptap);
            }
          if (npttap)
            {
              free (npttap);
            }
          free (stri);

          return 0;
        }

      tapcount++;
    }

  bhdr = dsmcc_bhdr_new (key, ot_ste, DSMCC_STE_SIZE + strilen + bptaplen +
                         npttaplen + stetaplen + stellen + idslen * 2, &bhdrlen);
  if (!bhdr)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      if (bptap)
        {
          free (bptap);
        }
      if (npttap)
        {
          free (npttap);
        }
      if (stetap)
        {
          free (stetap);
        }
      free (stri);

      return 0;
    }

  if (idslen > 0)
    {
      stel = dsmcc_stel_new (names, &stellen);
      if (!stel)
        {
          DSMCC_MERR ("(0x%08x) (%s)", key, path);
          free (bhdr);
          if (bptap)
            {
              free (bptap);
            }
          if (npttap)
            {
              free (npttap);
            }
          if (stetap)
            {
              free (stetap);
            }
          free (stri);

          return 0;
        }
    }

  ste = (unsigned char*)malloc (DSMCC_STE_SIZE + bhdrlen + strilen + bptaplen +
                npttaplen + stetaplen + stellen + idslen * 2);
  if (!ste)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      if (stel)
        {
          free (stel);
        }
      free (bhdr);
      if (bptap)
        {
          free (bptap);
        }
      if (npttap)
        {
          free (npttap);
        }
      if (stetap)
        {
          free (stetap);
        }
      free (stri);

      return 0;
    }

  memcpy (&ste[pos], bhdr, bhdrlen);
  pos += bhdrlen;

  us_tmp = strilen + stellen + 2;
  dsmcc_write_2 (ste, &pos, us_tmp);

  memcpy (&ste[pos], stri, strilen);
  pos += strilen;

  us_tmp = idslen;
  dsmcc_write_2 (ste, &pos, us_tmp);

  memcpy (&ste[pos], stel, stellen);
  pos += stellen;

  uc_tmp = DSMCC_STE_SCL_COUNT;
  dsmcc_write_1 (ste, &pos, uc_tmp);

  ui_tmp = bptaplen + npttaplen + stetaplen + idslen * 2 + 2;
  dsmcc_write_4 (ste, &pos, ui_tmp);

  uc_tmp = tapcount;
  dsmcc_write_1 (ste, &pos, uc_tmp);

  if (bptap)
    {
      memcpy (&ste[pos], bptap, bptaplen);
      pos += bptaplen;
    }

  if (npttap)
    {
      memcpy (&ste[pos], npttap, npttaplen);
      pos += npttaplen;
    }

  if (stetap)
    {
      memcpy (&ste[pos], stetap, stetaplen);
      pos += stetaplen;
    }

  uc_tmp = idslen;
  dsmcc_write_1 (ste, &pos, uc_tmp);

  for (i = 0; i < idslen; i++)
    {
      dsmcc_write_2 (ste, &pos, ids[i]);
    }

  if (stel)
    {
      free (stel);
    }
  free (bhdr);
  if (bptap)
    {
      free (bptap);
    }
  if (npttap)
    {
      free (npttap);
    }
  if (stetap)
    {
      free (stetap);
    }
  free (stri);

  ret = (DsmccSte*)malloc (sizeof (DsmccSte));
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      free (ste);

      return 0;
    }

  ret->key     = key;
  ret->bptag   = bptag;
  ret->npttag  = npttag;
  ret->nptbase = nptbase;
  ret->stetag  = stetag;
  ret->ste     = ste;
  ret->size    = pos;
  ret->path    = path;

  return ret;
}


void
dsmcc_ste_free (DsmccSte *ste)
{
  if (ste->ste)
    {
      free (ste->ste);
    }

  free (ste);
}
