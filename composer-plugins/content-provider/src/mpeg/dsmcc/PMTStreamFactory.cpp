// program map table encoding

#include "../../../include/mpeg/dsmcc/PMTStreamFactory.h"

#include "../../../include/mpeg/si/Pmt.h"
using namespace org::isdtv::contentProvider::mpeg::si;

unsigned char PMTStreamFactory::pmt_valid = 1;
unsigned int PMTStreamFactory::pmt_pcr = 0x1FFF;
unsigned int PMTStreamFactory::pmt_prog = 0x0001;
unsigned short PMTStreamFactory::pmt_ver = 0;
DsmccList* PMTStreamFactory::pmt_esis = 0;
/*
static void
dsmcc_pmt_usage (char* prog)
{
  printf ("\n");
  printf ("usage: %s [options]\n", prog);
  printf ("options:\n");
  printf ("  -out File, --out=File                   save PES to File (def. <%s>)\n", dsmcc_conf_get_pmt_out());
  printf ("  -pid PID, --pid=PID                     use PID (hex) (def. <0x%04x>)\n", dsmcc_conf_get_pid());
  printf ("  -oc OCSPEC, --oc=OCSPEC                 add an ES including an ObjectCarousel to the PMT\n");
  printf ("    where OCSPEC == 'PID:CTAG:CID'\n");
  printf ("     with PID  : pid of the ES [0x%04x-0x%04x] (hex)\n", DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);
  printf ("     with CTAG : component-tag of the ES [0..0xFE] (hex)\n");
  printf ("     with CID  : carousel-id of the ES [1..0xFFFFFFFF] (hex)\n");
  printf ("  -ste STESPEC, --ste=STESPEC             add an ES including STE- or NPT-descriptors to the PMT\n");
  printf ("    where STRSPEC == 'PID:CTAG'\n");
  printf ("     with PID  : pid of the ES [0x%04x-0x%04x] (hex)\n", DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);
  printf ("     with CTAG : component-tag of the ES [0..0xFE] (hex)\n");
  printf ("  -ait AITSPEC, --ait=AITSPEC             add an ES including an AIT to the PMT\n");
  printf ("    where AITSPEC == 'PID:CTAG:TYPE'\n");
  printf ("     with PID  : pid of the ES [0x%04x-0x%04x] (hex)\n", DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);
  printf ("     with CTAG : component-tag of the ES [0..0xFE] (hex)\n");
  printf ("     with TYPE : application-type [dvbj,html]\n");
  printf ("  -valid Valid, --valid=Valid             use Valid-Flag for the PMT [0|1] (def. <%d>)\n", pmt_valid);
  printf ("  -pcr PCR_PID, --pcr=PCR_PID             use PCR_PID for the PMT (hex) (def. <0x%04x>)\n", pmt_pcr);
  printf ("  -prog PROGRAM_NR, --prog=PROGRAM_NR     use PROGRAM_NUMBER for the PMT (hex) (def. <0x%04x>)\n", pmt_prog);
  printf ("  -ver PMTVer, --ver=PMTVer               use PMT-Version (hex) (def. <0x%02x>)\n", pmt_ver);
  printf ("  -ns, --nostuffing                       no stuffing at end of stream (def. <no>)\n");
  printf ("  -v, -verbose, --verbose                 increase verbosity (def. <no>)\n");
  printf ("  -h, -help, --help                       print this text\n");
  printf ("\n");
}


static unsigned int
dsmcc_pmt_parse_args (int    argc,
                      char **argv)
{
  int        i    = 0;
  DsmccEsis *esis = 0;
  char       buf[256];

  for (i = 1; i < argc; i++)
    {
      if (strstr (argv[i], "--ait=") == argv[i])
        {
          strcpy (buf, 1 + index (argv[i], '='));
          esis = dsmcc_esis_new (st_ait, strdup (buf));
          if (!esis)
            {
              dsmcc_pmt_usage (argv[0]);
              return 0;
            }
          dsmcc_list_push_back (pmt_esis, esis);
        }
      else if (!strcmp (argv[i], "-ait"))
        {
          if (i + 1 < argc)
            {
              esis = dsmcc_esis_new (st_ait, strdup (argv[++i]));
              if (!esis)
                {
                  dsmcc_pmt_usage (argv[0]);
                  return 0;
                }
              dsmcc_list_push_back (pmt_esis, esis);
            }
          else
            {
              dsmcc_pmt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--valid=") == argv[i])
        {
          pmt_valid = strtol (1 + index (argv[i], '='), 0, 10);
        }
      else if (!strcmp (argv[i], "-valid"))
        {
          if (i + 1 < argc)
            {
              pmt_valid = strtol (argv[++i], 0, 10);
            }
          else
            {
              dsmcc_pmt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--ver=") == argv[i])
        {
          pmt_ver = strtol (1 + index (argv[i], '='), 0, 16);
        }
      else if (!strcmp (argv[i], "-ver"))
        {
          if (i + 1 < argc)
            {
              pmt_ver = strtol (argv[++i], 0, 16);
            }
          else
            {
              dsmcc_pmt_usage (argv[0]);
              return 0;
            }
        }
      else
        {
          dsmcc_pmt_usage (argv[0]);
          return 0;
        }
    }

  dsmcc_conf_set_esis (pmt_esis);

  return 1;
}
*/

void PMTStreamFactory::dsmcc_pmt_conf(void) {
  DsmccLink *link = 0;
  DsmccEsis *esis = 0;

  DSMCC_MSG (" conf: out   = <%s>\n", dsmcc_conf_get_pmt_out());
  DSMCC_MSG (" conf: pid   = <0x%04x>\n", dsmcc_conf_get_pid());
  DSMCC_MSG (" conf: valid = <%d>\n", pmt_valid);
  DSMCC_MSG (" conf: ver   = <0x%02x>\n", pmt_ver);
  if (dsmcc_conf_get_stuff ())
    {
      DSMCC_MSG (" conf: stuff = <yes>\n");
    }
  else
    {
      DSMCC_MSG (" conf: stuff = <no>\n");
    }
  DSMCC_MSG ("\n");
  dsmcc_list_foreach (dsmcc_conf_get_esis (), link)
    {
      esis = (DsmccEsis*)link->data;

      dsmcc_esis_print (esis);
    }
  DSMCC_MSG ("\n");
}

unsigned int PMTStreamFactory::dsmcc_pmt_check(void) {
  if (dsmcc_conf_get_pid() < DSMCC_PES_MIN_PID ||
      dsmcc_conf_get_pid() > DSMCC_PES_MAX_PID)
    {
      DLMSG ("\n ----> error: pes pid-range [0x%04x-0x%04x].\n\n",
             DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);
      return 0;
    }

  if (pmt_ver > DSMCC_PMT_MAX_VER)
    {
      DLMSG ("\n ----> error: pmt version too large (max. 0x%02x).\n\n",
             DSMCC_PMT_MAX_VER);
      return 0;
    }

  if (pmt_pcr < DSMCC_PES_MIN_PID || pmt_pcr > DSMCC_PES_MAX_PID)
    {
      DLMSG ("\n ----> error: pcr pid-range [0x%04x-0x%04x].\n\n",
             DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);
      return 0;
    }

  if (pmt_prog > 0xFFFF)
    {
      DLMSG ("\n ----> error: program-number too large (max. 0xFFFF).\n\n");
      return 0;
    }

  if (!dsmcc_list_size (pmt_esis))
    {
      DLMSG ("\n ----> error: no elementary streams defined for pmt.\n\n");
      return 0;
    }

  return 1;
}

void PMTStreamFactory::createPMTStream(
		std::string streamUri,
		unsigned int pid,
		unsigned int programNumber,
		unsigned int pcrPid,
		unsigned int version,
		std::map<unsigned int, short>* streamsInfo,
		std::map<unsigned int, CarouselInfo*>* ocs,
		std::map<unsigned int, SEInfo*>* ses) {

  DsmccPmt  *pmt  = 0;
  DsmccPes  *pes  = 0;
  int        fd   = 0;
  DsmccLink *link = 0;
  DsmccEsis *esis = 0;
  DsmccEsi  *esi  = 0;

  std::map<unsigned int, short>::iterator i;
  std::string param;
  short sType;
  unsigned int currentPid;
  CarouselInfo* ocInfo;
  SEInfo* seInfo;

  pmt_esis = dsmcc_list_new();

  dsmcc_conf_init();
  dsmcc_conf_set_pmt_out((char*)streamUri.c_str());
  dsmcc_conf_set_pid(pid);
  pmt_prog = programNumber;
  pmt_pcr = pcrPid;
  pmt_ver = version;

  cout << "creating pmt pid '" << pid << "' program number '" << pmt_prog;
  cout << "' pcr '" << pmt_pcr << "' ver '" << pmt_ver << "'" << endl;
  i = streamsInfo->begin();
  while (i != streamsInfo->end()) {
	  param = "";
	  currentPid = i->first;
	  sType = i->second;
	  if (sType == Pmt::STREAM_TYPE_PRIVATE_SECTION) {
		  param = "PID:CTAG:TYPE";

	  } else if (sType == Pmt::STREAM_TYPE_DSMCC_TYPE_B) {
		  if (ocs->count(currentPid) != 0) {
			  ocInfo = ocs->find(currentPid)->second;
			  param = intToStrHexa(ocInfo->pid) + ":" + intToStrHexa(
					  ocInfo->componentTag) + ":" + intToStrHexa(ocInfo->myId);
		  }

	  } else if (sType == Pmt::STREAM_TYPE_DSMCC_TYPE_C) {
		  if (ses->count(currentPid) != 0) {
			  seInfo = ses->find(currentPid)->second;
			  param = intToStrHexa(seInfo->pid) + ":" + intToStrHexa(
					  seInfo->componentTag);
		  }

	  } else {
		  param = intToStrHexa(currentPid) + ":0";
	  }

	  if (param != "") {
		  esis = dsmcc_esis_new((DsmccStreamType)sType, (char*)param.c_str());
		  if (esis) {
			  dsmcc_list_push_back (pmt_esis, esis);
		  }
	  }

	  ++i;
  }
  if (!dsmcc_pmt_check ())
    {
      return;
    }

  DLMSG ("\n");

  dsmcc_pmt_conf ();

  DSMCC_MSG (" --> creating pmt...\n");

  pmt = dsmcc_pmt_new (pmt_valid,
                       pmt_prog,
                       pmt_pcr,
                       pmt_ver);
  if (!pmt)
    {
      DLMSG ("\n ----> error while creating pmt!\n\n");
      return;
    }

  DSMCC_MSG ("\n --> creating esi...\n");

  dsmcc_list_foreach (pmt_esis, link)
    {
      esis = (DsmccEsis*)link->data;

      esi = dsmcc_esi_new (esis);
      if (!esi)
        {
          DLMSG ("\n ----> error while creating esi!\n\n");
          return;
        }

      if (!dsmcc_esi_encode (esi))
        {
          DLMSG ("\n ----> error while encoding esi!\n\n");
          return;
        }

      dsmcc_pmt_add_esi (pmt, esi);
    }

  DSMCC_MSG ("\n --> encoding pmt...\n");

  if (!dsmcc_pmt_encode (pmt))
    {
      DLMSG ("\n ----> error while encoding pmt!\n\n");
      return;
    }

  DSMCC_MSG ("\n --> creating pes...\n");
  pes = dsmcc_pes_new (dsmcc_conf_get_pmt_out(), dsmcc_conf_get_pid());
  if (!pes)
    {
      DLMSG ("\n ----> error while creating pes!\n\n");
      return;
    }

#ifndef WIN32
  fd = open (pes->pes, O_WRONLY | O_CREAT | O_TRUNC, 0644);
#else
  fd = open (pes->pes, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
#endif
  if (fd < 0)
    {
      DLMSG ("\n ----> error while opening pes!\n\n");
      return;
    }

  if (!dsmcc_pes_encode (pes, pmt->data, pmt->len, fd))
    {
      DLMSG ("\n ----> error while encoding pes!\n\n");
      close (fd);

      return;
    }

  if (dsmcc_conf_get_stuff ())
    {
      dsmcc_pes_stuff (pes, fd);
    }
  close (fd);

  DSMCC_MSG ("\n");
  DLMSG (" --> successfully encoded pmt into <%s>\n\n", pes->pes);

  dsmcc_pes_free (pes);
}
