// global configuration

#include "../../../include/mpeg/dsmcc/conf.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/crc.h"
#include "../../../include/mpeg/dsmcc/def.h"
#include "../../../include/mpeg/dsmcc/srg.h"


static char           *c_in       = 0;
static char           *c_oc_out   = 0;
static char           *c_npt_out  = 0;
static char           *c_ste_out  = 0;
static char           *c_ait_out  = 0;
static char           *c_pmt_out  = 0;
static char           *c_dsi_info = 0;
static unsigned int    c_dsi_ilen = 0;
static unsigned short  c_pid      = 0;
static unsigned int    c_cid      = 0;
static unsigned char   c_tag      = 0;
static unsigned int    c_verb     = 0;
static unsigned int    c_key      = 0;
static unsigned short  c_mid      = 0;
static unsigned int    c_did      = 0;
static unsigned int    c_id       = 0;
static unsigned int    c_up       = 0;
static unsigned int    c_srg_obj  = 0;
static unsigned int    c_srg_rep  = 1;
static unsigned int    c_mrep     = 100;
static unsigned int    c_ste      = 0;
static unsigned int    c_stuff    = 1;
static char           *c_dump_out = 0;
static DsmccList      *c_exps     = 0;
static unsigned short  c_exps_n   = 0;
static DsmccList      *c_apps     = 0;
static DsmccList      *c_esis     = 0;
static unsigned int    c_tid      = 0;


void
dsmcc_conf_init (void)
{
  char *curr = 0;

  dsmcc_crc_init();

  curr = getenv ("PWD");
  if (curr)
    {
      c_in = strdup (curr);
    }
  else
    {
      c_in = strdup (".");
    }

  c_oc_out   = 0;
  c_npt_out  = 0;
  c_ste_out  = 0;
  c_ait_out  = 0;
  c_pmt_out  = 0;
  c_dsi_info = 0;
  c_dsi_ilen = 0;
  c_pid      = 0x0150;
  c_cid      = 0x00000001;
  c_tag      = 0x21;
  c_key      = 0x00000001;
  c_mid      = 0x0017;
  c_up       = 0;
  c_did      = 0x00000001;
  c_id       = 0x00000017;
  c_srg_obj  = 0;
  c_srg_rep  = 1;
  c_mrep     = 100;
  c_ste      = 0;
  c_stuff    = 1;
  c_dump_out = 0;
  c_exps     = dsmcc_list_new ();
  c_exps_n   = 0;
  c_apps     = dsmcc_list_new ();
  c_esis     = dsmcc_list_new ();
  c_tid      = dsmcc_tid (1, c_mid, c_up);
}


void
dsmcc_conf_set_in (char *in)
{
  c_in = in;

  if (c_in[strlen (c_in) - 1] == '/')
    {
      c_in[strlen (c_in) - 1] = 0;
    }
}


char *
dsmcc_conf_get_in (void)
{
  return c_in;
}


static char *
dsmcc_conf_loc (unsigned int oc)
{
  char loc[18];

  if (oc)
    {
      snprintf (loc, 18, "-%08x-%02x-%04x", c_cid, c_tag, c_pid);
    }
  else
    {
      snprintf (loc, 6, "-%04x", c_pid);
    }

  return strdup (loc);
}


void
dsmcc_conf_set_oc_out (char *out)
{
  c_oc_out = out;
}


char *
dsmcc_conf_get_oc_out (void)
{
  char *loc = 0;
  int   len = 0;

  if (!c_oc_out)
    {
      loc = dsmcc_conf_loc (1);
      len = strlen (loc) + 7;

      c_oc_out = (char*)malloc (len);
      c_oc_out[0] = 0;

      strcat (c_oc_out, "oc");
      strcat (c_oc_out, loc);
      strcat (c_oc_out, ".m2t");

      free (loc);
    }

  return c_oc_out;
}


void
dsmcc_conf_set_npt_out (char *out)
{
  c_npt_out = out;
}


char *
dsmcc_conf_get_npt_out (void)
{
  char *loc = 0;

  if (!c_npt_out)
    {
      loc = dsmcc_conf_loc (0);
      c_npt_out = (char*)malloc (strlen (loc) + 8);
      strcpy (c_npt_out, "npt");
      strcat (c_npt_out, loc);
      strcat (c_npt_out, ".m2t");
      free (loc);
    }

  return c_npt_out;
}


void
dsmcc_conf_set_ste_out (char *out)
{
  c_ste_out = out;
}


char *
dsmcc_conf_get_ste_out (void)
{
  char *loc = 0;

  if (!c_ste_out)
    {
      loc = dsmcc_conf_loc (0);
      c_ste_out = (char*)malloc (strlen (loc) + 8);
      strcpy (c_ste_out, "ste");
      strcat (c_ste_out, loc);
      strcat (c_ste_out, ".m2t");
      free (loc);
    }

  return c_ste_out;
}


void
dsmcc_conf_set_ait_out (char *out)
{
  c_ait_out = out;
}


char *
dsmcc_conf_get_ait_out (void)
{
  char *loc = 0;

  if (!c_ait_out)
    {
      loc = dsmcc_conf_loc (0);
      c_ait_out = (char*)malloc (strlen (loc) + 8);
      strcpy (c_ait_out, "ait");
      strcat (c_ait_out, loc);
      strcat (c_ait_out, ".m2t");
      free (loc);
    }

  return c_ait_out;
}


void
dsmcc_conf_set_pmt_out (char *out)
{
  c_pmt_out = out;
}


char *
dsmcc_conf_get_pmt_out (void)
{
  char *loc = 0;

  if (!c_pmt_out)
    {
      loc = dsmcc_conf_loc (0);
      c_pmt_out = (char*)malloc (strlen (loc) + 8);
      strcpy (c_pmt_out, "pmt");
      strcat (c_pmt_out, loc);
      strcat (c_pmt_out, ".m2t");
      free (loc);
    }

  return c_pmt_out;
}


void
dsmcc_conf_set_dsi_info (char *info)
{
  struct stat fst;
  int         fd = 0;

#ifdef WIN32
  if (stat (info, &fst))
#else
  if (lstat (info, &fst))
#endif
    {
      DSMCC_MERR ("(%s)", info);
      return;
    }

  if (fst.st_size > DSMCC_SRG_MAX_USERLEN)
    {
      DSMCC_MERR ("(%s)", info);
      return;
    }

  fd = open (info, O_RDONLY);
  if (fd < 0)
    {
      DSMCC_MERR ("(%s)", info);
      return;
    }

  c_dsi_info = (char*)malloc (fst.st_size);
  if (!c_dsi_info)
    {
      DSMCC_MERR ("(%s)", info);
      close (fd);
      return;
    }

  c_dsi_ilen = fst.st_size;

  if (read (fd, c_dsi_info, fst.st_size) < 0)
    {
      DSMCC_MERR ("(%s)", info);
      free (c_dsi_info);
      c_dsi_info = 0;
      c_dsi_ilen = 0;
      close (fd);
      return;
    }

  close (fd);
}


char *
dsmcc_conf_get_dsi_info (void)
{
  return c_dsi_info;
}


unsigned int
dsmcc_conf_get_dsi_info_len (void)
{
  return c_dsi_ilen;
}


unsigned int
dsmcc_conf_set_pid (unsigned int pid)
{
  c_pid = pid;

  return 1;
}


unsigned short
dsmcc_conf_get_pid (void)
{
  return c_pid;
}


unsigned int
dsmcc_conf_set_cid (unsigned int cid)
{
  c_cid = cid;

  return 1;
}


unsigned int
dsmcc_conf_get_cid (void)
{
  return c_cid;
}


unsigned int
dsmcc_conf_set_tag (unsigned char tag)
{
  c_tag = tag;

  return 1;
}


unsigned char
dsmcc_conf_get_tag (void)
{
  return c_tag;
}


void
dsmcc_conf_set_verb (unsigned int verb)
{
  c_verb = verb;
}


unsigned int
dsmcc_conf_get_verb (void)
{
  return c_verb;
}


void
dsmcc_conf_set_srg_obj (unsigned int obj)
{
  c_srg_obj = obj;
}


unsigned int
dsmcc_conf_get_srg_obj (void)
{
  return c_srg_obj;
}


void
dsmcc_conf_set_srg_rep (unsigned int rep)
{
  c_srg_rep = rep;
}


unsigned int
dsmcc_conf_get_srg_rep (void)
{
  return c_srg_rep;
}


void
dsmcc_conf_set_mount_rep (unsigned int rep)
{
  c_mrep = rep;
}


unsigned int
dsmcc_conf_get_mount_rep (void)
{
  return c_mrep;
}


void
dsmcc_conf_set_ste (unsigned int ste)
{
  c_ste = ste;
}


unsigned int
dsmcc_conf_get_ste (void)
{
  return c_ste;
}


void
dsmcc_conf_set_stuff (unsigned int stuff)
{
  c_stuff = stuff;
}


unsigned int
dsmcc_conf_get_stuff (void)
{
  return c_stuff;
}


void
dsmcc_conf_set_exps (DsmccList *exps)
{
  c_exps = exps;
}


DsmccList *
dsmcc_conf_get_exps (void)
{
  return c_exps;
}


unsigned short
dsmcc_conf_get_exps_num (void)
{
  return c_exps_n;
}


void
dsmcc_conf_set_apps (DsmccList *apps)
{
  c_apps = apps;
}


DsmccList *
dsmcc_conf_get_apps (void)
{
  return c_apps;
}


void
dsmcc_conf_set_esis (DsmccList *esis)
{
  c_esis = esis;
}


DsmccList *
dsmcc_conf_get_esis (void)
{
  return c_esis;
}


unsigned int
dsmcc_conf_get_key (void)
{
  return c_key++;
}


unsigned short
dsmcc_conf_get_mid (void)
{
  return c_mid++;
}


unsigned int
dsmcc_conf_set_did (unsigned int did)
{
  c_did = did;

  return 1;
}


unsigned int
dsmcc_conf_get_did (void)
{
  return c_did;
}


unsigned int
dsmcc_conf_get_id (void)
{
  return c_id++;
}


void
dsmcc_conf_set_up (void)
{
  c_up = 1;
}


unsigned int
dsmcc_conf_get_up (void)
{
  return c_up;
}


void
dsmcc_conf_set_dump_out (char *out)
{
  c_dump_out = out;
}


char *
dsmcc_conf_get_dump_out (void)
{
  char *loc = 0;

  if (!c_dump_out)
    {
      loc = dsmcc_conf_loc (1);
      c_dump_out = (char*)malloc (strlen (loc) + 8);
      strcpy (c_dump_out, "oc");
      strcat (c_dump_out, loc);
      strcat (c_dump_out, ".dump");
      free (loc);
    }

  return c_dump_out;
}


void
dsmcc_conf_reset_dc (unsigned short pid,
                     unsigned char  ctag)
{
  c_pid = pid;
  c_tag = ctag;
}


unsigned int
dsmcc_conf_dump_write (int fd)
{
  unsigned short inlen  = strlen (c_in);
  unsigned short exps_n = dsmcc_list_size (c_exps);

  if (write (fd, &inlen, 2) != 2)
    {
      return 0;
    }

  if (write (fd, c_in, inlen) != inlen)
    {
      return 0;
    }

  if (write (fd, &exps_n, 2) != 2)
    {
      return 0;
    }

  if (write (fd, &c_pid, 2) != 2)
    {
      return 0;
    }

  if (write (fd, &c_cid, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &c_tag, 1) != 1)
    {
      return 0;
    }

  if (write (fd, &c_did, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &c_key, 4) != 4)
    {
      return 0;
    }

  if (write (fd, &c_mid, 2) != 2)
    {
      return 0;
    }

  if (write (fd, &c_id, 4) != 4)
    {
      return 0;
    }

  return 1;
}


unsigned int
dsmcc_conf_dump_read (int fd)
{
  char            buf[4096];
  unsigned short  len    = 0;
  char           *oc_in  = 0;
  unsigned short  exps_n = 0;
  unsigned short  pid    = 0;
  unsigned int    cid    = 0;
  unsigned char   tag    = 0;
  unsigned int    did    = 0;
  unsigned int    key    = 0;
  unsigned short  mid    = 0;
  unsigned int    id     = 0;

  if (read (fd, &len, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, buf, len) != len)
    {
      DSMCC_ERR ();
      return 0;
    }

  buf[len] = 0;
  oc_in = strdup (buf);

  if (read (fd, &exps_n, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &pid, 2) != 2)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &cid, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &tag, 1) != 1)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (read (fd, &did, 4) != 4)
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

  if (read (fd, &id, 4) != 4)
    {
      DSMCC_ERR ();
      return 0;
    }

  c_in     = oc_in;
  c_exps_n = exps_n;
  c_pid    = pid;
  c_cid    = cid;
  c_tag    = tag;
  c_did    = did;
  c_key    = key;
  c_mid    = mid;
  c_id     = id;

  return 1;
}

void
dsmcc_conf_set_tid (int tid)
{
    c_tid = tid;
}

int
dsmcc_conf_get_tid ()
{
    return c_tid;
}
