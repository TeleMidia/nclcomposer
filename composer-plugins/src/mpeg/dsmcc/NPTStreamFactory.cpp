//dsmcc-npt.c - normal play time descriptor encoding

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/conf.h"
#include "../../../include/mpeg/dsmcc/list.h"
#include "../../../include/mpeg/dsmcc/nptd.h"
#include "../../../include/mpeg/dsmcc/pes.h"
#include "../../../include/mpeg/dsmcc/sec.h"
#include "../../../include/mpeg/dsmcc/util.h"


#define DMSG(x...) printf("dsmcc-npt: "x); fflush(0)
#define DLMSG(x...) printf(x); fflush(0)

#define DSMCC_NPT_TIDE 0x4000


static unsigned int       npt_npt  = 0;
static unsigned long long npt_stc  = 0;
static unsigned int       npt_len  = 1;
static unsigned short     npt_ver  = 0;
static unsigned short     npt_base = 0;
static unsigned char      npt_dis  = 0;
static unsigned int       npt_num  = 1;
static unsigned int       npt_den  = 1;
static unsigned int       npt_rate = DSMCC_NPT_DEFAULT_BITRATE;

/*
static void
dsmcc_npt_usage (char* prog)
{
  printf ("\n");
  printf ("usage: %s [options]\n", prog);
  printf ("options:\n");
  printf ("  -out File, --out=File                   save PES to File (def. <%s>)\n", dsmcc_conf_get_npt_out());
  printf ("  -pid PID, --pid=PID                     use PID (hex) (def. <0x%04x>)\n", dsmcc_conf_get_pid());
  printf ("  -npt NPT, --npt=NPT                     use NPT (hex) (def. <0x%08x>)\n", npt_npt);
  printf ("  -stc STC, --stc=STC                     use STC (hex) (def. <%lld>)\n", npt_stc);
  printf ("  -len Seconds, --len=Seconds             generate stream of Seconds length (def. <%d>)\n", npt_len);
  printf ("  -ver NPTVer, --ver=NPTVer               use NPT-Version (hex) (def. <0x%02x>)\n", npt_ver);
  printf ("  -base TimeBase, --base=TimeBase         use NPT-TimeBase (hex) (def. <0x%02x>)\n", npt_base);
  printf ("  -dis DisInd, --dis=DisInd               use Discontinuity-Indicator [0|1] (def. <%d>)\n", npt_dis);
  printf ("  -num Numerator, --num=Numerator         use NPT-Numerator (hex) (def. <0x%04x>)\n", npt_num);
  printf ("  -den Denominator, --den=Denominator     use NPT-Denominator (hex) (def. <0x%04x>)\n", npt_num);
  printf ("  -rate BitRate, --rate=BitRate           use NPT-BitRate (dec) (def. <%d>)\n", npt_rate);
  printf ("  -ns, --nostuffing                       no stuffing at end of stream (def. <no>)\n");
  printf ("  -v, -verbose, --verbose                 increase verbosity (def. <no>)\n");
  printf ("  -h, -help, --help                       print this text\n");
  printf ("\n");
}


static unsigned int
dsmcc_npt_parse_args (int    argc,
                      char **argv)
{
  int  i = 0;
  char buf[256];

  for (i = 1; i < argc; i++)
    {
      if (!strcmp (argv[i], "-h") || !strcmp (argv[i], "-help") || !strcmp (argv[i], "--help"))
        {
          dsmcc_npt_usage (argv[0]);
          return 0;
        }
      else if (!strcmp (argv[i], "-v") || !strcmp (argv[i], "-verbose") || !strcmp (argv[i], "--verbose"))
        {
          dsmcc_conf_set_verb (1);
        }
      else if (!strcmp (argv[i], "-ns") || !strcmp (argv[i], "--nostuffing"))
        {
          dsmcc_conf_set_stuff (0);
        }
      else if (strstr (argv[i], "--pid=") == argv[i])
        {
          dsmcc_conf_set_pid (strtol (1 + index (argv[i], '='), 0, 16));
        }
      else if (!strcmp (argv[i], "-pid"))
        {
          if (i + 1 < argc)
            {
              dsmcc_conf_set_pid (strtol (argv[++i], 0, 16));
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--out=") == argv[i])
        {
          strcpy (buf, 1 + index (argv[i], '='));
          dsmcc_conf_set_npt_out (strdup (buf));
        }
      else if (!strcmp (argv[i], "-out"))
        {
          if (i + 1 < argc)
            {
              dsmcc_conf_set_npt_out (strdup (argv[++i]));
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--npt=") == argv[i])
        {
          npt_npt = strtol (1 + index (argv[i], '='), 0, 16);
        }
      else if (!strcmp (argv[i], "-npt"))
        {
          if (i + 1 < argc)
            {
              npt_npt = strtol (argv[++i], 0, 16);
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--stc=") == argv[i])
        {
          npt_stc = strtoll (1 + index (argv[i], '='), 0, 16);
        }
      else if (!strcmp (argv[i], "-stc"))
        {
          if (i + 1 < argc)
            {
              npt_stc = strtoll (argv[++i], 0, 16);
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--len=") == argv[i])
        {
          npt_len = strtol (1 + index (argv[i], '='), 0, 10);
        }
      else if (!strcmp (argv[i], "-len"))
        {
          if (i + 1 < argc)
            {
              npt_len = strtol (argv[++i], 0, 10);
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--ver=") == argv[i])
        {
          npt_ver = strtol (1 + index (argv[i], '='), 0, 16);
        }
      else if (!strcmp (argv[i], "-ver"))
        {
          if (i + 1 < argc)
            {
              npt_ver = strtol (argv[++i], 0, 16);
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--base=") == argv[i])
        {
          npt_base = strtol (1 + index (argv[i], '='), 0, 16);
        }
      else if (!strcmp (argv[i], "-base"))
        {
          if (i + 1 < argc)
            {
              npt_base = strtol (argv[++i], 0, 16);
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--dis=") == argv[i])
        {
          npt_dis = strtol (1 + index (argv[i], '='), 0, 16);
        }
      else if (!strcmp (argv[i], "-dis"))
        {
          if (i + 1 < argc)
            {
              npt_dis = strtol (argv[++i], 0, 16);
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--num=") == argv[i])
        {
          npt_num = strtol (1 + index (argv[i], '='), 0, 16);
        }
      else if (!strcmp (argv[i], "-num"))
        {
          if (i + 1 < argc)
            {
              npt_num = strtol (argv[++i], 0, 16);
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--den=") == argv[i])
        {
          npt_den = strtol (1 + index (argv[i], '='), 0, 16);
        }
      else if (!strcmp (argv[i], "-den"))
        {
          if (i + 1 < argc)
            {
              npt_den = strtol (argv[++i], 0, 16);
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else if (strstr (argv[i], "--rate=") == argv[i])
        {
          npt_rate = strtol (1 + index (argv[i], '='), 0, 10);
        }
      else if (!strcmp (argv[i], "-rate"))
        {
          if (i + 1 < argc)
            {
              npt_rate = strtol (argv[++i], 0, 10);
            }
          else
            {
              dsmcc_npt_usage (argv[0]);
              return 0;
            }
        }
      else
        {
          dsmcc_npt_usage (argv[0]);
          return 0;
        }
    }

  return 1;
}

*/

static void
dsmcc_npt_conf (void)
{
  DSMCC_MSG (" conf: out   = <%s>\n", dsmcc_conf_get_npt_out());
  DSMCC_MSG (" conf: pid   = <0x%04x>\n", dsmcc_conf_get_pid());
  DSMCC_MSG (" conf: npt   = <0x%08x>\n", npt_npt);
  DSMCC_MSG (" conf: stc   = <%lld>\n", npt_stc);
  DSMCC_MSG (" conf: len   = <%d sec.>\n", npt_len);
  DSMCC_MSG (" conf: ver   = <0x%02x>\n", npt_ver);
  DSMCC_MSG (" conf: base  = <0x%02x>\n", npt_base);
  DSMCC_MSG (" conf: dis   = <%d>\n", npt_dis);
  DSMCC_MSG (" conf: num   = <0x%04x>\n", npt_num);
  DSMCC_MSG (" conf: den   = <0x%04x>\n", npt_den);
  DSMCC_MSG (" conf: rate  = <%d>\n", npt_rate);
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


static unsigned int
dsmcc_npt_check (void)
{
  if (dsmcc_conf_get_pid() < DSMCC_PES_MIN_PID ||
      dsmcc_conf_get_pid() > DSMCC_PES_MAX_PID)
    {
      DLMSG ("\n ----> error: pes pid-range [0x%04x-0x%04x].\n\n",
             DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);
      return 0;
    }

  if (npt_stc > DSMCC_NPTD_MAX_STC)
    {
      DLMSG ("\n ----> error: stc too large (max. %lld).\n\n",
             DSMCC_NPTD_MAX_STC);
      return 0;
    }

  if (npt_ver > DSMCC_SEC_MAX_VER)
    {
      DLMSG ("\n ----> error: npt version too large (max. 0x%02x).\n\n",
             DSMCC_SEC_MAX_VER);
      return 0;
    }

  if (npt_base > DSMCC_NPTD_MAX_BASE)
    {
      DLMSG ("\n ----> error: npt timebase too large (max. 0x%02x).\n\n",
             DSMCC_NPTD_MAX_BASE);
      return 0;
    }

  if (npt_dis != 0 && npt_dis != 1)
    {
      DLMSG ("\n ----> error: bad discontinuity-indicator, must be (0|1).\n\n");
      return 0;
    }

  if (npt_num > DSMCC_NPTD_MAX_NUM)
    {
      DLMSG ("\n ----> error: npt numerator too large (max. 0x%02x).\n\n",
             DSMCC_NPTD_MAX_NUM);
      return 0;
    }

  if (npt_den > DSMCC_NPTD_MAX_DEN)
    {
      DLMSG ("\n ----> error: npt denominator too large (max. 0x%02x).\n\n",
             DSMCC_NPTD_MAX_DEN);
      return 0;
    }

  return 1;
}


int createNPTStream (int argc, char **argv)
{
  DsmccNptd *nptd = 0;
  DsmccSec  *sec  = 0;
  DsmccPes  *pes  = 0;
  int        fd   = 0;
  int        i    = 0;
  DsmccList *npts = dsmcc_list_new();
  DsmccLink *link = 0;

  dsmcc_conf_init();

  /*if (!dsmcc_npt_parse_args (argc, argv))
    {
      return -666;
    }
*/
  if (!dsmcc_npt_check ())
    {
      return -666;
    }

  DLMSG ("\n");

  dsmcc_npt_conf ();

  DSMCC_MSG (" --> creating npt-sections...\n");

  for (i = 0; i < npt_len; i++)
    {
      nptd = dsmcc_nptd_new (npt_npt,
                             npt_stc,
                             npt_base,
                             npt_num,
                             npt_den,
                             npt_dis);
      if (!nptd)
        {
          DLMSG ("\n ----> error while creating npt!\n\n");
          return -666;
        }

      sec = dsmcc_sec_new (ot_ste,
                           DSMCC_NPT_TIDE,
                           npt_ver,
                           0,
                           0,
                           nptd->nptd,
                           nptd->size,
                           0,
                           0,
                           0,
                           0,
                           0);
      dsmcc_nptd_free (nptd);
      if (!sec)
        {
          DLMSG ("\n ----> error while creating sec!\n\n");
          return -666;
        }

      dsmcc_list_push_back (npts, sec);

      npt_npt += npt_rate;
      npt_stc += npt_rate;
    }

  DSMCC_MSG ("\n --> creating pes...\n");

  pes = dsmcc_pes_new (dsmcc_conf_get_npt_out(), dsmcc_conf_get_pid());
  if (!pes)
    {
      DLMSG ("\n ----> error while creating pes!\n\n");
      return -666;
    }

#ifndef WIN32
  fd = open (pes->pes, O_WRONLY | O_CREAT | O_TRUNC, 0644);
#else
  fd = open (pes->pes, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
#endif
  if (fd < 0)
    {
      DLMSG ("\n ----> error while opening pes!\n\n");
      return -666;
    }

  dsmcc_list_foreach (npts, link)
    {
      sec = (DsmccSec*)(link->data);

      if (!dsmcc_pes_encode (pes, sec->data, sec->len, fd))
        {
          DLMSG ("\n ----> error while encoding pes!\n\n");
          close (fd);

          return -666;
        }

      dsmcc_sec_free (sec);
    }

  if (dsmcc_conf_get_stuff ())
    {
      dsmcc_pes_stuff (pes, fd);
    }

  close (fd);

  DSMCC_MSG ("\n");
  DLMSG (" --> successfully encoded npt into <%s>.\n\n", pes->pes);

  dsmcc_pes_free (pes);

  return 0;
}
