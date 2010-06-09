// dsm service location

#include "../../../include/mpeg/dsmcc/sloc.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/cnam.h"
#include "../../../include/mpeg/dsmcc/list.h"
#include "../../../include/mpeg/dsmcc/nsap.h"


unsigned char *
dsmcc_sloc_new (ObjectType      type,
                char           *path,
                unsigned int    cid,
                unsigned short  onid,
                unsigned short  tsid,
                unsigned short  sid,
                unsigned int   *len)
{
  unsigned char  *ret      = 0;
  unsigned int    pos      = 0;
  unsigned char   uc_tmp   = 0;
  unsigned int    ui_tmp   = 0;
  unsigned char  *nsap     = 0;
  unsigned int    nsaplen  = 0;
  DsmccCnam      *cnam     = 0;
  DsmccList      *cnams    = 0;
  DsmccLink      *link     = 0;
  unsigned int    cnamslen = 0;
  char           *comps[64];
  unsigned int    nr_comps = 0;
  unsigned int    i        = 0;
  ObjectType      ctype    = ot_dir;

  nsap = dsmcc_nsap_new (cid, onid, tsid, sid, &nsaplen);
  if (!nsap)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (0x%08x) (0x%04x) (%s) (%s)",
                  cid, onid, tsid, sid, dsmcc_type_string (type), path);
      return 0;
    }

  cnams = dsmcc_list_new ();
  if (!cnams)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (0x%08x) (0x%04x) (%s) (%s)",
                  cid, onid, tsid, sid, dsmcc_type_string (type), path);
      free (nsap);
      return 0;
    }

#ifdef WIN32
  nr_comps = dsmcc_get_comp (path, comps, "\\");
#else
  nr_comps = dsmcc_get_comp (path, comps, "/");
#endif

  if (!nr_comps)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (0x%08x) (0x%04x) (%s) (%s)",
                  cid, onid, tsid, sid, dsmcc_type_string (type), path);
      free (nsap);
      dsmcc_list_free (cnams, (FreeFunc) dsmcc_cnam_free);
      return 0;
    }

  for (i = 0; i < nr_comps; i++)
    {
      if (i == (nr_comps - 1))
        {
          ctype = type;
        }

      cnam = dsmcc_cnam_new (ctype, comps[i]);
      if (!cnam)
        {
          DSMCC_MERR ("(0x%08x) (0x%04x) (0x%08x) (0x%04x) (%s) (%s)",
                      cid, onid, tsid, sid, dsmcc_type_string (type), path);
          free (nsap);
          dsmcc_list_free (cnams, (FreeFunc) dsmcc_cnam_free);
          return 0;
        }

      dsmcc_list_push_back (cnams, cnam);

      cnamslen += cnam->size;
    }

  ret = (unsigned char*)malloc (DSMCC_SLOC_SIZE + cnamslen + nsaplen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (0x%08x) (0x%04x) (%s) (%s)",
                  cid, onid, tsid, sid, dsmcc_type_string (type), path);
      free (nsap);
      dsmcc_list_free (cnams, (FreeFunc) dsmcc_cnam_free);
      return 0;
    }

  ui_tmp = DSMCC_SLOC_TAG;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = cnamslen + nsaplen;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = nsaplen;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  memcpy (&ret[pos], nsap, nsaplen);
  pos += nsaplen;

  free (nsap);

  ui_tmp = dsmcc_list_size (cnams);
  dsmcc_write_4 (ret, &pos, ui_tmp);

  dsmcc_list_foreach (cnams, link)
    {
      cnam = (DsmccCnam*)link->data;

      memcpy (&ret[pos], cnam->cnam, cnam->size);
      pos += cnam->size;
    }

  dsmcc_list_free (cnams, (FreeFunc) dsmcc_cnam_free);

  ui_tmp = DSMCC_SLOC_CTX_LEN;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  (*len) = pos;

  return ret;
}
