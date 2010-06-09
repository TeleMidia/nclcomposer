// module management

#include "../../../include/mpeg/dsmcc/mods.h"

#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/mcpd.h"


DsmccMods *
dsmcc_mods_new (DsmccDcc *dcc)
{
  DsmccMods *ret = 0;

  ret = (DsmccMods*)malloc (sizeof (DsmccMods));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->mods = dsmcc_list_new ();
  if (!ret->mods)
    {
      DSMCC_ERR ();
      free (ret);

      return 0;
    }

  ret->stdb = dsmcc_stdb_new ();
  if (!ret->stdb)
    {
      DSMCC_ERR ();
      dsmcc_list_free (ret->mods, 0); 
      free (ret);

      return 0;
    }

  ret->dcc = dcc;
  ret->srg = 0;

  return ret;
}


void
dsmcc_mods_free (DsmccMods *mods)
{
  if (mods->srg)
    {
      dsmcc_mod_free (mods->srg);
    }

  if (mods->mods)
    {
      dsmcc_list_free (mods->mods, (FreeFunc) dsmcc_mod_free);
    }

  free (mods);
}


static unsigned int
dsmcc_mods_add (DsmccMods *mods,
                DsmccMod  *mod)
{
  DsmccLink *link = 0;
  DsmccMod  *tmod = 0;

  dsmcc_list_foreach (mods->mods, link)
    {
      tmod = (DsmccMod*)link->data;
      if (tmod == mod)
        {
          DSMCC_MWARN ("(0x%04x) (0x%04x)", tmod->mid, mod->mid);
          return 0;
        }
    }

  dsmcc_list_push_back (mods->mods, mod);

  return 1;
}


static unsigned int
dsmcc_mods_add_srg (DsmccMods *mods)
{
  DsmccMod     *srg  = 0;
  DsmccList    *list = dsmcc_list_new ();
  DsmccLink    *link = 0;
  DsmccEnt     *ent  = 0;
  unsigned int  size = 0;

  if (mods->dcc->exps->root)
    {
      srg = dsmcc_mod_new (dsmcc_conf_get_mid(),
                           dsmcc_conf_get_cid(),
                           dsmcc_conf_get_did(),
                           1,
                           1,
                           dsmcc_conf_get_tag(),
                           DSMCC_SRG_DEFAULT_LABEL,
                           DSMCC_MCPD_PRIO_HIGH,
                           DSMCC_MCPD_LEVEL_STATIC);
    }
  else
    {
      srg = dsmcc_mod_new (dsmcc_conf_get_mid(),
                           dsmcc_conf_get_cid(),
                           dsmcc_conf_get_did(),
                           1,
                           1,
                           dsmcc_conf_get_tag(),
                           0,
                           DSMCC_MCPD_PRIO_LOW,
                           DSMCC_MCPD_LEVEL_NONE);
    }

  if (!srg)
    {
      DSMCC_ERR ();
      dsmcc_list_free (list, 0);

      return 0;
    }

  dsmcc_list_foreach (mods->dcc->dirs, link)
    {
      ent = (DsmccEnt*)link->data;

      if ((size + ent->rlen) > DSMCC_MODULE_SIZE)
        {
          continue;
        }

      size += ent->rlen;
      dsmcc_list_push_back (list, ent);
    }

  dsmcc_list_foreach (list, link)
    {
      ent = (DsmccEnt*)link->data;

      if (!dsmcc_mod_add (srg, ent))
        {
          DSMCC_MERR ("(%s) (0x%04x)", ent->path, srg->mid);
          dsmcc_list_free (list, 0);

          return 0;
        }

      dsmcc_list_remove (mods->dcc->dirs, ent);
      dsmcc_list_push_back (mods->dcc->code, ent);
    }

  dsmcc_list_clear (list, 0);

  if (dsmcc_conf_get_srg_obj ())
    {
      dsmcc_list_foreach (mods->dcc->objs, link)
        {
          ent = (DsmccEnt*)link->data;

          if ((size + ent->rlen) > DSMCC_MODULE_SIZE)
            {
              continue;
            }

          size += ent->rlen;
          dsmcc_list_push_back (list, ent);
        }

      dsmcc_list_foreach (list, link)
        {
          ent = (DsmccEnt*)link->data;

          if (!dsmcc_mod_add (srg, ent))
            {
              DSMCC_MERR ("(%s) (0x%04x)", ent->path, srg->mid);
              dsmcc_list_free (list, 0);

              return 0;
            }

          dsmcc_list_remove (mods->dcc->objs, ent);
          dsmcc_list_push_back (mods->dcc->code, ent);
        }
    }

  if (mods->dcc->exps->root)
    {
      DSMCC_MSG (" + created service-gateway <0x%04x> with objects(%d) and size(%d)\n",
                 srg->mid, dsmcc_list_size (srg->ents), srg->size);
    }
  else
    {
      DSMCC_MSG (" + created module-id <0x%04x> with objects(%d) and size(%d)\n",
                 srg->mid, dsmcc_list_size (srg->ents), srg->size);
    }

  mods->srg = srg;

  dsmcc_list_clear (list, 0);
  list = dsmcc_list_concat (mods->dcc->dirs, mods->dcc->objs);
  dsmcc_list_clear (mods->dcc->dirs, 0);
  dsmcc_list_free (mods->dcc->objs, 0);
  mods->dcc->objs = list;

  return 1;
}


unsigned int 
dsmcc_mods_ents (DsmccMods *mods)
{
  DsmccLink    *link = 0;
  DsmccList    *list = dsmcc_list_new ();
  DsmccEnt     *ent  = 0;
  DsmccMod     *mod  = 0;
  float         size = 0;
  unsigned int  nr_o = 0;
  char          t[3] = "kb";

  if (!dsmcc_mods_add_srg (mods))
    {
      DSMCC_ERR ();
      dsmcc_list_free (list, 0);

      return 0;
    }

  while (dsmcc_list_size (mods->dcc->objs) > 0)
    {
      size = 0;
      nr_o = 0;

      mod = dsmcc_mod_new (dsmcc_conf_get_mid(),
                           dsmcc_conf_get_cid(),
                           dsmcc_conf_get_did(),
                           1,
                           1,
                           dsmcc_conf_get_tag(),
                           0,
                           DSMCC_MCPD_PRIO_LOW,
                           DSMCC_MCPD_LEVEL_NONE);
      if (!mod)
        {
          DSMCC_MERR ("(0x%04x)", mod->mid);
          dsmcc_list_free (list, 0);

          return 0;
        }

      dsmcc_list_foreach (mods->dcc->objs, link)
        {
          ent = (DsmccEnt*)link->data;

          if (size == 0 && ent->rlen > DSMCC_MODULE_SIZE)
            {
              size += ent->rlen;
              dsmcc_list_push_back (list, ent);
              break;
            }

          if ((size + ent->rlen) > DSMCC_MODULE_SIZE)
            {
              continue;
            }

          size += ent->rlen;

          dsmcc_list_push_back (list, ent);
        }

      while (dsmcc_list_size (list) > 0)
        {
          ent = (DsmccEnt*)dsmcc_list_pop_front (list);

          if (!dsmcc_mod_add (mod, ent))
            {
              DSMCC_MERR ("(%s) (0x%04x)", ent->path, mod->mid);
              dsmcc_list_free (list, 0);

              return 0;
            }

          dsmcc_list_push_back (mods->dcc->code, ent);
          dsmcc_list_remove (mods->dcc->objs, ent);

          nr_o++;
        }

      if (!dsmcc_mods_add (mods, mod))
        {
          DSMCC_MERR ("(0x%04x)", mod->mid);
          dsmcc_list_free (list, 0);

          return 0;
        }

      DSMCC_MSG (" + created module-id <0x%04x> with objects(%d) and size(%d)\n",
                 mod->mid, nr_o, mod->size);
    }

  dsmcc_list_free (list, 0);

  size = mods->srg->size;
  dsmcc_list_foreach (mods->mods, link)
    {
      mod = (DsmccMod*)link->data;

      size += mod->size;
    }

  size /= 1024;
  if (size >= 1024)
    {
      size /= 1024;
      strcpy (t, "mb");
    }

  DSMCC_MSG (" + created %d modules - total size %.02f %s.\n\n",
             dsmcc_list_size (mods->mods) + 1, size, t);

  return 1;
}


unsigned int
dsmcc_mods_encode (DsmccMods *mods)
{
  DsmccLink* link = 0;
  DsmccMod*  mod  = 0;

  if (!dsmcc_mod_encode (mods->srg, mods->stdb))
    {
      DSMCC_MERR ("(0x%04x)", mods->srg->mid);
      return 0;
    }

  if (mods->dcc->exps->root)
    {
      DSMCC_MSG (" + encoded service-gateway <0x%04x> with size(%d)\n\n",
                 mods->srg->mid, mods->srg->size);
    }
  else
    {
      DSMCC_MSG (" + encoded module-id <0x%04x> with size(%d)\n\n",
                 mods->srg->mid, mods->srg->size);
    }

  dsmcc_list_foreach (mods->mods, link)
    {
      mod = (DsmccMod*)link->data;

      if (!dsmcc_mod_encode (mod, mods->stdb))
        {
          DSMCC_MERR ("(0x%04x)", mod->mid);
          return 0;
        }

      DSMCC_MSG (" + encoded module-id <0x%04x> with size(%d)\n\n",
                 mod->mid, mod->size);
    }

  return 1;
}


unsigned int
dsmcc_mods_dump_write (DsmccMods *mods,
                 int        fd)
{
  unsigned short  modsnum = dsmcc_list_size (mods->mods) + 1;
  DsmccLink      *link    = 0;

  if (write (fd, &modsnum, 2) != 2)
    {
      return 0;
    }

  if (!dsmcc_mod_dump_write (mods->srg, fd))
    {
      return 0;
    }

  dsmcc_list_foreach (mods->mods, link)
    {
      if (!dsmcc_mod_dump_write ((DsmccMod*)link->data, fd))
        {
          return 0;
        }
    }

  if (!dsmcc_dcc_dump_write (mods->dcc, fd))
    {
      return 0;
    }

  return 1;
}


DsmccList *
dsmcc_mods_dump_read (int fd)
{
  DsmccList      *mods = dsmcc_list_new ();
  unsigned short  num  = 0;
  DsmccMod       *mod  = 0;
  int             i    = 0;

  if (read (fd, &num, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  for (i = 0; i < num; i++)
    {
      mod = dsmcc_mod_dump_read (fd);
      if (!mod)
        {
          DSMCC_ERR ();
          return 0;
        }

      dsmcc_list_push_back (mods, mod);
    }

  return mods;
}
