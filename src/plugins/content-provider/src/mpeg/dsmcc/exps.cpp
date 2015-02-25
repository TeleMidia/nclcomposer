// extra pids specification

#include "../../../include/mpeg/dsmcc/exps.h"

#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/pes.h"
#include "../../../include/mpeg/dsmcc/util.h"


#define DSMCC_EXPS_COMPS 3


DsmccExps *
dsmcc_exps_new (char *spec)
{
  DsmccExps    *ret = 0;
  int           num = 0;
  unsigned int  par = 0;
  char         *comps[256];
  char          dpath[256];

  if (!spec)
    {
      DSMCC_MERR ("empty exp-spec line.");
      return 0;
    }

  ret = (DsmccExps*)malloc (sizeof (DsmccExps));
  if (!ret)
    {
      DSMCC_MERR ("(%s)", spec);
      return 0;
    }

  num = dsmcc_get_comp (spec, comps, ":");
  if (num != DSMCC_EXPS_COMPS)
    {
      DSMCC_MERR ("malformed exp-spec line (expected %d components).",
                  DSMCC_EXPS_COMPS);
      free (ret);
      return 0;
    }

#ifdef WIN32
  if (comps[0][0] != '\\')
    {
      strcpy (dpath, "\\");
      strcat (dpath, comps[0]);
    }
  else
    {
      strcpy (dpath, comps[0]);
    }

#else
  if (comps[0][0] != '/')
    {
      strcpy (dpath, "/");
      strcat (dpath, comps[0]);
    }
  else
    {
      strcpy (dpath, comps[0]);
    }  
#endif

  if (dpath[strlen (dpath) - 1] == '/')
    {
      dpath[strlen (dpath) - 1] = 0;
    }

  ret->path = strdup (dpath);

  par = strtol (comps[1], 0, 16);
  if (par < DSMCC_PES_MIN_PID || par > DSMCC_PES_MAX_PID)
    {
      DSMCC_MERR ("bad es-pid 0x%08x, range [0x%04x-0x%04x]).",
                  par, DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);
      free (ret);
      return 0;
    }
  ret->pid = par;

  par = strtol (comps[2], 0, 16);
  if (par > 0xFE)
    {
      DSMCC_MERR ("bad es-component-tag 0x%08x, range [0-0xFE]).", par);
      free (ret);
      return 0;
    }

  ret->ctag = par;
  ret->root = 0;

  return ret;
}


DsmccExps *
dsmcc_exps_new_intern (unsigned short  pid,
                       unsigned char   ctag,
                       char           *path)
{
  DsmccExps *ret = 0;

  ret = (DsmccExps*)malloc (sizeof (DsmccExps));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->pid  = pid;
  ret->ctag = ctag;
  ret->path = path;
  ret->root = 1;

  return ret;
}


void
dsmcc_exps_print (DsmccExps *exps)
{
  DSMCC_MSG (" + expspec: path <%s>\n", exps->path);
  DSMCC_MSG ("            pid  <0x%04x>\n", exps->pid);
  DSMCC_MSG ("            ctag <0x%02x>\n", exps->ctag);
}


void
dsmcc_exps_free (DsmccExps *exps)
{
  free (exps);
}


unsigned int
dsmcc_exps_dump_write (DsmccExps *exps,
                       int        fd)
{
  unsigned short pathlen = 0;

  if (write (fd, &exps->pid, 2) != 2)
    {
      return 0;
    }

  if (write (fd, &exps->ctag, 1) != 1)
    {
      return 0;
    }

  if (write (fd, &exps->root, 1) != 1)
    {
      return 0;
    }

  pathlen = strlen (exps->path);
  if (write (fd, &pathlen, 2) != 2)
    {
      return 0;
    }

  if (write (fd, exps->path, pathlen) != pathlen)
    {
      return 0;
    }

  return 1;
}


DsmccExps *
dsmcc_exps_dump_read (int fd)
{
  DsmccExps      *ret  = 0;
  unsigned short  plen = 0;
  char           *path = 0;
  char            buf[1024];
  unsigned short  pid  = 0;
  unsigned char   ctag = 0;
  unsigned char   root = 0;

  if (read (fd, &pid, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &ctag, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &root, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &plen, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, buf, plen) != plen)
    {
      DSMCC_ERR ();
      return 0;
    }

  buf[plen] = 0;
  path = strdup (buf);

  ret = (DsmccExps*)malloc (sizeof (DsmccExps));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->pid  = pid;
  ret->ctag = ctag;
  ret->root = root;
  ret->path = path;

#ifdef WIN32
  if (!strcmp (ret->path, "\\"))
#else
  if (!strcmp (ret->path, "/"))
#endif
    {
      free (ret->path);
      ret->path = strdup ("");
    }

  return ret;
}
