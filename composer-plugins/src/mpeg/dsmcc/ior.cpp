// interoperable object reference

#include "../../../include/mpeg/dsmcc/ior.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/bpro.h"
#include "../../../include/mpeg/dsmcc/lpro.h"


unsigned char *
dsmcc_ior_new (ObjectType      type,
               unsigned int    tid,
               unsigned short  atag,
               unsigned int    key,
               unsigned short  mid,
               unsigned int    cid,
               unsigned int   *len)
{
  unsigned char *ret     = 0;
  unsigned int   pos     = 0;
  unsigned int   ui_tmp  = 0;
  unsigned int   o_type  = 0;
  unsigned int   bprolen = 0;
  unsigned char *bpro    = 0;

  switch (type)
    {
      case ot_fil:
        o_type = DSMCC_DATA_FIL;
        break;
      case ot_dir:
        o_type = DSMCC_DATA_DIR;
        break;
      case ot_str:
        o_type = DSMCC_DATA_STR;
        break;
      case ot_ste:
        o_type = DSMCC_DATA_STE;
        break;
      case ot_srg:
        o_type = DSMCC_DATA_SRG;
        break;
      default:
        DSMCC_MERR ("(0x%08x) (%s) (0x%04x)",
                    key, dsmcc_type_string (type), mid);
        return 0;
  }

  bpro = dsmcc_bpro_new (tid, atag, key, mid, cid, &bprolen);
  if (!bpro)
    {
      DSMCC_MERR ("(0x%08x) (%s) (0x%04x)",
                  key, dsmcc_type_string (type), mid);
      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_IOR_SIZE + bprolen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (%s) (0x%04x)",
                  key, dsmcc_type_string (type), mid);
      free (bpro);

      return 0;
    }

  ui_tmp = DSMCC_IOR_TYPE_ID_LEN;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = o_type;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = DSMCC_IOR_PROF_COUNT;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  memcpy (&ret[pos], bpro, bprolen);
  pos += bprolen;

  (*len) = pos;

  free (bpro);

  return ret;
}


unsigned char *
dsmcc_ior_new_ext (ObjectType    type,
                   DsmccExt     *ext,
                   unsigned int *len)
{
  unsigned char *ret     = 0;
  unsigned int   pos     = 0;
  unsigned int   ui_tmp  = 0;
  unsigned int   o_type  = 0;
  unsigned int   lprolen = 0;
  unsigned char *lpro    = 0;

  switch (type)
    {
      case ot_fil:
        o_type = DSMCC_DATA_FIL;
        break;
      case ot_dir:
        o_type = DSMCC_DATA_DIR;
        break;
      case ot_str:
        o_type = DSMCC_DATA_STR;
        break;
      case ot_ste:
        o_type = DSMCC_DATA_STE;
        break;
      case ot_srg:
        o_type = DSMCC_DATA_SRG;
        break;
      default:
        DSMCC_MERR ("(%s)", dsmcc_type_string (type));
        return 0;
  }

  lpro = dsmcc_lpro_new (type, ext->path, ext->cid, ext->onid,
                         ext->tsid, ext->sid, &lprolen);
  if (!lpro)
    {
      DSMCC_MERR ("(%s)", dsmcc_type_string (type));
      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_IOR_SIZE + lprolen);
  if (!ret)
    {
      DSMCC_MERR ("(%s)", dsmcc_type_string (type));
      free (lpro);

      return 0;
    }

  ui_tmp = DSMCC_IOR_TYPE_ID_LEN;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = o_type;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = DSMCC_IOR_PROF_COUNT;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  memcpy (&ret[pos], lpro, lprolen);
  pos += lprolen;

  (*len) = pos;

  free (lpro);

  return ret;
}
