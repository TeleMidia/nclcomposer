// dsm conn binder

#include "../../../include/mpeg/dsmcc/conn.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/btap.h"
#include "../../../include/mpeg/dsmcc/util.h"


unsigned char *
dsmcc_conn_new (unsigned int    tid,
                unsigned short  atag,
                unsigned int   *len)
{
  unsigned char *ret     = 0;
  unsigned int   pos     = 0;
  unsigned char  uc_tmp  = 0;
  unsigned int   ui_tmp  = 0;
  unsigned int   btaplen = 0;
  unsigned char *btap    = 0;

  btap = dsmcc_btap_new (tid, atag, &btaplen);
  if (!btap)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", tid, atag);
      return 0;
    }

  ret = (unsigned char*)malloc (DSMCC_CONN_SIZE + btaplen);
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (0x%04x)", tid, atag);
      free (btap);

      return 0;
    }

  ui_tmp = DSMCC_CONN_ID_TAG;
  dsmcc_write_4 (ret, &pos, ui_tmp);

  uc_tmp = btaplen + 1;
  dsmcc_write_1 (ret, &pos, uc_tmp);
  
  uc_tmp = 1;
  dsmcc_write_1 (ret, &pos, uc_tmp);

  memcpy (&ret[pos], btap, btaplen);
  pos += btaplen;
  
  (*len) = pos;

  free (btap);

  return ret;
}
