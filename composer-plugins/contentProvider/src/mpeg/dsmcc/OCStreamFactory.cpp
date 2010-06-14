// object carousel encoding

#include "../../../include/mpeg/dsmcc/OCStreamFactory.h"

std::map<std::string, ObjectInfo*>* OCStreamFactory::objectsIor = NULL;
unsigned int  OCStreamFactory::dsmcc_oc_dump = 0;
DsmccList*    OCStreamFactory::dsmcc_ext_pids = 0;
unsigned int  OCStreamFactory::dsmcc_srg_obj = 0;

void OCStreamFactory::dsmcc_oc_conf(void) {
  DsmccLink *link = 0;
  DsmccExps *exps = 0;

  DSMCC_MSG (" conf: in    = <%s>\n", dsmcc_conf_get_in());
  DSMCC_MSG (" conf: out   = <%s>\n", dsmcc_conf_get_oc_out());
  DSMCC_MSG (" conf: pid   = <0x%04x>\n", dsmcc_conf_get_pid());
  DSMCC_MSG (" conf: cid   = <0x%08x>\n", dsmcc_conf_get_cid());
  DSMCC_MSG (" conf: tag   = <0x%02x>\n", dsmcc_conf_get_tag());
  DSMCC_MSG (" conf: did   = <0x%08x>\n", dsmcc_conf_get_did());
  DSMCC_MSG (" conf: mrep  = <%d>\n", dsmcc_conf_get_mount_rep());

  if (dsmcc_conf_get_ste ())
    {
      DSMCC_MSG (" conf: ste   = <yes>\n");
    }
  else
    {
      DSMCC_MSG (" conf: ste   = <no>\n");
    }
  if (dsmcc_conf_get_stuff ())
    {
      DSMCC_MSG (" conf: stuff = <yes>\n");
    }
  else
    {
      DSMCC_MSG (" conf: stuff = <no>\n");
    }
  if (dsmcc_oc_dump)
    {
      DSMCC_MSG (" conf: dump  = <%s>\n", dsmcc_conf_get_dump_out());
    }
  else
    {
      DSMCC_MSG (" conf: dump  = <no>\n");
    }
  DSMCC_MSG ("\n");

  if (dsmcc_list_size (dsmcc_conf_get_exps ()) > 0)
    {
      dsmcc_list_foreach (dsmcc_conf_get_exps (), link)
        {
          exps = (DsmccExps*)link->data;

          dsmcc_exps_print (exps);
        }
      DSMCC_MSG ("\n");
    }
}

std::map<std::string, ObjectInfo*>* OCStreamFactory::getObjectsIor() {
	return OCStreamFactory::objectsIor;
}

void OCStreamFactory::createOCStream(
		std::string directoryUri,
		std::string streamUri,
		unsigned int pid,
		unsigned int serviceDomainId,
		unsigned int componentTag) {

  DsmccDsi       *dsi     = 0;
  unsigned int    dsi_key = 0;
  unsigned short  dsi_mid = 0;
  DsmccDii       *dii     = 0;
  DsmccDiis      *diis    = 0;
  DsmccLink      *link    = 0;
  DsmccLink      *link_s  = 0;
  DsmccEnts      *ents    = 0;
  DsmccMods      *mods    = 0;
  DsmccMsec      *msec    = 0;
  DsmccSecs      *secs    = 0;
  DsmccPes       *pes     = 0;
  DsmccDccs      *dccs    = 0;
  DsmccDcc       *dcc     = 0;
  DsmccDcc       *srg_dcc = 0;
  unsigned int    secs_nr = 0;
  int             dump    = 0;

  dsmcc_conf_init();
  objectsIor = new std::map<std::string, ObjectInfo*>;

  //TODO: implement object event in object carousel generator

  dsmcc_ext_pids = dsmcc_list_new();

  //in
  dsmcc_conf_set_in((char*)directoryUri.c_str());

  //out
  dsmcc_conf_set_oc_out((char*)streamUri.c_str());

  //pid
  dsmcc_conf_set_pid(pid);

  //cid
  dsmcc_conf_set_cid(serviceDomainId);
  dsmcc_conf_set_did(dsmcc_conf_get_cid());

  //tag
  dsmcc_conf_set_tag(componentTag);

  dsmcc_conf_set_exps(dsmcc_ext_pids);

  if (dsmcc_conf_get_pid() < DSMCC_PES_MIN_PID ||
          dsmcc_conf_get_pid() > DSMCC_PES_MAX_PID) {

      DLMSG ("\n ----> error: pes pid-range [0x%04x-0x%04x].\n\n",
             DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);

      return;
  }

  if (dsmcc_conf_get_cid() < DSMCC_PES_MIN_CID ||
         dsmcc_conf_get_cid() > DSMCC_PES_MAX_CID) {

      DLMSG("\n ----> error: carousel-id range [0x%08x-0x%08x].\n\n",
             DSMCC_PES_MIN_CID, DSMCC_PES_MAX_CID);

      return;
  }

  DLMSG ("\n");

  dsmcc_oc_conf();

  dsmcc_size_init();

  DSMCC_MSG (" --> creating ents...\n");
  ents = dsmcc_ents_new (dsmcc_conf_get_in());
  if (!ents)
    {
      DLMSG ("\n ----> error while creating ents!\n\n");
      return;
    }

  DSMCC_MSG ("\n --> creating DataCarousels...\n");
  dccs = dsmcc_dccs_new (ents);
  if (!dccs)
    {
      DLMSG ("\n ----> error while creating DataCarousels!\n\n");
      return;
    }

  if (!dsmcc_dccs_ents (dccs))
    {
      DLMSG ("\n ----> error while creating DataCarousels!\n\n");
      return;
    }

  dsmcc_dccs_show (dccs);

  srg_dcc = (DsmccDcc*)dsmcc_list_pop_front (dccs->dccs);

  if (dsmcc_oc_dump)
    {
#ifndef WIN32
      dump = open (dsmcc_conf_get_dump_out(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
#else
      dump = open (dsmcc_conf_get_dump_out(), O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
#endif
      if (dump < 0)
        {
          DLMSG ("\n ----> error while opening dump file!\n\n");
          return;
        }
        
      if (!dsmcc_conf_dump_write (dump))
        {
          DLMSG ("\n ----> error while dumping conf!\n\n");
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
          return;
        }
    }

  dsmcc_list_foreach (dccs->dccs, link)
    {
      dcc = (DsmccDcc*)link->data;

      dsmcc_conf_reset_dc (dcc->exps->pid, dcc->exps->ctag);
      dsmcc_conf_set_srg_obj (1);
      dsmcc_conf_set_srg_rep (0);

      secs_nr = 0;

      DSMCC_MSG ("\n --> creating mods for pid <0x%04x> ...\n",
                 dsmcc_conf_get_pid());

      mods = dsmcc_mods_new (dcc);
      if (!mods)
        {
          DLMSG ("\n ----> error while creating mods!\n\n");
          if (dsmcc_oc_dump)
            {
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
            }
          return;
        }

      if (!dsmcc_mods_ents (mods))
        {
          DLMSG ("\n ----> error while creating mods!\n\n");
          if (dsmcc_oc_dump)
            {
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
            }
          return;
        }

      DSMCC_MSG ("\n --> creating diis for pid <0x%04x> ...\n",
                 dsmcc_conf_get_pid());

      diis = dsmcc_diis_new (mods);
      if (!diis)
        {
          DLMSG ("\n ----> error while creating diis!\n\n");
          if (dsmcc_oc_dump)
            {
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
            }
          return;
        }

      if (!dsmcc_diis_encode (diis))
        {
          DLMSG ("\n ----> error while encoding diis!\n\n");
          if (dsmcc_oc_dump)
            {
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
            }
          return;
        }

      DSMCC_MSG ("\n --> successfully encoded %d diis.\n",
                 dsmcc_list_size (diis->diis));

      DSMCC_MSG (" --> encoding mods...\n");

      if (!dsmcc_mods_encode (mods))
        {
          DLMSG ("\n ----> error while encoding mods!\n\n");
          if (dsmcc_oc_dump)
            {
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
            }
          return;
        }

      DSMCC_MSG (" --> successfully encoded %d mods!\n",
                 dsmcc_list_size (mods->mods) + 1);

      if (dsmcc_oc_dump)
        {
          if (!dsmcc_dsi_dump_write (0, dump, 0))
            {
              DLMSG ("\n ----> error while dumping dsi!\n\n");
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
              return;
            }
        }

      if (dsmcc_oc_dump)
        {
          if (!dsmcc_diis_dump_write (diis, dump))
            {
              DLMSG ("\n ----> error while dumping diis!\n\n");
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
              return;
            }
        }

      DSMCC_MSG ("\n --> creating secs...\n");

      secs = dsmcc_secs_new (0, diis, mods);
      if (!secs)
        {
          DLMSG ("\n ----> error while creating secs!\n\n");
          if (dsmcc_oc_dump)
            {
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
            }
          return;
        }

      if (!dsmcc_secs_encode (secs))
        {
          DLMSG ("\n ----> error while encoding secs!\n\n");
          if (dsmcc_oc_dump)
            {
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
            }
          return;
        }

      if (dsmcc_oc_dump)
        {
          if (!dsmcc_mods_dump_write (mods, dump))
            {
              DLMSG ("\n ----> error while dumping mods!\n\n");
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
              return;
            }
        }

      dsmcc_list_foreach (secs->dsecs, link_s)
        {
          secs_nr++;
        }

      dsmcc_list_foreach (secs->msecs, link_s)
        {
          msec = (DsmccMsec*)link_s->data;

          secs_nr += dsmcc_list_size (msec->secs);
        }

      secs_nr += dsmcc_list_size (secs->ssecs);

      DSMCC_MSG ("\n --> successfully created %d secs.\n", secs_nr);

      DSMCC_MSG ("\n --> creating pes on pid <0x%04x> in <%s>.\n",
                 dsmcc_conf_get_pid(), dsmcc_conf_get_oc_out());

      pes = dsmcc_pes_new (dsmcc_conf_get_oc_out(), dsmcc_conf_get_pid());
      if (!pes)
        {
          DLMSG ("\n ----> error while creating pes!\n\n");
          if (dsmcc_oc_dump)
            {
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
            }
          return;
        }

      if (!dsmcc_secs_encode_pes (secs, pes))
        {
          DLMSG ("\n ----> error while encoding pes!\n\n");
          if (dsmcc_oc_dump)
            {
              close (dump);
              unlink (dsmcc_conf_get_dump_out());
            }
          return;
        }

      DSMCC_MSG ("\n");

      DLMSG (" --> successfully encoded <%s%s> into <%s>.\n",
             dsmcc_conf_get_in(), dcc->exps->path, pes->pes);

      dsmcc_secs_free (secs);
      dsmcc_mods_free (mods);
      dsmcc_pes_free (pes);
    }

  dcc = srg_dcc;

  dsmcc_conf_reset_dc (dcc->exps->pid, dcc->exps->ctag);
  dsmcc_conf_set_srg_obj (dsmcc_srg_obj);
  dsmcc_conf_set_srg_rep (1);

  secs_nr = 0;

  DSMCC_MSG ("\n --> creating mods for pid <0x%04x> ...\n", dcc->exps->pid);
  mods = dsmcc_mods_new (dcc);
  if (!mods)
    {
      DLMSG ("\n ----> error while creating mods!\n\n");
      return;
    }

  if (!dsmcc_mods_ents (mods))
    {
      DLMSG ("\n ----> error while creating mods!\n\n");
      return;
    }

  DSMCC_MSG ("\n --> creating diis...\n");

  diis = dsmcc_diis_new (mods);
  if (!diis)
    {
      DLMSG ("\n ----> error while creating diis!\n\n");
      if (dsmcc_oc_dump)
        {
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
        }
      return;
    }

  if (!dsmcc_diis_encode (diis))
    {
      DLMSG ("\n ----> error while encoding diis!\n\n");
      if (dsmcc_oc_dump)
        {
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
        }
      return;
    }

  DSMCC_MSG ("\n --> successfully encoded %d diis.\n",
             dsmcc_list_size (diis->diis));

  DSMCC_MSG (" --> encoding mods...\n");

  if (!dsmcc_mods_encode (mods))
    {
      DLMSG ("\n ----> error while encoding mods!\n\n");
      return;
    }

  dsi_mid = ents->root->mid;
  dsi_key = ents->root->key;

  DSMCC_MSG (" --> successfully encoded %d mods!\n",
             dsmcc_list_size (mods->mods) + 1);

  dii = (DsmccDii*)dsmcc_list_front (diis->diis);

  DSMCC_MSG ("\n --> creating dsi...\n");

  dsi = dsmcc_dsi_new (1, 0, 0, dsmcc_conf_get_tag(),
                       dsi_key, dsi_mid, dsmcc_conf_get_cid(),
                       dsmcc_tid (dii->ver, dii->id, dii->up));
  if (!dsi)
    {
      DLMSG ("\n ----> error while creating dsi!\n\n");
      return;
    }

  DSMCC_MSG ("\n --> successfully created dsi with tid <0x%08x> (%d bytes).\n",
             dsmcc_tid (dsi->ver, dsi->id, dsi->up), dsi->size);

  if (dsmcc_oc_dump)
    {
      if (!dsmcc_dsi_dump_write (dsi, dump, 1))
        {
          DLMSG ("\n ----> error while dumping dsi!\n\n");
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
          return;
        }

      if (!dsmcc_diis_dump_write (diis, dump))
        {
          DLMSG ("\n ----> error while dumping diis!\n\n");
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
          return;
        }
    }

  DSMCC_MSG ("\n --> creating secs...\n");

  secs = dsmcc_secs_new (dsi, diis, mods);
  if (!secs)
    {
      DLMSG ("\n ----> error while creating secs!\n\n");
      if (dsmcc_oc_dump)
        {
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
        }
      return;
    }

  if (!dsmcc_secs_encode (secs))
    {
      DLMSG ("\n ----> error while encoding secs!\n\n");
      if (dsmcc_oc_dump)
        {
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
        }
      return;
    }

  if (dsmcc_oc_dump)
    {
      if (!dsmcc_mods_dump_write (mods, dump))
        {
          DLMSG ("\n ----> error while dumping mods!\n\n");
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
          return;
        }
    }

  dsmcc_list_foreach (secs->dsecs, link)
    {
      secs_nr++;
    }

  dsmcc_list_foreach (secs->msecs, link)
    {
      msec = (DsmccMsec*)link->data;

      secs_nr += dsmcc_list_size (msec->secs);
    }

  secs_nr += dsmcc_list_size (secs->ssecs);

  DSMCC_MSG ("\n --> successfully created %d secs.\n", secs_nr);

  DSMCC_MSG ("\n --> creating pes on pid <0x%04x> in <%s>.\n",
             dsmcc_conf_get_pid(), dsmcc_conf_get_oc_out());

  pes = dsmcc_pes_new (dsmcc_conf_get_oc_out(), dsmcc_conf_get_pid());
  if (!pes)
    {
      DLMSG ("\n ----> error while creating pes!\n\n");
      if (dsmcc_oc_dump)
        {
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
        }
      return;
    }

  if (!dsmcc_secs_encode_pes (secs, pes))
    {
      DLMSG ("\n ----> error while encoding pes!\n\n");
      if (dsmcc_oc_dump)
        {
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
        }
      return;
    }

  DSMCC_MSG ("\n");

  DLMSG (" --> successfully encoded <%s> into <%s>.\n\n",
         dsmcc_conf_get_in(), pes->pes);

  dsmcc_secs_free (secs);
  dsmcc_mods_free (mods);
  dsmcc_pes_free (pes);

  if (dsmcc_oc_dump)
    {
      if (!dsmcc_conf_dump_write (dump))
        {
          DLMSG ("\n ----> error while dumping conf!\n\n");
          close (dump);
          unlink (dsmcc_conf_get_dump_out());
          return;
        }

      close (dump);

      DLMSG (" --> dumped carousel structure to <%s> for further updates.\n\n",
             dsmcc_conf_get_dump_out());
    }

  return;
}
