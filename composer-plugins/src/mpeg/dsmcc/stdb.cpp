// stream event database

#include "../../../include/mpeg/dsmcc/stdb.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/sec.h"
#include "../../../include/mpeg/dsmcc/sted.h"
#include "../../../include/mpeg/dsmcc/util.h"


static DsmccStdbEnt *
dsmcc_stdb_ent_new (unsigned short  id,
                    char           *desc)
{
  DsmccStdbEnt *ret = 0;

  ret = (DsmccStdbEnt*)malloc (sizeof (DsmccStdbEnt));
//  ret = (DsmccStdb*)malloc (sizeof (DsmccStdb));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->id   = id;
  ret->desc = strdup (desc);

  return ret;
}


static void
dsmcc_stdb_ent_free (DsmccStdbEnt *ent)
{
  free (ent->desc);

  free (ent);
}


DsmccStdb *
dsmcc_stdb_new (void)
{
  DsmccStdb *ret = 0;

  ret = (DsmccStdb*)malloc (sizeof (DsmccStdb));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->secs = dsmcc_list_new ();
  if (!ret->secs)
    {
      DSMCC_ERR ();
      free (ret);

      return 0;
    }

  ret->evs = dsmcc_list_new ();
  if (!ret->evs)
    {
      DSMCC_ERR ();
      free (ret);

      return 0;
    }

  return ret;
}


void
dsmcc_stdb_free (DsmccStdb *stdb)
{
  dsmcc_list_free (stdb->evs, (FreeFunc) dsmcc_stdb_ent_free);

  free (stdb);
}


unsigned int
dsmcc_stdb_add (DsmccStdb      *stdb,
                unsigned short  id,
                char           *desc)
{
  DsmccLink    *link = 0;
  DsmccStdbEnt *ent  = 0;

  dsmcc_list_foreach (stdb->evs, link)
    {
      ent = (DsmccStdbEnt*)link->data;

      if (ent->id == id)
        {
          return 0;
        }
    }

  ent = dsmcc_stdb_ent_new (id, desc);
  if (!ent)
    {
      DSMCC_MERR ("(0x%04x)", id);
      return 0;
    }

  dsmcc_list_push_back (stdb->evs, ent);

  return 1;
}


unsigned int
dsmcc_stdb_encode (DsmccStdb *stdb)
{
  DsmccLink    *link = 0;
  DsmccStdbEnt *ent  = 0;
  DsmccSec     *sec  = 0;
  DsmccSted    *sted = 0;

  if (dsmcc_list_size (stdb->evs) == 0)
    {
      return 1;
    }

  DSMCC_MSG ("\n --> creating stream-events...\n");

  dsmcc_list_foreach (stdb->evs, link)
    {
      ent = (DsmccStdbEnt*)link->data;

      sted = dsmcc_sted_new (ent->id,
                             0,
                             (unsigned char *) ent->desc,
                             strlen ((char *) ent->desc));
      if (!sted)
        {
          DSMCC_MERR ("(0x%04x)", ent->id);

          return 0;
        }

      sec = dsmcc_sec_new (ot_ste,
                           ent->id,
                           1,
                           0,
                           0,
                           sted->sted,
                           sted->size,
                           0,
                           0,
                           0,
                           0,
                           0);
      dsmcc_sted_free (sted);
      if (!sec)
        {
          DSMCC_MERR ("(0x%04x)", ent->id);

          return 0;
        }

      dsmcc_list_push_back (stdb->secs, sec);

      DSMCC_MSG (" + created stream-event with id <0x%04x>\n", ent->id);
    }

  return 1;
}
