// program map table

#include "../../../include/mpeg/dsmcc/pmt.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/crc.h"
#include "../../../include/mpeg/dsmcc/util.h"


static void
dsmcc_pmt_header (DsmccPmt      *pmt,
                  unsigned char *data,
                  unsigned int  *pos)
{
  unsigned char  uc_tmp = 0;
  unsigned short us_tmp = 0;

  uc_tmp = pmt->tid;
  dsmcc_write_1 (data, pos, uc_tmp);

  us_tmp = ((pmt->size - 3) | DSMCC_PMT_SIZE_MASK);
  dsmcc_write_2 (data, pos, us_tmp);

  us_tmp = pmt->prog;
  dsmcc_write_2 (data, pos, us_tmp);

  uc_tmp = ((pmt->ver << 1) | DSMCC_PMT_VER_MASK | pmt->valid);
  dsmcc_write_1 (data, pos, uc_tmp);

  uc_tmp = DSMCC_PMT_SEC_NUM;
  dsmcc_write_1 (data, pos, uc_tmp);

  uc_tmp = DSMCC_PMT_SEC_LAST;
  dsmcc_write_1 (data, pos, uc_tmp);

  us_tmp = (pmt->ppid | DSMCC_PMT_PPID_MASK);
  dsmcc_write_2 (data, pos, us_tmp);

  us_tmp = DSMCC_PMT_PINFO_LEN;
  dsmcc_write_2 (data, pos, us_tmp);
}


DsmccPmt *
dsmcc_pmt_new (unsigned char  valid,
               unsigned short prog,
               unsigned short ppid,
               unsigned char  ver)
{
  DsmccPmt *ret = 0;

  ret = new DsmccPmt;
  if (!ret)
    {
      DSMCC_MERR ("(%d) (0x%04x) (0x%04x) (0x%02x)",
                  valid, prog, ppid, ver);
      return 0;
    }

  ret->tid   = DSMCC_PMT_TID;
  ret->valid = valid;
  ret->prog  = prog;
  ret->ppid  = ppid;
  ret->ver   = ver;
  ret->size  = DSMCC_PMT_HEADER_SIZE + 4;
  ret->data  = 0;

  ret->esi = dsmcc_list_new ();
  if (!ret->esi)
    {
      DSMCC_MERR ("(%d) (0x%04x) (0x%04x) (0x%02x)",
                  valid, prog, ppid, ver);
      free (ret);
      return 0;
    }

  return ret;
}


void
dsmcc_pmt_free (DsmccPmt *pmt)
{
  if (pmt->esi)
    {
      dsmcc_list_free (pmt->esi, (FreeFunc) dsmcc_esi_free);
    }

  if (pmt->data)
    {
      free (pmt->data);
    }

  free (pmt);
}


void
dsmcc_pmt_add_esi (DsmccPmt *pmt,
                   DsmccEsi *esi)
{
  dsmcc_list_push_back (pmt->esi, esi);
}


unsigned int
dsmcc_pmt_encode (DsmccPmt *pmt)
{
  DsmccEsi       *esi    = 0;
  DsmccLink      *link   = 0;
  unsigned int    pos    = 0;
  unsigned int    ui_tmp = 0;
  unsigned short  esi_l  = 0;

  if (!pmt->esi)
    {
      DSMCC_MERR ("(%d) (0x%04x) (0x%04x) (0x%02x)",
                  pmt->valid, pmt->prog, pmt->ppid, pmt->ver);
      return 0;
    }

  dsmcc_list_foreach (pmt->esi, link)
    {
      esi = (DsmccEsi*)link->data;

      pmt->size += esi->size;
      esi_l     += esi->size;
    }

  pmt->data = (unsigned char*)malloc (pmt->size);
  if (!pmt->data)
    {
      DSMCC_MERR ("(%d) (0x%04x) (0x%04x) (0x%02x)",
                  pmt->valid, pmt->prog, pmt->ppid, pmt->ver);
      return 0;
    }

  dsmcc_pmt_header (pmt, pmt->data, &pos);

  dsmcc_list_foreach (pmt->esi, link)
    {
      esi = (DsmccEsi*)link->data;

      memcpy (&pmt->data[pos], esi->data, esi->size);
      pos += esi->size;
    }

  pmt->crc = dsmcc_crc ((char *) pmt->data, pos);

  ui_tmp = pmt->crc;
  dsmcc_write_4 (pmt->data, &pos, ui_tmp);

  pmt->len = pos;

  return 1;
}
