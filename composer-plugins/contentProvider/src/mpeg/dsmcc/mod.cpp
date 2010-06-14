//module

#include "../../../include/mpeg/dsmcc/mod.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/OCStreamFactory.h"
#include "../../../include/mpeg/dsmcc/code.h"
#include "../../../include/mpeg/dsmcc/mcpd.h"
#include "../../../include/mpeg/dsmcc/mld.h"

DsmccMod *
dsmcc_mod_new (unsigned int   mid,
               unsigned int   cid,
               unsigned int   did,
               unsigned int   tid,
               unsigned char  ver,
               unsigned char  ctag,
               char          *lab,
               unsigned char  prio,
               unsigned char  lev)
{
  DsmccMod *ret = 0;

  ret = (DsmccMod*)malloc (sizeof (DsmccMod));
  if (!ret)
    {
      DSMCC_MERR ("(0x%04x)", mid);
      return 0;
    }

  ret->ents = dsmcc_list_new ();
  if (!ret->ents)
    {
      DSMCC_MERR ("(0x%04x)", mid);
      free (ret);

      return 0;
    }

  ret->mid  = mid;
  ret->cid  = cid;
  ret->did  = did;
  ret->tid  = tid;
  ret->ver  = ver;
  ret->ctag = ctag;
  ret->data = 0;
  ret->size = 0;
  ret->sfil = 0;
  ret->lab  = dsmcc_mld_new (lab, &ret->llen);
  ret->prio = dsmcc_mcpd_new (prio, lev, &ret->plen);
  ret->upd  = 0;

  return ret;
}


void
dsmcc_mod_free (DsmccMod *mod)
{
  if (mod->ents)
    {
      dsmcc_list_free (mod->ents, 0);
    }

  if (mod->data)
    {
      free (mod->data);
    }

  if (mod->lab)
    {
      free (mod->lab);
    }

  free (mod);
}


unsigned int
dsmcc_mod_add (DsmccMod *mod,
               DsmccEnt *ent)
{
  DsmccLink *link = 0;
  DsmccEnt  *tent = 0;

  dsmcc_list_foreach (mod->ents, link)
    {
      tent = (DsmccEnt*)link->data;
      if (tent == ent)
        {
          DSMCC_MWARN ("(%s) (%s)", tent->path, ent->path);
          return 0;
        }
    }

  ent->mid  = mod->mid;
  ent->cid  = mod->cid;
  ent->ctag = mod->ctag;
  ent->tid  = mod->tid;

  mod->size += ent->rlen;

  dsmcc_list_push_back (mod->ents, ent);

  return 1;
}


unsigned int
dsmcc_mod_encode (DsmccMod  *mod,
                  DsmccStdb *stdb)
{
  DsmccLink    *link  = 0;
  DsmccEnt     *tent  = 0;
  unsigned int  msize = 0;
  unsigned int  size  = 0;
  DsmccFil     *fil   = 0;
  DsmccDir     *dir   = 0;
  DsmccStr     *str   = 0;
  DsmccSte     *ste   = 0;
  unsigned int  pos   = 0;
  DsmccList    *fils  = 0;
  DsmccList    *dirs  = 0;
  DsmccList    *strs  = 0;
  DsmccList    *stes  = 0;
  unsigned int  sfil  = 0;

  fils = dsmcc_list_new ();
  if (!fils)
    {
      DSMCC_MERR ("(0x%04x)", mod->mid);
      return 0;
    }

  dirs = dsmcc_list_new ();
  if (!dirs)
    {
      DSMCC_MERR ("(0x%04x)", mod->mid);
      return 0;
    }

  strs = dsmcc_list_new ();
  if (!strs)
    {
      DSMCC_MERR ("(0x%04x)", mod->mid);
      return 0;
    }

  stes = dsmcc_list_new ();
  if (!stes)
    {
      DSMCC_MERR ("(0x%04x)", mod->mid);
      return 0;
    }

  dsmcc_list_foreach (mod->ents, link)
    {
      tent = (DsmccEnt*)link->data;

      switch (tent->type)
        {
          case ot_dir:
            size = dsmcc_code_dir (tent, &dir);
            if (size != tent->rlen)
              {
                DSMCC_MERR ("(0x%08x) (%s) (%s) (0x%04x) (%d) (%d)",
                            tent->key, dsmcc_type_string (tent->type),
                            tent->path, mod->mid, tent->rlen, size);
                return 0;
              }

            msize += size;
            dsmcc_list_push_back (dirs, dir);
            break;
          case ot_fil:
            if (dsmcc_list_size (mod->ents) == 1)
              {
                sfil = 1;
              }
            size = dsmcc_code_fil (tent, &fil, sfil);
            if (size != tent->rlen)
              {
                DSMCC_MERR ("(0x%08x) (%s) (%s) (0x%04x) (%d) (%d)",
                            tent->key, dsmcc_type_string (tent->type),
                            tent->path, mod->mid, tent->rlen, size);
                return 0;
              }
            msize += size;
            dsmcc_list_push_back (fils, fil);
            break;
          case ot_str:
            size = dsmcc_code_str (tent, &str);
            if (size != tent->rlen)
              {
                DSMCC_MERR ("(0x%08x) (%s) (%s) (0x%04x) (%d) (%d)",
                            tent->key, dsmcc_type_string (tent->type),
                            tent->path, mod->mid, tent->rlen, size);
                return 0;
              }
            msize += size;
            dsmcc_list_push_back (strs, str);
            break;
          case ot_ste:
            size = dsmcc_code_ste (tent, stdb, &ste);
            if (size != tent->rlen)
              {
                DSMCC_MERR ("(0x%08x) (%s) (%s) (0x%04x) (%d) (%d)",
                            tent->key, dsmcc_type_string (tent->type),
                            tent->path, mod->mid, tent->rlen, size);
                return 0;
              }
            msize += size;
            dsmcc_list_push_back (stes, ste);
            break;
          default:
            DSMCC_MERR ("(0x%08x) (%s) (%s) (0x%04x) (%d) (%d)",
                        tent->key, dsmcc_type_string (tent->type),
                        tent->path, mod->mid, tent->rlen, size);
            return 0;
        }

      std::string file;
      ObjectInfo* oi;

      oi = new ObjectInfo;
      file = tent->path;
      oi->size = tent->rlen;
      oi->pid = dsmcc_conf_get_pid();
      oi->sd = dsmcc_conf_get_cid();
      oi->componentTag = dsmcc_conf_get_tag();
      oi->ior =  intToStrHexa(dsmcc_conf_get_cid()) + "." +
      		intToStrHexa(mod->mid) + "." + intToStrHexa(tent->key);

      (*OCStreamFactory::objectsIor)[file] = oi;
    }

  mod->size = msize;

  if (sfil)
    {
      mod->sfil = (DsmccFil*)dsmcc_list_pop_front (fils);

      return 1;
    }

  mod->data = (unsigned char*)malloc (msize);
  if (!mod->data)
    {
      DSMCC_MERR ("(0x%04x) (%d)", mod->mid, mod->size);
      dsmcc_list_free (fils, (FreeFunc) dsmcc_fil_free);
      dsmcc_list_free (dirs, (FreeFunc) dsmcc_dir_free);
      dsmcc_list_free (strs, (FreeFunc) dsmcc_str_free);
      dsmcc_list_free (stes, (FreeFunc) dsmcc_ste_free);

      return 0;
    }

  while (dsmcc_list_size (dirs) > 0)
    {
      dir = (DsmccDir*)dsmcc_list_pop_front (dirs);

      memcpy (&mod->data[pos], dir->dir, dir->size);
      pos += dir->size;
    }
  dsmcc_list_free (dirs, (FreeFunc) dsmcc_dir_free);

  while (dsmcc_list_size (strs) > 0)
    {
      str = (DsmccStr*)dsmcc_list_pop_front (strs);

      memcpy (&mod->data[pos], str->str, str->size);
      pos += str->size;
    }
  dsmcc_list_free (strs, (FreeFunc) dsmcc_str_free);

  while (dsmcc_list_size (stes) > 0)
    {
      ste = (DsmccSte*)dsmcc_list_pop_front (stes);

      memcpy (&mod->data[pos], ste->ste, ste->size);
      pos += ste->size;
    }
  dsmcc_list_free (stes, (FreeFunc) dsmcc_ste_free);

  while (dsmcc_list_size (fils) > 0)
    {
      fil = (DsmccFil*)dsmcc_list_pop_front (fils);

      memcpy (&mod->data[pos], fil->fil, fil->size);
      pos += fil->size;
    }
  dsmcc_list_free (fils, (FreeFunc) dsmcc_fil_free);

  return 1;
}


unsigned int
dsmcc_mod_dump_write (DsmccMod *mod,
                      int       fd)
{
  unsigned short  entslen = dsmcc_list_size (mod->ents);
  DsmccLink      *link    = 0;

  if (write (fd, &mod->mid, 2) != 2)
    {
      return 0;
    }

  if (write (fd, &mod->cid, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &mod->did, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &mod->tid, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &mod->ver, 1) != 1)
    {
      return 0;
    }

  if (write (fd, &mod->ctag, 1) != 1)
    {
      return 0;
    }

  if (write (fd, &entslen, 2) != 2)
    {
      return 0;
    }

  dsmcc_list_foreach (mod->ents, link)
    {
      if (!dsmcc_ent_dump_write ((DsmccEnt*)link->data, fd))
        {
          return 0;
        }
    }

  return 1;
}


DsmccMod *
dsmcc_mod_dump_read (int fd)
{
  DsmccList      *ents = dsmcc_list_new ();
  unsigned short  mid  = 0;
  unsigned int    cid  = 0;
  unsigned int    did  = 0;
  unsigned int    tid  = 0;
  unsigned char   ver  = 0;
  unsigned char   ctag = 0;
  unsigned short  num  = 0;
  DsmccEnt       *ent  = 0;
  int             i    = 0;
  DsmccMod       *mod  = 0;

  if (read (fd, &mid, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &cid, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &did, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &tid, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &ver, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &ctag, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &num, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  for (i = 0; i < num; i++)
    {
      ent = dsmcc_ent_dump_read (fd);
      if (!ent)
        {
          DSMCC_ERR ();
          return 0;
        }

      dsmcc_list_push_back (ents, ent);
    }

  /*DSMCC_MSG("dsmcc_mod_dump_read: mid 0x%x, cid 0x%x, did 0x%x, tid 0x%x, ver 0x%x, ctag 0x%x\n", mid, cid, did, tid, ver, ctag);*/
  mod = dsmcc_mod_new (mid, cid, did, tid, ver, ctag, 0,
                       DSMCC_MCPD_PRIO_LOW, DSMCC_MCPD_LEVEL_NONE);
  mod->ents = ents;
  mod->upd  = 0;

  if (mod->mid == 0x0017)
  {
    dsmcc_conf_set_tid(dsmcc_tid (mod->ver + 1, mod->mid, mod->upd + 1));
  }

  return mod;
}
