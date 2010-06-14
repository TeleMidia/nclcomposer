// network service access point

#include "../../../include/mpeg/dsmcc/nsap.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_nsap_new (unsigned int    cid,
                unsigned short  onid,
                unsigned short  tsid,
                unsigned short  sid,
                unsigned int   *len)
{
  unsigned char *ret     = 0;
  unsigned int   pos     = 0;
  unsigned char  uc_tmp  = 0;
  unsigned short us_tmp  = 0;
  unsigned int   ui_tmp  = 0;

  ret = (unsigned char*)malloc (DSMCC_NSAP_SIZE);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x) (0x%08x) (0x%04x)",
                  cid, onid, tsid, sid);
      return 0;
    }

  uc_tmp = DSMCC_NSAP_AFI;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  uc_tmp = DSMCC_NSAP_TYPE;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  ui_tmp = cid;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  ui_tmp = DSMCC_NSAP_SPEC;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  us_tmp = onid;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = tsid;
  dsmcc_write_2 (ret, &pos, us_tmp);

  us_tmp = sid;
  dsmcc_write_2 (ret, &pos, us_tmp);

  ui_tmp = DSMCC_NSAP_RESERVED;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  (*len) = pos;

  return ret;
}
