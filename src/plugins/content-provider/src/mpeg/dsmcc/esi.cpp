// elementary stream info

#include "../../../include/mpeg/dsmcc/esi.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccEsi *
dsmcc_esi_new (DsmccEsis *esis)
{
  DsmccEsi *ret = 0;

  ret = new DsmccEsi;
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->pid  = esis->pid;
  ret->type = esis->type;
  ret->size = DSMCC_ESI_SIZE;
  ret->data = 0;
  ret->asd  = 0;
  ret->dbid = 0;
  ret->sid  = 0;
  ret->cid  = 0;

  ret->sid = dsmcc_sid_new (esis->ctag);
  if (!ret->sid)
    {
      DSMCC_ERR ();
      free (ret);
      return 0;
    }

  if (esis->type == st_oc)
    {
      ret->cid = dsmcc_cid_new (esis->cid, 0, 0, 0, 0, 0, 0, 0);
      if (!ret->cid)
        {
          DSMCC_ERR ();
          free (ret);
          return 0;
        }
    }
  else if (esis->type == st_ait)
    {
      ret->dbid = dsmcc_dbid_new (DSMCC_DBID_OC, esis->atype);
      if (!ret->dbid)
        {
          DSMCC_ERR ();
          free (ret);
          return 0;
        }

      ret->asd = dsmcc_asd_new ();
      if (!ret->asd)
        {
          DSMCC_ERR ();
          free (ret);
          return 0;
        }
    }

  return ret;
}


void
dsmcc_esi_free (DsmccEsi *esi)
{
  if (esi->asd)
    {
      dsmcc_asd_free (esi->asd);
    }

  if (esi->dbid)
    {
      dsmcc_dbid_free (esi->dbid);
    }

  if (esi->sid)
    {
      dsmcc_sid_free (esi->sid);
    }

  if (esi->cid)
    {
      dsmcc_cid_free (esi->cid);
    }

  if (esi->data)
    {
      free (esi->data);
    }

  free (esi);
}


unsigned int
dsmcc_esi_add_app (DsmccEsi *esi,
                   DsmccAsd *asd)
{
  if (esi->asd)
    {
      return 0;
    }

  esi->asd = asd;

  return 1;
}


unsigned int
dsmcc_esi_add_id (DsmccEsi  *esi,
                  DsmccDbid *dbid)
{
  if (esi->dbid)
    {
      return 0;
    }

  esi->dbid = dbid;

  return 1;
}


unsigned int
dsmcc_esi_add_sid (DsmccEsi *esi,
                   DsmccSid *sid)
{
  if (esi->sid)
    {
      return 0;
    }

  esi->sid = sid;

  return 1;
}


unsigned int
dsmcc_esi_add_cid (DsmccEsi *esi,
                   DsmccCid *cid)
{
  if (esi->cid)
    {
      return 0;
    }

  esi->cid = cid;

  return 1;
}


unsigned int
dsmcc_esi_encode (DsmccEsi *esi)
{
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;
  unsigned short us_tmp = 0;
  unsigned char  stype  = 0;
  unsigned short esilen = 0;

  switch (esi->type)
    {
      case st_oc:
        stype = DSMCC_ESI_ST_OC;
        break;
      case st_ste:
        stype = DSMCC_ESI_ST_STE;
        break;
      case st_ait:
        stype = DSMCC_ESI_ST_AIT;
        break;
      default:
        stype = esi->type;
    }

  if (esi->asd)
    {
      esi->size += esi->asd->size;
      esilen    += esi->asd->size;
    }

  if (esi->dbid)
    {
      esi->size += esi->dbid->size;
      esilen    += esi->dbid->size;
    }

  if (esi->sid)
    {
      esi->size += esi->sid->size;
      esilen    += esi->sid->size;
    }

  if (esi->cid)
    {
      esi->size += esi->cid->size;
      esilen    += esi->cid->size;
    }

  esi->data = (unsigned char*)malloc (esi->size);
  if (!esi->data)
    {
      DSMCC_ERR ();
      return 0;
    }

  uc_tmp = stype;
  dsmcc_write_1 (esi->data, &pos, uc_tmp);

  us_tmp = (esi->pid | DSMCC_ESI_PID_MASK);
  dsmcc_write_2 (esi->data, &pos, us_tmp);

  us_tmp = (esilen | DSMCC_ESI_ESI_MASK);
  dsmcc_write_2 (esi->data, &pos, us_tmp);

  if (esi->asd)
    {
      memcpy (&esi->data[pos], esi->asd->data, esi->asd->size);
      pos += esi->asd->size;
    }

  if (esi->dbid)
    {
      memcpy (&esi->data[pos], esi->dbid->data, esi->dbid->size);
      pos += esi->dbid->size;
    }

  if (esi->sid)
    {
      memcpy (&esi->data[pos], esi->sid->data, esi->sid->size);
      pos += esi->sid->size;
    }

  if (esi->cid)
    {
      memcpy (&esi->data[pos], esi->cid->data, esi->cid->size);
      pos += esi->cid->size;
    }

  return 1;
}
