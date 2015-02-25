// biop stream message

#include "../../../include/mpeg/dsmcc/str.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/bhdr.h"
#include "../../../include/mpeg/dsmcc/dtap.h"
#include "../../../include/mpeg/dsmcc/stri.h"


DsmccStr *
dsmcc_str_new (char          *path,
               unsigned int   key,
               unsigned char *info,
               unsigned int   ilen,
               unsigned int   dsec,
               unsigned int   dmsec,
               unsigned char  dmask,
               unsigned char  bptag,
               unsigned char  npttag,
               unsigned char  nptbase)
{
  DsmccStr       *ret       = 0;
  unsigned char  *str       = 0;
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
  unsigned char   tapcount  = 0;

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

  bhdr = dsmcc_bhdr_new (key, ot_str, DSMCC_STR_SIZE + strilen + bptaplen + npttaplen, &bhdrlen);
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
      free (stri);

      return 0;
    }

  str = (unsigned char*)malloc (DSMCC_STR_SIZE + bhdrlen + strilen + bptaplen + npttaplen);
  if (!str)
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
      free (stri);

      return 0;
    }

  memcpy (&str[pos], bhdr, bhdrlen);
  pos += bhdrlen;

  us_tmp = strilen;
  dsmcc_write_2 (str, &pos, us_tmp);

  memcpy (&str[pos], stri, strilen);
  pos += strilen;

  uc_tmp = DSMCC_STR_SCL_COUNT;
  dsmcc_write_1 (str, &pos, uc_tmp);

  ui_tmp = bptaplen + npttaplen + 1;
  dsmcc_write_4 (str, &pos, ui_tmp);

  uc_tmp = tapcount;
  dsmcc_write_1 (str, &pos, uc_tmp);

  if (bptap)
    {
      memcpy (&str[pos], bptap, bptaplen);
      pos += bptaplen;
    }

  if (npttap)
    {
      memcpy (&str[pos], npttap, npttaplen);
      pos += npttaplen;
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
  free (stri);

  ret = (DsmccStr*)malloc (sizeof (DsmccStr));
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      free (str);

      return 0;
    }

  ret->key     = key;
  ret->bptag   = bptag;
  ret->npttag  = npttag;
  ret->nptbase = nptbase;
  ret->str     = str;
  ret->size    = pos;
  ret->path    = path;

  return ret;
}


void
dsmcc_str_free (DsmccStr *str)
{
  if (str->str)
    {
      free (str->str);
    }

  free (str);
}
