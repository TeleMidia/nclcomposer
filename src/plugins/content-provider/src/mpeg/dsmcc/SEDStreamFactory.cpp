// stream event descriptor encoding

#include "../../../include/mpeg/dsmcc/SEDStreamFactory.h"

unsigned short SEDStreamFactory::event_id = 1;
unsigned int SEDStreamFactory::event_npt = 0;
char* SEDStreamFactory::event_desc = NULL;
unsigned int SEDStreamFactory::event_dlen = 12;
unsigned short SEDStreamFactory::event_ver = 0;

void SEDStreamFactory::dsmcc_ste_conf(void) {
  DSMCC_MSG (" conf: out   = <%s>\n", dsmcc_conf_get_ste_out());
  DSMCC_MSG (" conf: pid   = <0x%04x>\n", dsmcc_conf_get_pid());
  DSMCC_MSG (" conf: id    = <0x%04x>\n", event_id);
  DSMCC_MSG (" conf: npt   = <0x%08x>\n", event_npt);
  DSMCC_MSG (" conf: desc  = <%s>\n", event_desc);
  DSMCC_MSG (" conf: ver   = <0x%02x>\n", event_ver);
  if (dsmcc_conf_get_stuff ())
    {
      DSMCC_MSG (" conf: stuff = <yes>\n");
    }
  else
    {
      DSMCC_MSG (" conf: stuff = <no>\n");
    }
  DSMCC_MSG ("\n");
}

unsigned int SEDStreamFactory::dsmcc_ste_check(void) {
  unsigned int din = 0;

  if (dsmcc_conf_get_pid() < DSMCC_PES_MIN_PID ||
      dsmcc_conf_get_pid() > DSMCC_PES_MAX_PID)
    {
      DLMSG ("\n ----> error: pes pid-range [0x%04x-0x%04x].\n\n",
             DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);
      return 0;
    }

  if (event_id >= DSMCC_STREAM_MIN_ID &&
      event_id <= DSMCC_STREAM_MAX_DIN_ID)
    {
      din = 1;
    }
  else if (event_id >= DSMCC_STREAM_MIN_SCH_ID &&
           event_id <= DSMCC_STREAM_MAX_ID)
    {
      din = 0;
    }
  else
    {
      DLMSG ("\n ----> error: event-id range [0x%04x-0x%04x,0x%04x-0x%04x].\n\n",
             DSMCC_STREAM_MIN_ID, DSMCC_STREAM_MAX_DIN_ID,
             DSMCC_STREAM_MIN_SCH_ID, DSMCC_STREAM_MAX_ID);
      return 0;
    }

  if (din && event_npt)
    {
      DLMSG ("\n ----> error: do-it-now event should have npt=0.\n\n");
      return 0;
    }

  if (!din && !event_npt)
    {
      DLMSG ("\n ----> error: scheduled event should have npt>0.\n\n");
      return 0;
    }

  if (strlen (event_desc) > DSMCC_STED_MAX_DESC)
    {
      DLMSG ("\n ----> error: event description too long (max. 0x%02x).\n\n",
             DSMCC_STED_MAX_DESC);
      return 0;
    }

  if (event_ver > DSMCC_SEC_MAX_VER)
    {
      DLMSG ("\n ----> error: event version too large (max. 0x%02x).\n\n",
             DSMCC_SEC_MAX_VER);
      return 0;
    }

  return 1;
}

void SEDStreamFactory::createSEDStream(
		std::string streamUri,
		unsigned int pid,
		unsigned int cTag,
		unsigned int eventId,
		unsigned int npt,
		std::string privateData) {

  DsmccSted *sted = 0;
  DsmccSec  *sec  = 0;
  DsmccPes  *pes  = 0;
  int        fd   = 0;
  int        len  = 0;

  if (event_desc != NULL) {
	  free(event_desc);
  }

  dsmcc_conf_init();

  dsmcc_conf_set_ste_out((char*)streamUri.c_str());
  dsmcc_conf_set_pid(pid);

  len = privateData.length();
  event_id = eventId;
  event_npt = npt;
  event_desc = (char*)malloc(len);
  memcpy((void*)event_desc, (void*)(privateData.c_str()), len);

  event_dlen = len;

  cout << "SEDStreamFactory::createSEDStream id " << endl;
  if (!dsmcc_ste_check ())
    {
      return;
    }

  DLMSG ("\n");

  dsmcc_ste_conf ();

  DSMCC_MSG (" --> creating stream-event...\n");

  sted = dsmcc_sted_new (event_id,
                         event_npt,
                         (unsigned char *) event_desc,
                         event_dlen);
  if (!sted)
    {
      DLMSG ("\n ----> error while creating stream-event!\n\n");
      return;
    }

  DSMCC_MSG ("\n --> creating section...\n");

  sec = dsmcc_sec_new (ot_ste,
                       event_id,
                       event_ver,
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
      DLMSG ("\n ----> error while creating sec!\n\n");
      return;
    }

  DSMCC_MSG ("\n --> creating pes...\n");

  pes = dsmcc_pes_new (dsmcc_conf_get_ste_out(), dsmcc_conf_get_pid());
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

  if (!dsmcc_pes_encode (pes, sec->data, sec->len, fd))
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

  dsmcc_sec_free (sec);

  DSMCC_MSG ("\n");
  DLMSG (" --> successfully encoded stream-event into <%s>.\n\n", pes->pes);

  dsmcc_pes_free (pes);
}
