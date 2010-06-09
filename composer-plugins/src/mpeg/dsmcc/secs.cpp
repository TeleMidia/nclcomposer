// dsmcc private section management

#include "../../../include/mpeg/dsmcc/secs.h"

#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/dii.h"
#include "../../../include/mpeg/dsmcc/msec.h"
#include "../../../include/mpeg/dsmcc/sec.h"


DsmccSecs *
dsmcc_secs_new (DsmccDsi  *dsi,
                DsmccDiis *diis,
                DsmccMods *mods)
{
  DsmccSecs *ret = 0;

  ret = (DsmccSecs*)malloc (sizeof (DsmccSecs));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->msecs = dsmcc_list_new ();
  if (!ret->msecs)
    {
      DSMCC_ERR ();
      free (ret);

      return 0;
    }

  ret->dsecs = dsmcc_list_new ();
  if (!ret->dsecs)
    {
      DSMCC_ERR ();
      dsmcc_list_free (ret->msecs, 0);
      free (ret);

      return 0;
    }

  ret->ssecs = 0;
  ret->dsi   = dsi;
  ret->diis  = diis;
  ret->mods  = mods;

  return ret;
}


void
dsmcc_secs_free (DsmccSecs *secs)
{
  if (secs->dsecs)
    {
      dsmcc_list_free (secs->dsecs, (FreeFunc) dsmcc_sec_free);
    }

  if (secs->msecs)
    {
      dsmcc_list_free (secs->msecs, (FreeFunc) dsmcc_msec_free);
    }

  if (secs->ssecs)
    {
      dsmcc_list_free (secs->ssecs, (FreeFunc) dsmcc_sec_free);
    }

  free (secs);
}


unsigned int 
dsmcc_secs_encode (DsmccSecs *secs)
{
  DsmccLink    *link = 0;
  DsmccMod     *mod  = 0;
  DsmccMsec    *msec = 0;
  DsmccDii     *dii  = 0;
  DsmccSec     *sec  = 0;
  unsigned int  tid  = 0;

  if (secs->dsi)
    {
      tid = dsmcc_tid (secs->dsi->ver, secs->dsi->id, secs->dsi->up);

      sec = dsmcc_sec_new (ot_non,
                           (tid & 0x0000FFFF),
                           0,
                           0,
                           0,
                           secs->dsi->dsi,
                           secs->dsi->size,
                           0,
                           0,
                           0,
                           0,
                           0);
      if (!sec)
        {
          DSMCC_MERR ("(0x%08x) (%d)", tid, secs->dsi->size);
          return 0;
        }

      dsmcc_list_push_back (secs->dsecs, sec);
    }

  dsmcc_list_foreach (secs->diis->diis, link)
    {
      dii = (DsmccDii*)link->data;

      tid = dsmcc_tid (dii->ver, dii->id, dii->up);

      sec = dsmcc_sec_new (ot_non,
                           (tid & 0x0000FFFF),
                           0,
                           0,
                           0,
                           dii->dii,
                           dii->size,
                           0,
                           0,
                           0,
                           0,
                           0);
      if (!sec)
        {
          DSMCC_MERR ("(0x%08x) (%d)", tid, dii->size);
          return 0;
        }

      dsmcc_list_push_back (secs->dsecs, sec);
    }

  mod = secs->mods->srg;

  msec = dsmcc_msec_new (mod);
  if (!msec)
    {
      DSMCC_MERR ("(%d)", mod->mid);
      return 0;
    }

  if (!dsmcc_msec_encode (msec))
    {
      DSMCC_MERR ("(0x%04x)", mod->mid);
      return 0;
    }

  dsmcc_list_push_front (secs->msecs, msec);

  dsmcc_list_foreach (secs->mods->mods, link)
    {
      mod = (DsmccMod*)link->data;

      msec = dsmcc_msec_new (mod);
      if (!msec)
        {
          DSMCC_MERR ("(0x%04x)", mod->mid);
          return 0;
        }

      if (!dsmcc_msec_encode (msec))
        {
          DSMCC_MERR ("(0x%04x)", mod->mid);
          return 0;
        }

      dsmcc_list_push_back (secs->msecs, msec);
    }

  if (dsmcc_conf_get_ste ())
    {
      if (!dsmcc_stdb_encode (secs->mods->stdb))
        {
          DSMCC_ERR ();

          return 0;
        }
    }

  secs->ssecs = secs->mods->stdb->secs;
  secs->mods->stdb->secs = 0;

  return 1;
}


static unsigned int
dsmcc_secs_insert_msg (DsmccPes  *pes,
                       DsmccList *msg,
                       DsmccMsec *srg,
                       int        fd,
                       int        init)
{
  DsmccLink *link = 0;
  DsmccSec  *sec  = 0;

  dsmcc_list_foreach (msg, link)
    {
      sec = (DsmccSec*)link->data;

      if (!dsmcc_pes_encode (pes, sec->data, sec->len, fd))
        {
          DSMCC_MERR ("(0x%04x) (0x%02x) (0x%04x) (0x%02x)",
                      pes->pid, sec->tid, sec->tide, sec->num);
          return 0;
        }
    }

  if (!init && !dsmcc_conf_get_srg_rep ())
    {
      return 1;
    }

  if (srg)
    {
      dsmcc_list_foreach (srg->secs, link)
        {
          sec = (DsmccSec*)link->data;
          sec = dsmcc_sec_complete (sec);

          if (!dsmcc_pes_encode (pes, sec->data, sec->len, fd))
            {
              DSMCC_MERR ("(0x%04x) (0x%02x) (0x%04x) (0x%02x)",
                          pes->pid, sec->tid, sec->tide, sec->num);
              return 0;
            }
        }
    }

  return 1;
}


static unsigned int
dsmcc_secs_insert_ste (DsmccPes     *pes,
                       DsmccList    *ste,
                       unsigned int  up,
                       int           fd)
{
  DsmccLink *link = 0;
  DsmccSec  *sec  = 0;

  dsmcc_list_foreach (ste, link)
    {
      sec = (DsmccSec*)link->data;

      if (up)
        {
          dsmcc_sec_update (sec);
        }

      if (!dsmcc_pes_encode (pes, sec->data, sec->len, fd))
        {
          DSMCC_MERR ("(0x%04x) (0x%02x) (0x%04x) (0x%02x) (0x%02x)",
                      pes->pid, sec->tid, sec->tide, sec->num, sec->ver);
          return 0;
        }
    }

  return 1;
}


unsigned int
dsmcc_secs_encode_pes (DsmccSecs *secs,
                       DsmccPes  *pes)
{
  DsmccMsec *msec = 0;
  DsmccSec  *sec  = 0;
  int        fd   = 0;
  int        cnt  = 1;
  DsmccList *msg  = 0;
  DsmccMsec *srg  = 0;
  DsmccList *ste  = 0;

#ifndef WIN32
  fd = open (pes->pes, O_WRONLY | O_CREAT | O_TRUNC, 0644);
#else
  fd = open (pes->pes, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
#endif
  if (fd < 0)
    {
      DSMCC_MERR ("(0x%04x)", pes->pid);
      return 0;
    }

  msg = secs->dsecs;
  ste = secs->ssecs;

  if (secs->mods->dcc->exps->root)
    {
      srg = (DsmccMsec*)dsmcc_list_pop_front (secs->msecs);
    }

  if (!dsmcc_secs_insert_msg (pes, msg, srg, fd, 1))
    {
      DSMCC_MERR ("(0x%04x)", pes->pid);
      close (fd);
      dsmcc_msec_free (srg);

      return 0;
    }

  if (dsmcc_conf_get_ste ())
    {
      if (!dsmcc_secs_insert_ste (pes, ste, 0, fd))
        {
          DSMCC_MERR ("(0x%04x)", pes->pid);
          close (fd);

          return 0;
        }
    }

  while (dsmcc_list_size (secs->msecs) > 0)
    {
      msec = (DsmccMsec*)dsmcc_list_pop_front (secs->msecs);

      while (dsmcc_list_size (msec->secs) > 0)
        {
          if ((cnt % dsmcc_conf_get_mount_rep ()) == 0)
            {
              if (!dsmcc_secs_insert_msg (pes, msg, srg, fd, 0))
                {
                  close (fd);
                  if (srg)
                    {
                      dsmcc_msec_free (srg);
                    }
                  DSMCC_MERR ("(0x%04x) (0x%02x) (0x%04x) (0x%02x)",
                              pes->pid, sec->tid, sec->tide, sec->num);

                  return 0;
                }
            }

          sec = (DsmccSec*)dsmcc_sec_complete ((DsmccSec*)dsmcc_list_pop_front (msec->secs));
          if (!sec)
            {
              DSMCC_MERR ("(0x%04x) (0x%02x) (0x%04x) (0x%02x)",
                          pes->pid, sec->tid, sec->tide, sec->num);
              close (fd);
              if (srg)
                {
                  dsmcc_msec_free (srg);
                }

              return 0;
            }

          if (!dsmcc_pes_encode (pes, sec->data, sec->len, fd))
            {
              DSMCC_MERR ("(0x%04x) (0x%02x) (0x%04x) (0x%02x)",
                          pes->pid, sec->tid, sec->tide, sec->num);
              close (fd);
              if (srg)
                {
                  dsmcc_msec_free (srg);
                }

              return 0;
            }

          dsmcc_sec_free (sec);

          cnt++;
        }

      dsmcc_msec_free (msec);
    }

  if (dsmcc_conf_get_ste ())
    {
      if (!dsmcc_secs_insert_ste (pes, ste, 1, fd))
        {
          DSMCC_MERR ("(0x%04x)", pes->pid);
          close (fd);

          return 0;
        }
    }

  if (dsmcc_conf_get_stuff ())
    {
      dsmcc_pes_stuff (pes, fd);
    }

  close (fd);

  if (srg)
    {
      dsmcc_msec_free (srg);
    }

  return 1;
}
