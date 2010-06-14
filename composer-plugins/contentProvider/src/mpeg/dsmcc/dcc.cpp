// data carousel contents

#include "../../../include/mpeg/dsmcc/dcc.h"

#include <stdlib.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccDcc *
dsmcc_dcc_new (DsmccEnts *ents,
               DsmccEnt  *ent,
               DsmccExps *exps)
{
  DsmccDcc *ret = 0;

  ret = (DsmccDcc*)malloc (sizeof (DsmccDcc));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->ents = ents;
  ret->ent  = ent;
  ret->exps = exps;

  ret->ent->dcroot = 1;

  ret->dirs = dsmcc_list_new ();
  if (!ret->dirs)
    {
      DSMCC_ERR ();
      free (ret);

      return 0;
    }

  ret->objs = dsmcc_list_new ();
  if (!ret->objs)
    {
      DSMCC_ERR ();
      dsmcc_list_free (ret->dirs, 0);
      free (ret);

      return 0;
    }

  ret->code = dsmcc_list_new ();
  if (!ret->code)
    {
      DSMCC_ERR ();
      dsmcc_list_free (ret->dirs, 0);
      dsmcc_list_free (ret->objs, 0);
      free (ret);

      return 0;
    }

  return ret;
}


static unsigned int
dsmcc_dcc_ents_rec (DsmccDcc *dcc,
                    DsmccEnt *ent)
{
  DsmccEnt  *tent = 0;
  DsmccLink *link = 0;

  if (ent->ctag != DSMCC_ENT_UNDEF_TAG)
    {
      if (ent->dcroot)
        {
          return 1;
        }

      DSMCC_MERR ("<%s>\n", ent->path);
      return 0;
    }

  ent->ctag = dcc->exps->ctag;

  if (ent->type == ot_dir)
    {
      dsmcc_list_push_back (dcc->dirs, ent);

      dsmcc_list_foreach (ent->ents, link)
        {
          tent = (DsmccEnt*)link->data;

          if (!dsmcc_dcc_ents_rec (dcc, tent))
            {
              DSMCC_ERR ();
              return 0;
            }
        }
    }
  else if (!ent->ext)
    {
      dsmcc_list_push_back (dcc->objs, ent);
    }

  return 1;
}


unsigned int
dsmcc_dcc_ents (DsmccDcc *dcc)
{
  if (!dsmcc_dcc_ents_rec (dcc, dcc->ent))
    {
      DSMCC_ERR ();
      return 0;
    }

  return 1;
}


void
dsmcc_dcc_free (DsmccDcc *dcc)
{
  dsmcc_list_free (dcc->dirs, 0);

  dsmcc_list_free (dcc->objs, 0);

  dsmcc_list_free (dcc->code, 0);

  free (dcc);
}


void
dsmcc_dcc_show (DsmccDcc *dcc)
{
  DsmccLink    *link = 0;
  DsmccEnt     *ent  = 0;
  unsigned int  nume = 0;
  unsigned int  size = 0;

  DSMCC_MSG ("\n --> DataCarousel on pid <0x%04x> with ctag <0x%02x> :\n",
             dcc->exps->pid, dcc->exps->ctag);

  dsmcc_list_foreach (dcc->dirs, link)
    {
      ent = (DsmccEnt*)link->data;

      DSMCC_MSG (" + dir <%s>\n", ent->path);

      size += ent->rlen;
      nume++;
    }

  dsmcc_list_foreach (dcc->objs, link)
    {
      ent = (DsmccEnt*)link->data;

      DSMCC_MSG (" + obj <%s>\n", ent->path);

      size += ent->rlen;
      nume++;
    }

  DSMCC_MSG (" + parsed %d objects with total size %d kb.\n",
             nume, (size / 1024) + 1);
}


unsigned int
dsmcc_dcc_dump_write (DsmccDcc *dcc,
                      int       fd)
{
  if (!dsmcc_exps_dump_write (dcc->exps, fd))
    {
      return 0;
    }

  return 1;
}


DsmccDcc *
dsmcc_dcc_dump_read (int fd)
{
  DsmccDcc  *ret  = 0;
  DsmccExps *exps = 0;

  exps = dsmcc_exps_dump_read (fd);
  if (!exps)
    {
      return 0;
    }

  ret = (DsmccDcc*)malloc (sizeof (DsmccDcc));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->ents = 0;
  ret->ent  = 0;
  ret->exps = exps;
  ret->dirs = 0;
  ret->objs = 0;
  ret->code = 0;

  return ret;
}
