// initial biop object sizes

#include "../../../include/mpeg/dsmcc/size.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/bind.h"
#include "../../../include/mpeg/dsmcc/cnam.h"
#include "../../../include/mpeg/dsmcc/dir.h"
#include "../../../include/mpeg/dsmcc/ent.h"
#include "../../../include/mpeg/dsmcc/ext.h"
#include "../../../include/mpeg/dsmcc/fil.h"
#include "../../../include/mpeg/dsmcc/ste.h"
#include "../../../include/mpeg/dsmcc/str.h"
#include "../../../include/mpeg/dsmcc/util.h"


static unsigned int dsmcc_dir_size      = 0;
static unsigned int dsmcc_fil_size      = 0;
static unsigned int dsmcc_str_size      = 0;
static unsigned int dsmcc_ste_size      = 0;
static unsigned int dsmcc_bind_loc_size = 0;
static unsigned int dsmcc_bind_ext_size = 0;


unsigned int
dsmcc_size_init (void)
{
  DsmccExt  *ext  = 0;
  DsmccBind *bind = 0;
  DsmccDir  *dir  = 0;
  DsmccFil  *fil  = 0;
  DsmccStr  *str  = 0;
  DsmccSte  *ste  = 0;

  dir = dsmcc_dir_new (0, 0, 0, 0);
  if (!dir)
    {
      DSMCC_ERR ();
      return 0;
    }
  dsmcc_dir_size = dir->size;
  dsmcc_dir_free (dir);

  fil = dsmcc_fil_new (0, 0, 0, 0, 0);
  if (!fil)
    {
      DSMCC_ERR ();
      return 0;
    }
  dsmcc_fil_size = fil->size;
  dsmcc_fil_free (fil);

  str = dsmcc_str_new (0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  if (!str)
    {
      DSMCC_ERR ();
      return 0;
    }
  dsmcc_str_size = str->size;
  dsmcc_str_free (str);

  ste = dsmcc_ste_new (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  if (!ste)
    {
      DSMCC_ERR ();
      return 0;
    }
  dsmcc_ste_size = ste->size;
  dsmcc_ste_free (ste);

  bind = dsmcc_bind_new (ot_fil, "", 0, 0, 0, 0, 0, 0);
  if (!bind)
    {
      DSMCC_ERR ();
      return 0;
    }
  dsmcc_bind_loc_size = bind->size - 1;
  dsmcc_bind_free (bind);

  ext = dsmcc_ext_new (" ", " ", 0, 0, 0, 0);
  if (!ext)
    {
      DSMCC_ERR ();
      return 0;
    }

  bind = dsmcc_bind_new (ot_fil, "", 0, 0, 0, 0, 0, ext);
  if (!bind)
    {
      DSMCC_ERR ();
      return 0;
    }
  dsmcc_bind_ext_size = bind->size - DSMCC_CNAM_SIZE - 3;
  dsmcc_bind_free (bind);

  return 1;
}


static unsigned int
dsmcc_size_bind (char     *name,
                 DsmccExt *ext)
{
  unsigned int  len  = 0;
  unsigned int  nr_c = 0;
  unsigned int  i    = 0;
  char         *comps[64];

  if (name)
    {
      len = strlen (name) + 1;
    }

  if (ext)
    {
      len += dsmcc_bind_ext_size;

#ifdef WIN32
      nr_c = dsmcc_get_comp (ext->path, comps, "\\");
#else
      nr_c = dsmcc_get_comp (ext->path, comps, "/");
#endif
      for (i = 0; i < nr_c; i++)
        {
          len += DSMCC_CNAM_SIZE + strlen (comps[i]) + 1;
          free (comps[i]);
        }
    }
  else
    {
      len += dsmcc_bind_loc_size;
    }

  return len;
}


unsigned int
dsmcc_size_dir (DsmccList *ents)
{
  unsigned int  blen = 0;
  DsmccLink    *link = 0;
  DsmccEnt     *ent  = 0;

  if (ents)
    {
      dsmcc_list_foreach (ents, link)
        {
          ent = (DsmccEnt*)link->data;

          blen += dsmcc_size_bind (ent->name, ent->ext);
        }
    }

  return (dsmcc_dir_size + blen);
}


unsigned int
dsmcc_size_fil (unsigned int dlen)
{
  return (dsmcc_fil_size + dlen);
}


unsigned int
dsmcc_size_str (unsigned int dlen)
{
  return (dsmcc_str_size + dlen);
}


unsigned int
dsmcc_size_ste (unsigned int dlen)
{
  return (dsmcc_ste_size + dlen);
}
