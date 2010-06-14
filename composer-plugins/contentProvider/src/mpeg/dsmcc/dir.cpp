// biop directory message

#include "../../../include/mpeg/dsmcc/dir.h"

#include <stdlib.h>
#include <string.h>

#include "../../../include/mpeg/dsmcc/bhdr.h"
#include "../../../include/mpeg/dsmcc/bind.h"


DsmccDir *
dsmcc_dir_new (char          *path,
               unsigned int   key,
               DsmccList     *binds,
               unsigned char  srg)
{
  DsmccDir       *ret     = 0;
  unsigned char  *dir     = 0;
  unsigned int    pos     = 0;
  unsigned char   uc_tmp  = 0;
  unsigned short  us_tmp  = 0;
  unsigned int    ui_tmp  = 0;
  unsigned int    bhdrlen = 0;
  unsigned char  *bhdr    = 0;
  unsigned int    bindlen = 0;
  DsmccLink      *link    = 0;
  DsmccBind      *bind    = 0;

  if (dsmcc_list_size (binds) > 0)
    {
      if (dsmcc_list_size (binds) > DSMCC_DIR_MAX_BINDS)
        {
          DSMCC_MERR ("(0x%08x) (%s) (%d)",
                      key, path, dsmcc_list_size (binds));
          return 0;
        }

      dsmcc_list_foreach (binds, link)
        {
          bind = (DsmccBind*)link->data;
          bindlen += bind->size;
        }
    }

  bhdr = dsmcc_bhdr_new (key, srg ? ot_srg : ot_dir, DSMCC_DIR_SIZE + bindlen, &bhdrlen);
  if (!bhdr)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      return 0;
    }

  dir = (unsigned char*)malloc (DSMCC_DIR_SIZE + bhdrlen + bindlen);
  if (!dir)
    {
      DSMCC_MERR ("(0x%08x) (%s) (%d) (%d)", key, path, bhdrlen, bindlen);
      free (bhdr);

      return 0;
    }

  memcpy (&dir[pos], bhdr, bhdrlen);
  pos += bhdrlen;

  us_tmp = DSMCC_DIR_OBJ_INFO_LEN;
  dsmcc_write_2 (dir, &pos, us_tmp);

  uc_tmp = DSMCC_DIR_SCL_COUNT;
  dsmcc_write_1 (dir, &pos, uc_tmp);

  ui_tmp = bindlen + 2;
  dsmcc_write_4 (dir, &pos, ui_tmp);

  us_tmp = dsmcc_list_size (binds);
  dsmcc_write_2 (dir, &pos, us_tmp);

  if (dsmcc_list_size (binds) > 0)
    {
      dsmcc_list_foreach (binds, link)
        {
          bind = (DsmccBind*)link->data;
          memcpy (&dir[pos], bind->bind, bind->size);
          pos += bind->size;
        }
    }

  free (bhdr);

  ret = (DsmccDir*)malloc (sizeof (DsmccDir));
  if (!ret)
    {
      DSMCC_MERR ("(0x%08x) (%s)", key, path);
      free (dir);

      return 0;
    }

  ret->key  = key;
  ret->dir  = dir;
  ret->size = pos; 
  ret->path = path;
  ret->srg  = srg;

  return ret;
}


void
dsmcc_dir_free (DsmccDir *dir)
{
  if (dir->dir)
    {
      free (dir->dir);
    }

  free (dir);
}
