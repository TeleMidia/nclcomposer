// encoding of biop objects

#include "../../../include/mpeg/dsmcc/code.h"

#include <fcntl.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/bind.h"
#include "../../../include/mpeg/dsmcc/dtap.h"
#include "../../../include/mpeg/dsmcc/conf.h"


unsigned int
dsmcc_code_dir (DsmccEnt  *ent,
                DsmccDir **dir)
{
  DsmccDir     *data  = 0;
  DsmccLink    *link  = 0;
  DsmccEnt     *tent  = 0;
  DsmccList    *binds = 0;
  DsmccBind    *bind  = 0;
  unsigned int  key   = 0;

  (*dir) = 0;

  binds = dsmcc_list_new ();
  if (!binds)
    {
      DSMCC_ERR ();
      return 0;
    }

  DSMCC_MSG (" # encoding dir key(0x%08x) <%s>\n", ent->key, ent->path);

  dsmcc_list_foreach (ent->ents, link)
    {
      tent = (DsmccEnt*)link->data;

      if (tent->update)
        {
          key = tent->upent->key;
        }
      else
        {
          key = tent->key;
        }
        
      if (tent->tid != (unsigned int)dsmcc_conf_get_tid())
      {
        tent->tid = dsmcc_conf_get_tid();
      }
      if (tent->cid != dsmcc_conf_get_cid())
      {
        tent->cid = dsmcc_conf_get_cid();
      }
      if (tent->ctag != dsmcc_conf_get_tag())
      {
        tent->ctag = dsmcc_conf_get_tag();
      }
      /*DSMCC_MSG (" # creating bind:");
      DSMCC_MSG (" type %s", dsmcc_type_string (tent->type));
      DSMCC_MSG (" name %s", tent->name);
      DSMCC_MSG (" tid 0x%x", tent->tid);
      DSMCC_MSG (" ctag 0x%x", tent->ctag);
      DSMCC_MSG (" key 0x%x", key);
      DSMCC_MSG (" mid 0x%x", tent->mid);
      DSMCC_MSG (" cid 0x%x\n", tent->cid);*/
      bind = dsmcc_bind_new (tent->type,
                             tent->name,
                             tent->tid,
                             tent->ctag,
                             key,
                             tent->mid,
                             tent->cid,
                             tent->ext);
      if (!bind)
        {
          DSMCC_MERR ("(0x%08x) (0x%04x) (%s) (%s)",
                      key, tent->mid, dsmcc_type_string (tent->type), tent->path);
          dsmcc_list_free (binds, free);

          return 0;
        }

      dsmcc_list_push_back (binds, bind);
    }

  if (ent->update)
    {
      key = ent->upent->key;
    }
  else
    {
      key = ent->key;
    }

  data = dsmcc_dir_new (ent->path, key, binds, ent->srg);
  if (!data)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (%s) (%s)",
                  key, ent->mid, dsmcc_type_string (ent->type), ent->path);
      dsmcc_list_free (binds, free);

      return 0;
    }

  dsmcc_list_free (binds, (FreeFunc) dsmcc_bind_free);

  (*dir) = data;

  return data->size;
}


unsigned int
dsmcc_code_fil (DsmccEnt      *ent,
                DsmccFil     **fil,
                unsigned int   sin)
{
  DsmccFil      *data = 0;
  unsigned char *buf  = 0;

#ifdef WIN32
  FILE*          fp;
  size_t         bCnt = 0;

#else
  int            fd   = 0;
#endif

  unsigned int   key  = 0;

  (*fil) = 0;

  if (ent->update)
    {
      key = ent->upent->key;
    }
  else
    {
      key = ent->key;
    }

  DSMCC_MSG (" # encoding fil key(0x%08x) <%s>\n", key, ent->path);

  if (ent->size > 0 && !sin)
    {
#ifdef WIN32
	  fp = fopen(ent->path, "rb");
	  if (fp == NULL)

#else
      fd = open (ent->path, O_RDONLY);
      if (fd < 0)
#endif
        {
          DSMCC_MERR ("(0x%08x) (0x%04x) (%s) (%s)",
                      key, ent->mid, dsmcc_type_string (ent->type), ent->path);
          return 0;
        }

      buf = (unsigned char*)malloc (ent->size);
      if (!buf)
        {
          DSMCC_MERR ("(0x%08x) (0x%04x) (%s) (%d) (%s)",
                      key, ent->mid, dsmcc_type_string (ent->type),
                      ent->size, ent->path);
#ifdef WIN32
          fclose(fp);
#else
          close (fd);
#endif
          return 0;
        }

#ifdef WIN32
      bCnt = fread(buf, ent->size, 1, fp);
      if (bCnt < 0)
#else
      if (read (fd, buf, ent->size) < 0)
#endif
        {
          DSMCC_MERR ("(0x%08x) (0x%04x) (%s) (%d) (%s)",
                      key, ent->mid, dsmcc_type_string (ent->type),
                      ent->size, ent->path);
          free (buf);
#ifdef WIN32
          fclose(fp);
#else
          close (fd);
#endif
          return 0;
        }
#ifdef WIN32
      fclose(fp);
#else
      close (fd);
#endif
    }

  data = dsmcc_fil_new (strdup (ent->path), key, buf, ent->size, sin);
  if (!data)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (%s) (%d) (%s)",
                  key, ent->mid, dsmcc_type_string (ent->type),
                  ent->size, ent->path);
      free (buf);

      return 0;
    }

  if (buf)
    {
      free (buf);
    }

  (*fil) = data;

  return data->size;
}


unsigned int
dsmcc_code_str (DsmccEnt  *ent,
                DsmccStr **str)
{
  DsmccStr      *data = 0;
  unsigned char *info = 0;
  unsigned int   key  = 0;

  (*str) = 0;

  if (ent->update)
    {
      key = ent->upent->key;
    }
  else
    {
      key = ent->key;
    }

  DSMCC_MSG (" # encoding str key(0x%08x) <%s>\n", key, ent->path);

  if (ent->info)
    {
      info = (unsigned char*)dsmcc_list_pop_front (ent->info);
    }

  data = dsmcc_str_new (ent->path,
                        key,
                        info,
                        ent->size,
                        ent->dsec,
                        ent->dmsec,
                        ent->dmask,
                        ent->bptag,
                        ent->npttag,
                        ent->nptbase);
  if (info)
    {
      free (info);
    }

  if (!data)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (%s) (%d) (%s)",
                  key, ent->mid, dsmcc_type_string (ent->type),
                  ent->size, ent->path);
      return 0;
    }

  (*str) = data;

  return data->size;
}


unsigned int
dsmcc_code_ste (DsmccEnt   *ent,
                DsmccStdb  *stdb,
                DsmccSte  **ste)
{
  DsmccSte       *data  = 0;
  DsmccList      *names = dsmcc_list_new ();
  unsigned char  *info  = 0;
  unsigned short  ids[256];
  unsigned char  *name  = 0;
  unsigned char  *id    = 0;
  unsigned int    nr    = 0;
  unsigned int    key   = 0;

  (*ste) = 0;

  if (ent->update)
    {
      key = ent->upent->key;
    }
  else
    {
      key = ent->key;
    }

  DSMCC_MSG (" # encoding ste key(0x%08x) <%s>\n", key, ent->path);

  if (ent->info)
    {
      info = (unsigned char*)dsmcc_list_pop_front (ent->info);

      while (dsmcc_list_size (ent->info))
        {
          name = (unsigned char*)dsmcc_list_pop_front (ent->info);
          id   = (unsigned char*)dsmcc_list_pop_front (ent->info);

          dsmcc_list_push_back (names, name);
          ids[nr] = strtol ((char *)id, 0, 16);

          if (dsmcc_conf_get_ste ())
            {
              if (!dsmcc_stdb_add (stdb, ids[nr], (char *) info))
                {
                  DSMCC_MERR ("(0x%08x) (0x%04x) (%s) (%d) (%s) (%s) (0x%04x)",
                              key, ent->mid, dsmcc_type_string (ent->type),
                              ent->size, ent->path, name, ids[nr]);

                  return 0;
                }
            }

          ent->rlen += strlen ((char *) name) + 4;

          nr++;
        }
    }

  data = dsmcc_ste_new (ent->path,
                        key,
                        info,
                        ent->size,
                        ent->dsec,
                        ent->dmsec,
                        ent->dmask,
                        ent->bptag,
                        ent->npttag,
                        ent->nptbase,
                        ent->stetag,
                        names,
                        ids);
  if (info)
    {
      free (info);
    }

  if (!data)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (%s) (%d) (%s)",
                  key, ent->mid, dsmcc_type_string (ent->type),
                  ent->size, ent->path);
      return 0;
    }

  (*ste) = data;

  return data->size;
}
