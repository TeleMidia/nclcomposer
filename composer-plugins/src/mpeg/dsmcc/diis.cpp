// download info indication management

#include "../../../include/mpeg/dsmcc/diis.h"

#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/bmod.h"
#include "../../../include/mpeg/dsmcc/dii.h"
#include "../../../include/mpeg/dsmcc/sec.h"
#include "../../../include/mpeg/dsmcc/util.h"


DsmccDiis *
dsmcc_diis_new (DsmccMods *mods)
{
  DsmccDiis *ret = 0;

  ret = (DsmccDiis*)malloc (sizeof (DsmccDiis));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->diis = dsmcc_list_new ();
  if (!ret->diis)
    {
      DSMCC_ERR ();
      free (ret);

      return 0;
    }

  ret->mods = mods;

  return ret;
}


void
dsmcc_diis_free (DsmccDiis *diis)
{
  if (diis->diis)
    {
      dsmcc_list_free (diis->diis, free);
    }

  free (diis);
}


unsigned int
dsmcc_diis_encode (DsmccDiis *diis)
{
  DsmccDii      *dii   = 0;
  DsmccBmod     *bmod  = 0;
  DsmccList     *bmods = 0;
  DsmccList     *mods  = 0;
  DsmccMod      *mod   = 0;
  DsmccLink     *link  = 0;
  DsmccLink     *mlink = 0;
  DsmccLink     *elink = 0;
  DsmccEnt      *ent   = 0;
  unsigned int   bsize = 0;

  bmods = dsmcc_list_new ();
  if (!bmods)
    {
      DSMCC_ERR ();
      return 0;
    }

  mods = dsmcc_list_new ();
  if (!mods)
    {
      DSMCC_ERR ();
      return 0;
    }

  bmod = dsmcc_bmod_new (diis->mods->srg->mid,
                         diis->mods->srg->size,
                         diis->mods->srg->ver,
                         diis->mods->srg->ctag,
                         diis->mods->srg->lab,
                         diis->mods->srg->llen,
                         diis->mods->srg->prio,
                         diis->mods->srg->plen);
  if (!bmod)
    {
      DSMCC_MERR ("(0x%04x) (%d) (0x%02x) (0x%04x)",
                  diis->mods->srg->mid, diis->mods->srg->size,
                  diis->mods->srg->ver, diis->mods->srg->ctag);
      dsmcc_list_free (bmods, (FreeFunc) dsmcc_bmod_free);

      return 0;
    }

  dsmcc_list_push_back (bmods, bmod);
  dsmcc_list_push_back (mods, diis->mods->srg);
  bsize += bmod->size;

  dsmcc_list_foreach (diis->mods->mods, link)
    {
      mod = (DsmccMod*)link->data;

      bmod = dsmcc_bmod_new (mod->mid, mod->size, mod->ver, mod->ctag,
                             mod->lab, mod->llen, mod->prio, mod->plen);
      if (!bmod)
        {
          DSMCC_MERR ("(0x%04x) (%d) (0x%02x) (0x%02x)",
                      mod->mid, mod->size, mod->ver, mod->ctag);
          dsmcc_list_free (bmods, (FreeFunc) dsmcc_bmod_free);

          return 0;
        }

      if ((bsize + bmod->size) > DSMCC_DII_MAX_MODS_SIZE)
        {
          dii = dsmcc_dii_new (1,
                               dsmcc_conf_get_id(),
                               0,
                               diis->mods->srg->did,
                               bmods);
          if (!dii)
            {
              DSMCC_MERR ("(0x%08x) (%d) (%d)",
                          diis->mods->srg->did, dsmcc_list_size (bmods), bsize);
              dsmcc_list_free (bmods, (FreeFunc) dsmcc_bmod_free);
              dsmcc_list_free (diis->diis, (FreeFunc) dsmcc_dii_free);

              return 0;
            }

          if (dii->size > DSMCC_SEC_MAX_SIZE)
            {
              DSMCC_MERR ("(0x%04x) (0x%08x) (%d) (%d) (%d)",
                          dii->id, diis->mods->srg->did, dsmcc_list_size (bmods), bsize, dii->size);
              dsmcc_list_free (bmods, (FreeFunc) dsmcc_bmod_free);
              dsmcc_dii_free (dii);
              dsmcc_list_free (diis->diis, (FreeFunc) dsmcc_dii_free);

              return 0;
            }

          DSMCC_MSG (" + encoded dii with tid <0x%08x> including %d mods (%d bytes).\n",
                     dsmcc_tid (dii->ver, dii->id, dii->up),
                     dsmcc_list_size (bmods), dii->size);

          dsmcc_list_push_back (diis->diis, dii);

          dsmcc_list_foreach (mods, mlink)
            {
              mod = (DsmccMod*)mlink->data;

              mod->tid = dsmcc_tid (dii->ver, dii->id, dii->up);

              dsmcc_list_foreach (mod->ents, elink)
                {
                  ent = (DsmccEnt*)elink->data;

                  ent->tid = mod->tid;
                }
            }

          dsmcc_list_clear (bmods, (FreeFunc) dsmcc_bmod_free);
          dsmcc_list_clear (mods, 0);
          bsize = 0;
        }

      dsmcc_list_push_back (bmods, bmod);
      dsmcc_list_push_back (mods, mod);
      bsize += bmod->size;
    }

  if (bsize > DSMCC_DII_MAX_MODS_SIZE)
    {
      DSMCC_MERR ("(0x%08x) (%d) (%d)",
                  diis->mods->srg->did, dsmcc_list_size (bmods), bsize);
      dsmcc_list_free (bmods, (FreeFunc) dsmcc_bmod_free);
      dsmcc_list_free (diis->diis, (FreeFunc) dsmcc_dii_free);

      return 0;
    }

  dii = dsmcc_dii_new (1,
                       dsmcc_conf_get_id(),
                       0,
                       diis->mods->srg->did,
                       bmods);
  if (!dii)
    {
      DSMCC_MERR ("(0x%08x) (%d) (%d)",
                  diis->mods->srg->did, dsmcc_list_size (bmods), bsize);
      dsmcc_list_free (bmods, (FreeFunc) dsmcc_bmod_free);
      dsmcc_list_free (diis->diis, (FreeFunc) dsmcc_dii_free);

      return 0;
    }

  if (dii->size > DSMCC_SEC_MAX_SIZE)
    {
      DSMCC_MERR ("(0x%04x) (0x%08x) (%d) (%d) (%d)",
                  dii->id, diis->mods->srg->did, dsmcc_list_size (bmods), bsize, dii->size);
      dsmcc_list_free (bmods, (FreeFunc) dsmcc_bmod_free);
      dsmcc_dii_free (dii);
      dsmcc_list_free (diis->diis, (FreeFunc) dsmcc_dii_free);

      return 0;
    }

  DSMCC_MSG (" + encoded dii with tid <0x%08x> including %d mods (%d bytes).\n",
             dsmcc_tid (dii->ver, dii->id, dii->up),
             dsmcc_list_size (bmods), dii->size);

  dsmcc_list_foreach (mods, mlink)
    {
      mod = (DsmccMod*)mlink->data;

      mod->tid = dsmcc_tid (dii->ver, dii->id, dii->up);

      dsmcc_list_foreach (mod->ents, elink)
        {
          ent = (DsmccEnt*)elink->data;

          ent->tid = mod->tid;
        }
    }

  dsmcc_list_push_back (diis->diis, dii);

  return 1;
}


unsigned int
dsmcc_diis_dump_write (DsmccDiis *diis,
                       int        fd)
{
  DsmccLink    *link   = 0;
  unsigned int  ui_tmp = dsmcc_list_size (diis->diis);

  if (write (fd, &ui_tmp, 4) != 4)
    {
      return 0;
    }

  dsmcc_list_foreach (diis->diis, link)
    {
      if (!dsmcc_dii_dump_write ((DsmccDii*)link->data, fd))
        {
          return 0;
        }
    }

  return 1;
}


DsmccList *
dsmcc_diis_dump_read (int fd)
{
  DsmccList *diis = dsmcc_list_new ();
  DsmccDii  *dii  = 0;
  int        dnum = 0;
  int        i    = 0;

  if (read (fd, &dnum, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  for (i = 0; i < dnum; i++)
    {
      dii = dsmcc_dii_dump_read (fd);
      if (!dii)
        {
          DSMCC_ERR ();
          return 0;
        }

      dsmcc_list_push_back (diis, dii);
    }

  return diis;
}
