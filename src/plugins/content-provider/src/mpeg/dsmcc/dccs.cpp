// data carousel contents management

#include "../../../include/mpeg/dsmcc/dccs.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/dcc.h"
#include "../../../include/mpeg/dsmcc/exps.h"


DsmccDccs *
dsmcc_dccs_new (DsmccEnts *ents)
{
  DsmccDccs *ret = 0;

  ret = (DsmccDccs*)malloc (sizeof (DsmccDccs));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->ents = ents;

  ret->dccs = dsmcc_list_new ();
  if (!ret->dccs)
    {
      DSMCC_ERR ();
      free (ret);

      return 0;
    }

  return ret;
}


static DsmccEnt *
dsmcc_dccs_find (DsmccDccs *dccs,
                 DsmccExps *exps)
{
  DsmccLink *link = 0;
  DsmccEnt  *ent  = 0;

  dsmcc_list_foreach (dccs->ents->dirs, link)
    {
      ent = (DsmccEnt*)link->data;

      if (!strcmp (ent->rpath, exps->path))
        {
          return  ent;
        }
    }

  return 0;
}


static DsmccDcc *
dsmcc_dccs_create_root (DsmccDccs    *dccs,
                        unsigned int  single)
{
  DsmccDcc  *dcc  = 0;
  DsmccLink *link = 0;
  DsmccEnt  *ent  = 0;

  dcc = dsmcc_dcc_new (dccs->ents, dccs->ents->root, 0);
  if (!dcc)
    {
      DSMCC_ERR ();
      return 0;
    }

#ifdef WIN32
  dcc->exps = dsmcc_exps_new_intern (dsmcc_conf_get_pid (),
                                     dsmcc_conf_get_tag (),
                                     strdup ("\\"));  
#else
  dcc->exps = dsmcc_exps_new_intern (dsmcc_conf_get_pid (),
                                     dsmcc_conf_get_tag (),
                                     strdup ("/"));
#endif

  if (!dcc->exps)
    {
      DSMCC_ERR ();
      return 0;
    }

  dcc->exps->root = 1;

  if (single)
    {
      dcc->ents = dccs->ents;
      dcc->dirs = dccs->ents->dirs;
      dcc->objs = dccs->ents->objs;

      return dcc;
    }

  dsmcc_list_foreach (dccs->ents->dirs, link)
    {
      ent = (DsmccEnt*)link->data;

      if (ent->ctag == DSMCC_ENT_UNDEF_TAG)
        {
          dsmcc_list_push_back (dcc->dirs, ent);
        }
    }

  dsmcc_list_foreach (dccs->ents->objs, link)
    {
      ent = (DsmccEnt*)link->data;

      if (ent->ctag == DSMCC_ENT_UNDEF_TAG)
        {
          dsmcc_list_push_back (dcc->objs, ent);
        }
    }

  return dcc;
}


unsigned int
dsmcc_dccs_ents (DsmccDccs *dccs)
{
  DsmccExps *exps = 0;
  DsmccList *list = 0;
  DsmccLink *link = 0;
  DsmccEnt  *ent  = 0;
  DsmccDcc  *dcc  = 0;

  list = dsmcc_conf_get_exps ();

  if (dsmcc_list_size (list) == 0)
    {
      dcc = dsmcc_dccs_create_root (dccs, 1);
      if (!dcc)
        {
          DSMCC_ERR ();
          return 0;
        }

      dsmcc_list_push_back (dccs->dccs, dcc);

      DSMCC_MSG (" + added DataCarousel for <%s>\n", dcc->exps->path);

      return 1;
    }

  dsmcc_list_foreach (list, link)
    {
      exps = (DsmccExps*)link->data;

      ent = dsmcc_dccs_find (dccs, exps);
      if (!ent)
        {
          DSMCC_MERR ("<%s> not present!", exps->path);
          return 0;
        }

      dcc = dsmcc_dcc_new (dccs->ents, ent, exps);
      if (!dcc)
        {
          DSMCC_ERR ();
          return 0;
        }

      if (!dsmcc_dcc_ents (dcc))
        {
          DSMCC_ERR ();
          return 0;
        }

      DSMCC_MSG (" + added DataCarousel for <%s>\n", exps->path);

      dsmcc_list_push_back (dccs->dccs, dcc);
    }

  dcc = dsmcc_dccs_create_root (dccs, 0);
  if (!dcc)
    {
      DSMCC_ERR ();
      return 0;
    }

  dsmcc_list_push_front (dccs->dccs, dcc);

  DSMCC_MSG (" + added DataCarousel for <%s>\n", dcc->exps->path);

  return 1;
}


void
dsmcc_dccs_free (DsmccDccs *dccs)
{
  if (dccs->dccs)
    {
      dsmcc_list_free (dccs->dccs, (FreeFunc) dsmcc_dcc_free);
    }

  free (dccs);
}


void
dsmcc_dccs_show (DsmccDccs *dccs)
{
  DsmccLink *link = 0;
  DsmccDcc  *dcc  = 0;

  dsmcc_list_foreach (dccs->dccs, link)
    {
      dcc = (DsmccDcc*)link->data;

      dsmcc_dcc_show (dcc);
    }
}
