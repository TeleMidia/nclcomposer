// nptd.c - normal play time reference descriptor

#include "../../../include/mpeg/dsmcc/nptd.h"

#include <malloc.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccNptd *
dsmcc_nptd_new (unsigned int       npt,
                unsigned long long stc,
                unsigned char      base,
                unsigned short     num,
                unsigned short     den,
                unsigned char      dis)
{
  DsmccNptd     *ret    = 0;
  unsigned char *nptd   = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;
  unsigned short us_tmp = 0;
  unsigned int   ui_tmp = 0;
  unsigned int   stc_b  = 0;

  ret = (DsmccNptd*)(malloc (sizeof (DsmccNptd)));
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (%lld) (0x%02x) (0x%04x) (0x%04x)",
                  npt, stc, base, num, den);

      return 0;
    }

  ret->npt  = npt;
  ret->stc  = stc;
  ret->base = base;
  ret->num  = num;
  ret->den  = den;

  nptd = (unsigned char*)(malloc (DSMCC_NPTD_SIZE));
  if (!nptd)
    {
      DSMCC_MERR ("(0x%08x) (%lld) (0x%02x) (0x%04x) (0x%04x)",
                  npt, stc, base, num, den);

      free (ret);
      return 0;
    }

  uc_tmp = DSMCC_NPTD_TAG;
  dsmcc_write_1 (nptd, &pos, uc_tmp);

  uc_tmp = DSMCC_NPTD_SIZE - 2;
  dsmcc_write_1 (nptd, &pos, uc_tmp);

  uc_tmp = (base | (dis << 7));
  dsmcc_write_1 (nptd, &pos, uc_tmp);

  stc_b = ((stc >> 32) & 0x00000001);

  uc_tmp = ((DSMCC_NPTD_STC_RESERVED << 1) | (unsigned char) stc_b);
  dsmcc_write_1 (nptd, &pos, uc_tmp);

  ui_tmp = (stc & 0xFFFFFFFF);
  dsmcc_write_4 (nptd, &pos, ui_tmp);

  ui_tmp = DSMCC_NPTD_NPT_RESERVED;
  dsmcc_write_4 (nptd, &pos, ui_tmp);

  ui_tmp = npt;
  dsmcc_write_4 (nptd, &pos, ui_tmp);

  us_tmp = num;
  dsmcc_write_2 (nptd, &pos, us_tmp);

  us_tmp = den;
  dsmcc_write_2 (nptd, &pos, us_tmp);

  ret->nptd = nptd;
  ret->size = pos;

  return ret;
}


void
dsmcc_nptd_free (DsmccNptd *nptd)
{
  if (nptd->nptd)
    {
      free (nptd->nptd);
    }

  free (nptd);
}
