// filesystem entity

#include "../../../include/mpeg/dsmcc/ent.h"

#ifdef WIN32
  #include <sys/types.h>
  #include <stdio.h>
  #include <io.h>
  #include "../../../include/win32/alphasort.h"
  #include "../../../include/win32/scandir.h"
#endif

#include <dirent.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/crc.h"
#include "../../../include/mpeg/dsmcc/dir.h"
#include "../../../include/mpeg/dsmcc/stri.h"


#define DSMCC_IS_DIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#define DSMCC_IS_FIL(mode) (((mode) & S_IFMT) == S_IFREG)
#define DSMCC_IS_STR(mode) ((((mode) & S_IFMT) == S_IFREG) && (((mode) & S_IRWXU) == (S_IRUSR | S_IXUSR)))
#define DSMCC_IS_STE(mode) ((((mode) & S_IFMT) == S_IFREG) && (((mode) & S_IRWXU) == S_IRWXU))

#ifdef WIN32
  #define DSMCC_IS_EXT(mode) (((mode) & S_IFMT) == 0120000)
#else
  #define DSMCC_IS_EXT(mode) (((mode) & S_IFMT) == S_IFLNK)
#endif

#define DSMCC_STREAM_MAX_SIZE 0xFFFF


static ObjectType
dsmcc_ent_type (unsigned int mode)
{
  if (DSMCC_IS_EXT(mode))
    {
      return ot_ext;
    }

  if (DSMCC_IS_DIR(mode))
    {
      return ot_dir;
    }

   if (DSMCC_IS_STE(mode))
    {
      return ot_ste;
    }

  if (DSMCC_IS_STR(mode))
    {
      return ot_str;
    }

  if (DSMCC_IS_FIL(mode))
    {
      return ot_fil;
    }

  return ot_non;
}


static unsigned int
dsmcc_ent_stream_format (DsmccEnt     *ent,
                         ObjectType    type,
                         char         *path,
                         unsigned int  size)
{
  unsigned char *info = 0;
  unsigned char *head = 0;
  unsigned char *tail = 0;
  unsigned char *dinf = 0;
  int            fd   = 0;
  int            len  = 0;
  int            pos  = 0;
  int            tmp  = 0;
  int            last = 0;
  int            otmp = 0;
  DsmccLink     *link = 0;
  unsigned int   ctmp = 0;
  unsigned int   nr_c = 0;
  char          *comps[64];
  unsigned char  buf[DSMCC_STREAM_MAX_SIZE + 1];

  if (size == 0)
    {
      return 1;
    }

  if (size > DSMCC_STREAM_MAX_SIZE)
    {
      return 0;
    }

  fd = open (path, O_RDONLY);
  if (fd < 0)
    {
      DSMCC_MERR ("(%s)", path);
      return 0;
    }

  memset (buf, 0, DSMCC_STREAM_MAX_SIZE + 1);

  if (read (fd, buf, size) < 0)
    {
      DSMCC_MERR ("(%s)", path);
      close (fd);
      return 0;
    }

  close (fd);

  if (type == ot_str)
    {
      head = (unsigned char *) strstr ((char *) buf, DSMCC_FORMAT_STR);
      if (!head)
        {
          DSMCC_MWARN ("ignoring malformed stream <%s> (bad header).", path);
          return 0;
        }

      len = strlen (DSMCC_FORMAT_STR) + 1;
    }
  else if (type == ot_ste)
    {
      head = (unsigned char *) strstr ((char *) buf, DSMCC_FORMAT_STE);
      if (!head)
        {
          DSMCC_MWARN ("ignoring malformed stream-event <%s> (bad header).", path);
          return 0;
        }

      len = strlen (DSMCC_FORMAT_STE) + 1;
    }
  else
    {
      return 0;
    }

  if ((size - len) == 0)
    {
      if (type == ot_str)
        {
          DSMCC_MSG (" + found stream <%s>\n", path);
        }
      else
        {
          DSMCC_MSG (" + found stream-event <%s>\n", path);
        }

      return 1;
    }

  dinf = (unsigned char *) strstr ((char *) head, DSMCC_FORMAT_INF);
  tail = (unsigned char *) strstr ((char *) head, DSMCC_FORMAT_IDS);
  if (tail)
    {
      if (type != ot_ste)
        {
          DSMCC_MWARN ("ignoring malformed stream <%s> (type mismatch).", path);
          return 0;
        }

      if (dinf)
        {
          pos = abs (head - dinf) - len - 1;
        }
      else
        {
          pos = abs (head - tail) - len - 1;
        }
    }
  else
    {
      if (dinf)
        {
          pos = abs (head - dinf) - len - 1;
        }
      else
        {
          pos = size - abs (head - buf) - len - 1;
        }
    }

  if (pos <= 0)
    {
      pos  = 0;
      info = (unsigned char *) strdup ("");
    }
  else
    {
      info = (unsigned char*)malloc (pos + 1);
      if (!info)
        {
          DSMCC_MERR ("(%s)", path);
          return 0;
        }

      memcpy (info, &head[len], pos);
      info[pos] = 0;
    }

  ent->info = dsmcc_list_new ();
  ent->size = pos;

  dsmcc_list_push_back (ent->info, info);

  if (tail)
    {
      buf[size] = 0;
      tmp = size - 1;

      while (buf[tmp] == 10)
        {
          buf[tmp] = 0;
          tmp--;
        }

      last = tmp;
      pos  = strlen (DSMCC_FORMAT_IDS) + 1;
      tmp  = pos;
      otmp = pos;

      while (&tail[tmp] < &buf[last])
        {
          if (tail[tmp] == 10)
            {
              tail[tmp] = 0;

              if (strlen ((char *) &tail[otmp]) > 0)
                {
                  dsmcc_list_push_back (ent->info, strdup ((char *) &tail[otmp]));
                }

              otmp = tmp + 1;
            }

          tmp++;
        }

      if (strlen ((char *) &tail[otmp]) > 0)
        {
          dsmcc_list_push_back (ent->info, strdup ((char *) &tail[otmp]));
        }

      if ((dsmcc_list_size (ent->info) % 2) == 0)
        {
          DSMCC_MWARN ("ignoring malformed stream-event <%s> (ids/names count mismatch).", path);
          dsmcc_list_clear (ent->info, free);
          return 0;
        }
    }

  if (type == ot_ste)
    {
      int nr = 0;
      dsmcc_list_foreach (ent->info, link)
        {
          if (nr > 0 && nr % 2 == 0)
            {
              int conv = strtol ((char *) link->data, 0, 16);
              if (conv < DSMCC_STREAM_MIN_ID ||
                  conv > DSMCC_STREAM_MAX_ID ||
                  (conv > DSMCC_STREAM_MAX_DIN_ID &&
                   conv < DSMCC_STREAM_MIN_SCH_ID))
                {
                  DSMCC_MWARN ("ignoring malformed stream-event <%s> (bad event-id range).", path);
                  dsmcc_list_clear (ent->info, free);
                  return 0;
                }
            }
          else
            {
              if (strlen ((char *) link->data) > 0xFF)
                {
                  DSMCC_MWARN ("ignoring malformed stream-event <%s> (event-name too long).", path);
                  dsmcc_list_clear (ent->info, free);
                  return 0;
                }
            }

          nr++;
        }
    }

  if (tail)
    {
      *(--tail) = 0;
    }

  if (dinf)
    {
      len = strlen (DSMCC_FORMAT_INF) + 1;

      nr_c = dsmcc_get_comp ((char *) (dinf + len), comps, " ");
      if (nr_c != 9)
        {
          if (type == ot_str)
            {
              DSMCC_MWARN ("ignoring malformed stream <%s> (expected 6 stream-info components).", path);
            }
          else
            {
              DSMCC_MWARN ("ignoring malformed stream-event <%s> (expected 6 stream-info components).", path);
            }

          return 0;
        }

      for (pos = 0; (unsigned int)pos < strlen (comps[nr_c - 1]); pos++)
        {
          if (comps[nr_c - 1][pos] == 10)
            {
              comps[nr_c - 1][pos] = 0;
            }
        }

      ctmp = strtol (comps[0], 0, 16);
      if (ctmp >= 0xFF)
        {
          if (type == ot_str)
            {
              DSMCC_MWARN ("ignoring malformed stream <%s> (bad program component-tag).", path);
            }
          else
            {
              DSMCC_MWARN ("ignoring malformed stream-event <%s> (bad program component-tag).", path);
            }
        }
      ent->bptag = ctmp;

      ctmp = strtol (comps[1], 0, 16);
      if (ctmp >= 0xFF)
        {
          if (type == ot_str)
            {
              DSMCC_MWARN ("ignoring malformed stream <%s> (bad npt component-tag).", path);
            }
          else
            {
              DSMCC_MWARN ("ignoring malformed stream-event <%s> (bad npt component-tag).", path);
            }
        }
      ent->npttag = ctmp;

      ctmp = strtol (comps[2], 0, 16);
      if (ctmp >= 0xFF)
        {
          if (type == ot_str)
            {
              DSMCC_MWARN ("ignoring malformed stream <%s> (bad npt timebase).", path);
            }
          else
            {
              DSMCC_MWARN ("ignoring malformed stream-event <%s> (bad npt timebase).", path);
            }
        }
      ent->nptbase = ctmp;

      ctmp = strtol (comps[3], 0, 16);
      if (ctmp >= 0xFF)
        {
          if (type == ot_str)
            {
              DSMCC_MWARN ("ignoring malformed stream <%s> (bad events component-tag).", path);
            }
          else
            {
              DSMCC_MWARN ("ignoring malformed stream-event <%s> (bad events component-tag).", path);
            }
        }
      ent->stetag = ctmp;

      ent->dsec  = strtol (comps[4], 0, 10);
      ent->dmsec = strtol (comps[5], 0, 10);
      ent->dmask = 0;

      ctmp = strtol (comps[6], 0, 10);
      if (ctmp != 0 && ctmp != 1)
        {
          if (type == ot_str)
            {
              DSMCC_MWARN ("ignoring malformed stream <%s> (bad audio stream-type).", path);
            }
          else
            {
              DSMCC_MWARN ("ignoring malformed stream-event <%s> (bad audio stream-type).", path);
            }
        }

      if (ctmp)
        {
          ent->dmask = (ent->dmask | DSMCC_STRI_AUDIO);
        }

      ctmp = strtol (comps[7], 0, 10);
      if (ctmp != 0 && ctmp != 1)
        {
          if (type == ot_str)
            {
              DSMCC_MWARN ("ignoring malformed stream <%s> (bad video stream-type).", path);
            }
          else
            {
              DSMCC_MWARN ("ignoring malformed stream-event <%s> (bad video stream-type).", path);
            }
        }

      if (ctmp)
        {
          ent->dmask = (ent->dmask | DSMCC_STRI_VIDEO);
        }

      ctmp = strtol (comps[8], 0, 10);
      if (ctmp != 0 && ctmp != 1)
        {
          if (type == ot_str)
            {
              DSMCC_MWARN ("ignoring malformed stream <%s> (bad data stream-type).", path);
            }
          else
            {
              DSMCC_MWARN ("ignoring malformed stream-event <%s> (bad data stream-type).", path);
            }
        }

      if (ctmp)
        {
          ent->dmask = (ent->dmask | DSMCC_STRI_DATA);
        }
    }

  if (type == ot_str)
    {
      DSMCC_MSG (" + found stream <%s>\n", path);
    }
  else
    {
      DSMCC_MSG (" + found stream-event <%s>\n", path);
    }

  return 1;
}


static unsigned int
dsmcc_ent_external_format (DsmccEnt     *ent,
                           char         *path,
                           unsigned int  size)
{
#ifndef WIN32
  char          dest[1024];
  int           len  = 0;
  char         *ext  = 0;
  unsigned int  cid  = 0;
  unsigned int  onid = 0;
  unsigned int  tsid = 0;
  unsigned int  sid  = 0;
  char         *tmp  = 0;
  unsigned int  nr_c = 0;
  ObjectType    type = (ObjectType)0;
  char         *orig = 0;
  char         *comps[64];

  len = readlink (path, dest, 1024);
  if (len <= 0)
    {
      DSMCC_MWARN ("ignoring external reference <%s> (read error).", path);
      return 0;
    }

  if (len >= 1024)
    {
      DSMCC_MWARN ("ignoring external reference <%s> (too long).", path);
      return 0;
    }

  dest[len] = 0;
  orig      = strdup (dest);

  tmp = index (dest, ':');
  if (!tmp)
    {
      DSMCC_MWARN ("ignoring external reference <%s> (only path found).", path);
      return 0;
    }

  (*tmp++) = 0;
  ext = strdup (dest);

  nr_c = dsmcc_get_comp (tmp, comps, ":");
  if (nr_c != 5)
    {
      DSMCC_MWARN ("ignoring external reference <%s> (expected 5 components after path).", path);
      return 0;
    }

  if (!strcmp (comps[0], "dir"))
    {
      type = ot_dir;
    }
  else if (!strcmp (comps[0], "fil"))
    {
      type = ot_fil;
    }
  else if (!strcmp (comps[0], "str"))
    {
      type = ot_str;
    }
  else if (!strcmp (comps[0], "ste"))
    {
      type = ot_ste;
    }
  else
    {
      DSMCC_MWARN ("ignoring external reference <%s> (unknown type <%s>).", path, comps[0]);
      return 0;
    }

  cid = strtol (comps[1], 0, 16);
  if (cid == 0 || cid >= 0xFFFFFFFF)
    {
      DSMCC_MWARN ("ignoring external reference <%s> (invalid cid 0x%08x).", path, cid);
      return 0;
    }

  onid = strtol (comps[2], 0, 16);
  if (onid == 0 || onid >= 0xFFFF)
    {
      DSMCC_MWARN ("ignoring external reference <%s> (invalid onid 0x%04x).", path, onid);
      return 0;
    }

  tsid = strtol (comps[3], 0, 16);
  if (tsid == 0 || tsid >= 0xFFFF)
    {
      DSMCC_MWARN ("ignoring external reference <%s> (invalid tsid 0x%04x).", path, tsid);
      return 0;
    }

  sid = strtol (comps[4], 0, 16);
  if (sid == 0 || sid >= 0xFFFF)
    {
      DSMCC_MWARN ("ignoring external reference <%s> (invalid sid 0x%04x).", path, sid);
      return 0;
    }

  ent->ext = dsmcc_ext_new (orig, ext, cid, onid, tsid, sid);
  if (!ent->ext)
    {
      DSMCC_MERR ("(%s) (%s) (0x%08x) (0x%04x) (0x%04x) (0x%04x)",
                  ext, dsmcc_type_string (type), onid, tsid, sid, cid);
      return 0;
    }

  ent->type = type;
#endif
  return 1;
}


static void
dsmcc_ent_print_stream_info (DsmccEnt *ent)
{
  DsmccLink *link  = 0;
  int        count = 0;
  char      *info1 = 0;

  if (ent->info)
    {
      info1 = (char*)dsmcc_list_pop_front (ent->info);
      DSMCC_MSG ("    desc : <%s>\n", info1);
    }

  DSMCC_MSG ("    type :");
  if (!ent->dmask)
    {
      DSMCC_MSG (" <unknown>");
    }
  if ((ent->dmask & DSMCC_STRI_AUDIO) == DSMCC_STRI_AUDIO)
    {
      DSMCC_MSG (" <audio>");
    }
  if ((ent->dmask & DSMCC_STRI_VIDEO) == DSMCC_STRI_VIDEO)
    {
      DSMCC_MSG (" <video>");
    }
  if ((ent->dmask & DSMCC_STRI_DATA) == DSMCC_STRI_DATA)
    {
      DSMCC_MSG (" <data>");
    }

  DSMCC_MSG ("\n    len  : <%d sec, %d msec>\n", ent->dsec, ent->dmsec);

  if (ent->type == ot_str)
    {
      DSMCC_MSG ("    setup: <prog-tag: 0x%02x> <npt-tag: 0x%02x, timebase: 0x%02x>\n",
                 ent->bptag, ent->npttag, ent->nptbase);
    }
  else
    {
      DSMCC_MSG ("    setup: <prog-tag: 0x%02x> <npt-tag: 0x%02x, timebase: 0x%02x> <ste-tag: 0x%02x>\n",
                 ent->bptag, ent->npttag, ent->nptbase, ent->stetag);
    }

  if (ent->info)
    {
      dsmcc_list_foreach (ent->info, link)
        {
          if (count++ % 2 == 0)
            {
              DSMCC_MSG ("    event: <%s>", (char *) link->data);
            }
          else
            {
              DSMCC_MSG (" <%s>\n", (char *) link->data);
            }
        }

      dsmcc_list_push_front (ent->info, info1);
    }
}


static void
dsmcc_ent_print_ext_info (DsmccEnt *ent)
{
  DSMCC_MSG (" + found external reference <%s>\n", ent->path);
  DSMCC_MSG ("    path : <%s>\n", ent->ext->path);
  DSMCC_MSG ("    type : <%s> at dvb://0x%x.0x%x.0x%x on carousel-id 0x%x.\n",
             dsmcc_type_string (ent->type), ent->ext->onid,
             ent->ext->tsid, ent->ext->sid, ent->ext->cid);
}


static DsmccEnt *
dsmcc_ent_id (char          *dir,
              struct dirent *ent)
{
  DsmccEnt    *ret       = 0;
  char         nbuf[256] = "";
  ObjectType   type      = ot_non;
  struct stat  fst;

  if (!strcmp (ent->d_name, ".") || !strcmp (ent->d_name, ".."))
    {
      return 0;
    }

  strcat (nbuf, dir);
#ifdef WIN32
  strcat (nbuf, "\\");
#else
  strcat (nbuf, "/");
#endif

  strcat (nbuf, ent->d_name);

#ifdef WIN32
  if (stat (nbuf, &fst))
#else
  if (lstat (nbuf, &fst))
#endif
    {
      DSMCC_MWARN ("(%s)", nbuf);
      return 0;
    }

  if (fst.st_size > DSMCC_ENT_MAX_SIZE)
    {
      DSMCC_MWARN ("ignoring too large <%s> with size(%d).", nbuf, (int) fst.st_size);
      return 0;
    }

  type = dsmcc_ent_type (fst.st_mode);
  if (type == ot_non)
    {
      DSMCC_MWARN ("ignoring unknown object-type <%s>.", nbuf);
      return 0;
    }

  ret = (DsmccEnt*)malloc (sizeof (DsmccEnt));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->name    = strdup (ent->d_name);
  ret->rpath   = strdup (nbuf + strlen (dsmcc_conf_get_in()));
  ret->path    = strdup (nbuf);
  ret->size    = 0;
  ret->rlen    = 0;
  ret->type    = type;
  ret->ents    = 0;
  ret->tid     = 0;
  ret->ctag    = DSMCC_ENT_UNDEF_TAG;
  ret->key     = 0;
  ret->mid     = 0;
  ret->cid     = 0;
  ret->info    = 0;
  ret->bptag   = 0;
  ret->npttag  = 0;
  ret->nptbase = 0;
  ret->stetag  = 0;
  ret->dsec    = 0;
  ret->dmsec   = 0;
  ret->dmask   = DSMCC_STRI_DATA;
  ret->ext     = 0;
  ret->crc     = 0;
  ret->exist   = 0;
  ret->update  = 0;
  ret->upent   = 0;
  ret->dcroot  = 0;
  ret->srg     = 0;

  if (type == ot_fil)
    {
#ifdef WIN32
      ret->size = getFileSize(nbuf);
#else
      ret->size = fst.st_size;
#endif
    }

  if (type == ot_str || type == ot_ste)
    {
      if (!dsmcc_ent_stream_format (ret, type, nbuf, fst.st_size))
        {
          free (ret);
          return 0;
        }

      dsmcc_ent_print_stream_info (ret);
    }

  if (type == ot_ext)
    {
      if (!dsmcc_ent_external_format (ret, nbuf, fst.st_size))
        {
          free (ret);
          return 0;
        }

      dsmcc_ent_print_ext_info (ret);
    }

  if (!ret->ext && ret->type != ot_dir)
    {
      ret->crc = dsmcc_crc_file (ret->path);
    }

  return ret;
}


static DsmccList *
dsmcc_ent_list (char *name)
{
  struct dirent **ents = 0;
  DsmccList      *ret  = 0;
  DsmccEnt       *ent  = 0;
  int             n    = 0;

  ret = dsmcc_list_new ();
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  n = scandir (name, &ents, 0, alphasort);
  if (n < 0)
    {
      DSMCC_ERR ();
      free (ret);
      return 0;
    }

  while (n--)
    {
      ent = dsmcc_ent_id (name, ents[n]);
      free (ents[n]);
      if (!ent)
        {
          continue;
        }

      dsmcc_list_push_front (ret, ent);

      if (ent->type == ot_dir && !ent->ext)
        {
          ent->ents = dsmcc_ent_list (ent->path);
        }
    }

  if (dsmcc_list_size (ret) > DSMCC_DIR_MAX_BINDS)
    {
      DSMCC_MERR ("directory %s has too many (%d) bindings (max. %d)",
                  name, dsmcc_list_size (ret), DSMCC_DIR_MAX_BINDS);

      dsmcc_list_free (ret, free);
      return 0;
    }

  free (ents);

  return ret;
}


DsmccEnt *
dsmcc_ent_new (char *name)
{
  DsmccEnt    *ret = 0;
  struct stat  fst;

#ifdef WIN32
  if (stat (name, &fst))
#else
  if (lstat (name, &fst))
#endif
    {
      DSMCC_MWARN ("cannot read <%s>.", name);
      return 0;
    }

  if (!DSMCC_IS_DIR(fst.st_mode))
    {
      DSMCC_MWARN ("<%s> is not a directory.", name);
      return 0;
    }

  ret = (DsmccEnt*)malloc (sizeof (DsmccEnt));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->name    = strdup ("");
  ret->rpath   = strdup ("");
  ret->path    = strdup (name);
  ret->size    = 0;
  ret->rlen    = 0;
  ret->type    = ot_dir;
  ret->tid     = 0;
  ret->ctag    = DSMCC_ENT_UNDEF_TAG;
  ret->key     = 0;
  ret->mid     = 0;
  ret->cid     = 0;
  ret->info    = 0;
  ret->bptag   = 0;
  ret->npttag  = 0;
  ret->nptbase = 0;
  ret->stetag  = 0;
  ret->dsec    = 0;
  ret->dmsec   = 0;
  ret->dmask   = DSMCC_STRI_DATA;
  ret->ext     = 0;
  ret->exist   = 0;
  ret->update  = 0;
  ret->upent   = 0;
  ret->dcroot  = 0;
  ret->srg     = 1;

  ret->ents = dsmcc_ent_list (name);
  if (!ret->ents)
    {
      DSMCC_MERR ("could not create root-object.");
      free (ret);
      return 0;
    }

  return ret;
}


void
dsmcc_ent_free (DsmccEnt *ent)
{
  if (ent->name)
    {
      free (ent->name);
    }

  if (ent->path)
    {
      free (ent->path);
    }

  if (ent->ents)
    {
      dsmcc_list_free (ent->ents, 0);
    }

  if (ent->info)
    {
      dsmcc_list_free (ent->info, free);
    }

  if (ent->ext)
    {
      dsmcc_ext_free (ent->ext);
    }

  free (ent);
}


unsigned int
dsmcc_ent_dump_write (DsmccEnt *ent,
                      int       fd)
{
  unsigned short  rpathlen = strlen (ent->rpath);
  unsigned char   type     = ent->type;
  unsigned short  mid      = ent->mid;
  unsigned short  entsnum  = 0;
  DsmccLink      *link     = 0;
  DsmccEnt       *dent     = 0;

  // ent->ext; // TODO

  if (write (fd, &rpathlen, 2) != 2)
    {
      return 0;
    }

  if (write (fd, ent->rpath, rpathlen) != rpathlen)
    {
      return 0;
    }

  if (write (fd, &type, 1) != 1)
    {
      return 0;
    }

  if (write (fd, &ent->size, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &ent->tid, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &ent->ctag, 1) != 1)
    {
      return 0;
    }

  if (write (fd, &ent->key, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &mid, 2) != 2)
    {
      return 0;
    }

  if (write (fd, &ent->cid, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &ent->crc, 4) != 4)
    {
      return 0;
    }

  if (ent->ents)
    {
      entsnum = dsmcc_list_size (ent->ents);
    }

  if (write (fd, &entsnum, 2) != 2)
    {
      return 0;
    }

  if (ent->ents)
    {
      dsmcc_list_foreach (ent->ents, link)
        {
          dent = (DsmccEnt*)link->data;

          rpathlen = strlen (dent->rpath);
          if (write (fd, &rpathlen, 2) != 2)
            {
              return 0;
            }

          if (write (fd, dent->rpath, rpathlen) != rpathlen)
            {
              return 0;
            }
        }
    }

  return 1;
}


DsmccEnt *
dsmcc_ent_dump_read (int fd)
{
  DsmccEnt       *ent   = 0;
  DsmccList      *ents  = 0;
  unsigned short  rlen  = 0;
  char           *rpath = 0;
  char            buf[1024];
  ObjectType      type  = ot_non;
  unsigned int    size  = 0;
  unsigned int    tid   = 0;
  unsigned char   ctag  = 0;
  unsigned int    key   = 0;
  unsigned short  mid   = 0;
  unsigned int    cid   = 0;
  unsigned int    num   = 0;
  int             i     = 0;
  unsigned int    crc   = 0;
  char           *last  = 0;

  if (read (fd, &rlen, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, buf, rlen) != rlen)
    {
      DSMCC_ERR ();
      return 0;
    }

  buf[rlen] = 0;
  rpath = strdup (buf);

  if (read (fd, &type, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &size, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &tid, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &ctag, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &key, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

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

  if (read (fd, &crc, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &num, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (num > 0)
    {
      ents = dsmcc_list_new ();

      for (i = 0; (unsigned int)i < num; i++)
        {
          if (read (fd, &rlen, 2) != 2)
            {
              DSMCC_ERR ();
              return 0;
            }

          if (read (fd, buf, rlen) != rlen)
            {
              DSMCC_ERR ();
              return 0;
            }

          buf[rlen] = 0;
          dsmcc_list_push_back (ents, strdup (buf));
        }
    }

  ent = (DsmccEnt*)malloc (sizeof (DsmccEnt));
  if (!ent)
    {
      DSMCC_ERR ();
      return 0;
    }

  ent->name    = 0;
  ent->rpath   = rpath;
  ent->path    = 0;
  ent->type    = type;
  ent->size    = size;
  ent->rlen    = 0;
  ent->ents    = ents;
  ent->tid     = tid;
  ent->ctag    = ctag;
  ent->key     = key;
  ent->mid     = mid;
  ent->cid     = cid;
  ent->info    = 0;
  ent->bptag   = 0;
  ent->npttag  = 0;
  ent->nptbase = 0;
  ent->stetag  = 0;
  ent->dsec    = 0;
  ent->dmsec   = 0;
  ent->dmask   = 0;
  ent->ext     = 0;
  ent->crc     = crc;
  ent->exist   = 0;
  ent->update  = 0;
  ent->upent   = 0;

  ent->path = (char*)malloc (strlen (dsmcc_conf_get_in()) + strlen (ent->rpath) + 1);
  strcpy (ent->path, dsmcc_conf_get_in());
  strcat (ent->path, ent->rpath);

  last = strrchr (ent->rpath, '/');
  if (last)
    {
      ent->name = strdup (last);
    }

  return ent;
}
