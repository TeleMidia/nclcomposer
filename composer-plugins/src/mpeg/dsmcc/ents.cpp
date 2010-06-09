// filesystem entity management

#include "../../../include/mpeg/dsmcc/ents.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/crc.h"
#include "../../../include/mpeg/dsmcc/dtap.h"
#include "../../../include/mpeg/dsmcc/size.h"


static void
dsmcc_ents_build (DsmccEnts *ents,
                  DsmccEnt  *root)
{
  DsmccLink *link = 0;
  DsmccEnt  *ent  = 0;

  if (root->type == ot_dir && root->ents)
    {
      dsmcc_list_foreach (root->ents, link)
        {
          ent = (DsmccEnt*)link->data;

          if (ent->ext)
            {
              continue;
            }

          ent->key = dsmcc_conf_get_key ();

          if (ent->type == ot_dir && ent->ents)
            {
              dsmcc_list_push_back (ents->dirs, ent);
              dsmcc_ents_build (ents, ent);
            }
          else if (ent->type != ot_non)
            {
              dsmcc_list_push_back (ents->objs, ent);
            }
        }
    }
}


DsmccEnts *
dsmcc_ents_new (char *name)
{
  DsmccEnts *ret  = 0;
  DsmccLink *link = 0;
  DsmccEnt  *ent  = 0;

  ret = (DsmccEnts*)malloc (sizeof (DsmccEnts));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

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

  ret->root = dsmcc_ent_new (name);
  if (!ret->root)
    {
      DSMCC_ERR ();
      dsmcc_list_free (ret->dirs, 0);
      dsmcc_list_free (ret->objs, 0);
      dsmcc_list_free (ret->code, 0);
      free (ret);

      return 0;
    }

  ret->root->key = dsmcc_conf_get_key ();
  dsmcc_list_push_back (ret->dirs, ret->root);
  dsmcc_ents_build (ret, ret->root);

  dsmcc_list_foreach (ret->dirs, link)
    {
      ent = (DsmccEnt*)link->data;

      ent->rlen = dsmcc_size_dir (ent->ents);
      ent->crc  = dsmcc_crc_dir (ent->ents);
    }

  dsmcc_list_foreach (ret->objs, link)
    {
      ent = (DsmccEnt*)link->data;

      switch (ent->type)
        {
          case ot_fil:
            ent->rlen = dsmcc_size_fil (ent->size);
            break;
          case ot_str:
            ent->rlen = dsmcc_size_str (ent->size);
            if (ent->bptag)
              {
                ent->rlen += DSMCC_DTAP_SIZE;
              }
            if (ent->npttag)
              {
                ent->rlen += DSMCC_DTAP_SIZE;
              }
            break;
          case ot_ste:
            ent->rlen = dsmcc_size_ste (ent->size);
            if (ent->bptag)
              {
                ent->rlen += DSMCC_DTAP_SIZE;
              }
            if (ent->npttag)
              {
                ent->rlen += DSMCC_DTAP_SIZE;
              }
            if (ent->stetag)
              {
                ent->rlen += DSMCC_DTAP_SIZE;
              }
            break;
          default:
            break;
        }
    }

  return ret;
}


void
dsmcc_ents_free (DsmccEnts *ents)
{
  if (ents->dirs)
    {
      dsmcc_list_free (ents->dirs, (FreeFunc) dsmcc_ent_free);
    }

  if (ents->objs)
    {
      dsmcc_list_free (ents->objs, (FreeFunc) dsmcc_ent_free);
    }

  if (ents->code)
    {
      dsmcc_list_free (ents->code, (FreeFunc) dsmcc_ent_free);
    }

  free (ents);
}
