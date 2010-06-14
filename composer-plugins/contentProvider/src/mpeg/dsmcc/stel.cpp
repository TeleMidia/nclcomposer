// dsm event list

#include "../../../include/mpeg/dsmcc/stel.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_stel_new (DsmccList    *names,
                unsigned int *len)
{
  unsigned char *ret      = 0;
  unsigned int   pos      = 0;
  unsigned char  uc_tmp   = 0;
  DsmccLink     *link     = 0;
  unsigned char  namelen  = 0;
  char          *name     = 0;
  unsigned int   nameslen = 0;

  if (dsmcc_list_size (names) == 0)
    {
      DSMCC_ERR ();
      return 0;
    }

  dsmcc_list_foreach (names, link)
    {
      name = (char*)link->data;

      nameslen += strlen (name) + 1;
    }

  ret = (unsigned char*)malloc (nameslen + dsmcc_list_size (names));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  dsmcc_list_foreach (names, link)
    {
      name = (char*)link->data;

      namelen = strlen (name) + 1;

      uc_tmp = namelen;
      dsmcc_write_1 (ret, &pos, uc_tmp);

      memcpy (&ret[pos], name, namelen);
      pos += namelen;
    }

  (*len) = pos;

  return ret;
}
