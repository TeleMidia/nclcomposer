// elementary stream info specification

#include "../../../include/mpeg/dsmcc/esis.h"

#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/pes.h"
#include "../../../include/mpeg/dsmcc/util.h"


#define DSMCC_ESIS_COMPS_OC  3
#define DSMCC_ESIS_COMPS_STE 2
#define DSMCC_ESIS_COMPS_AIT 3

DsmccEsis* dsmcc_esis_new(DsmccStreamType  type, char* spec) {
  DsmccEsis    *ret = 0;
  int           num = 0;
  unsigned int  par = 0;
  char         *comps[256];

  if (!spec)
    {
      DSMCC_MERR ("empty esi-spec line.");
      return 0;
    }

  ret = new DsmccEsis;
  if (!ret)
    {
      DSMCC_MERR ("(%s)", spec);
      return 0;
    }

  ret->type = type;

  num = dsmcc_get_comp (spec, comps, ":");

  switch (type)
    {
      case st_oc:
        if (num != DSMCC_ESIS_COMPS_OC)
          {
            DSMCC_MERR ("malformed esi-spec line (expected at least %d components for es-type 'oc').",
                        DSMCC_ESIS_COMPS_OC);
            free (ret);
            return 0;
          }
        break;

      case st_ste:
        if (num != DSMCC_ESIS_COMPS_STE)
          {
            DSMCC_MERR ("malformed esi-spec line (expected at least %d components for es-type 'ste').",
                        DSMCC_ESIS_COMPS_STE);
            free (ret);
            return 0;
          }
        break;

      case st_ait:
        if (num != DSMCC_ESIS_COMPS_AIT)
          {
            DSMCC_MERR ("malformed esi-spec line (expected at least %d components for es-type 'ait').",
                        DSMCC_ESIS_COMPS_AIT);
            free (ret);
            return 0;
          }
        break;

      default:
    	  break;
    }

  par = strtol (comps[0], 0, 16);
  if (par < DSMCC_PES_MIN_PID || par > DSMCC_PES_MAX_PID)
    {
      DSMCC_MERR ("bad es-pid 0x%08x, range [0x%04x-0x%04x]).",
                  par, DSMCC_PES_MIN_PID, DSMCC_PES_MAX_PID);
      free (ret);
      return 0;
    }
  ret->pid = par;

  par = strtol (comps[1], 0, 16);
  if (par > 0xFE)
    {
      DSMCC_MERR ("bad es-component-tag 0x%08x, range [0-0xFE]).", par);
      free (ret);
      return 0;
    }
  ret->ctag = par;

  if (type == st_oc)
    {
      ret->cid = strtol (comps[2], 0, 16);
    }
  else if (type == st_ait)
    {
      if (!strcmp (comps[2], "dvbj"))
        {
          ret->atype = at_dvbj;
        }
      else if (!strcmp (comps[2], "html"))
        {
          ret->atype = at_html;
        }
      else
        {
          DSMCC_MERR ("unknown app-type '%s' for es-type 'ait'.", comps[2]);
          free (ret);
          return 0;
        }
    }

  return ret;
}


void
dsmcc_esis_print (DsmccEsis *esis)
{
  DSMCC_MSG (" + esispec: pid  <0x%04x>\n", esis->pid);
  DSMCC_MSG ("            ctag <0x%02x>\n", esis->ctag);
  DSMCC_MSG ("            type ");
  if (esis->type == st_oc)
    {
      DSMCC_MSG ("<oc> with cid <0x%08x>\n", esis->cid);
    }
  else if (esis->type == st_ste)
    {
      DSMCC_MSG ("<ste>\n");
    }
  else
    {
      DSMCC_MSG ("<ait> with of type ");
      if (esis->atype == at_dvbj)
        {
          DSMCC_MSG ("<dvbj>\n");
        }
      else
        {
          DSMCC_MSG ("<html>\n");
        }
    }
}


void
dsmcc_esis_free (DsmccEsis *esis)
{
  free (esis);
}
