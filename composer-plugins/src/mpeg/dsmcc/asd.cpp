// application signalling descriptor

#include "../../../include/mpeg/dsmcc/asd.h"

#include <malloc.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccAsd *
dsmcc_asd_new (void)
{
  DsmccAsd      *ret    = 0;
  unsigned int   pos    = 0;
  unsigned char  uc_tmp = 0;

  ret = new DsmccAsd;
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->size = DSMCC_ASD_SIZE;

  ret->data = (unsigned char*)malloc (ret->size);
  if (!ret->data)
    {
      DSMCC_ERR ();

      free (ret);
      return 0;
    }

  uc_tmp = DSMCC_ASD_TAG;
  dsmcc_write_1 (ret->data, &pos, uc_tmp);

  uc_tmp = (ret->size - 2);
  dsmcc_write_1 (ret->data, &pos, uc_tmp);

  return ret;
}


void
dsmcc_asd_free (DsmccAsd *asd)
{
  if (asd->data)
    {
      free (asd->data);
    }

  free (asd);
}
