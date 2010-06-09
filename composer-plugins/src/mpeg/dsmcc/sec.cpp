// dsmcc private section

#include "../../../include/mpeg/dsmcc/sec.h"

#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/crc.h"


static void
dsmcc_sec_header (DsmccSec      *sec,
                  unsigned char *data,
                  unsigned int  *pos)
{
  unsigned char  uc_tmp = 0;
  unsigned short us_tmp = 0;

  uc_tmp = sec->tid;
  dsmcc_write_1 (data, pos, uc_tmp);

  us_tmp = (sec->size | DSMCC_SEC_SIZE_MASK);
  dsmcc_write_2 (data, pos, us_tmp);

  us_tmp = sec->tide;
  dsmcc_write_2 (data, pos, us_tmp);

  uc_tmp = ((sec->ver << 1) | DSMCC_SEC_VER_MASK);
  dsmcc_write_1 (data, pos, uc_tmp);

  uc_tmp = sec->num;
  dsmcc_write_1 (data, pos, uc_tmp);

  uc_tmp = sec->last;
  dsmcc_write_1 (data, pos, uc_tmp);
}


DsmccSec *
dsmcc_sec_new (ObjectType      type,
               unsigned short  tide,
               unsigned char   ver,
               unsigned char   num,
               unsigned char   last,
               unsigned char  *data,
               unsigned int    dlen,
               unsigned char  *fhdr,
               unsigned int    flen,
               char           *path,
               unsigned int    off,
               unsigned int    olen)
{
  DsmccSec     *ret    = 0;
  unsigned int  pos    = 0;
  unsigned int  ui_tmp = 0;

  ret = (DsmccSec*)malloc (sizeof (DsmccSec));
  if (!ret)
    {
      DSMCC_MERR ("(%s) (0x%04x) (0x%02x) (%d) (%d) (%d)",
                  dsmcc_type_string (type), tide, ver, num, last, dlen);
      return 0;
    }

  ret->tide = tide;
  ret->ver  = ver;
  ret->num  = num;
  ret->last = last;
  ret->size = DSMCC_SEC_HEADER_SIZE + dlen + 1;
  ret->path = path;
  ret->off  = off;
  ret->olen = olen;
  ret->data = 0;
  ret->dlen = 0;
  ret->fhdr = 0;
  ret->flen = 0;

  switch (type)
    {
      case ot_dir:
      case ot_fil:
        ret->tid = DSMCC_SEC_TID_DDB;
        break;
      case ot_str:
      case ot_ste:
        ret->tid = DSMCC_SEC_TID_STR;
        break;
      default:
        ret->tid = DSMCC_SEC_TID_MSG;
        break;
    }

  if (path)
    {
      ret->data = data;
      ret->dlen = dlen;
      ret->fhdr = fhdr;
      ret->flen = flen;

      return ret;
    }

  ret->data = (unsigned char*)malloc (DSMCC_SEC_HEADER_SIZE + dlen + 4);
  if (!ret->data)
    {
      DSMCC_MERR ("(%s) (0x%04x) (0x%02x) (%d) (%d) (%d)",
                  dsmcc_type_string (type), tide, ver, num, last, dlen);
      return 0;
    }

  dsmcc_sec_header (ret, ret->data, &pos);

  memcpy (&ret->data[pos], data, dlen);
  pos += dlen;

  ret->crc = dsmcc_crc ((char *) ret->data, pos);

  ui_tmp = ret->crc;
  dsmcc_write_4 (ret->data, &pos, ui_tmp);

  ret->len = pos;

  return ret;
}


DsmccSec *
dsmcc_sec_complete (DsmccSec *sec)
{
  unsigned char *data   = 0;
  unsigned int   pos    = 0;
  unsigned int   ui_tmp = 0;
  int            fd     = 0;

  if (sec->path)
    {
      data = (unsigned char*)malloc (DSMCC_SEC_HEADER_SIZE + sec->dlen + sec->flen + sec->olen + 4);
      if (!data)
        {
          DSMCC_MERR ("(%s) (0x%04x) (0x%02x) (%d) (%d) (%d) (%s) (%d) (%d)",
                      dsmcc_type_string (sec->type), sec->tide,
                      sec->ver, sec->num, sec->last, sec->dlen,
                      sec->path, sec->off, sec->olen);
          return 0;
        }

      sec->size += sec->olen;

      if (sec->fhdr)
        {
          sec->size += sec->flen;
        }

      dsmcc_sec_header (sec, data, &pos);

      memcpy (&data[pos], sec->data, sec->dlen);
      pos += sec->dlen;

      free (sec->data);
      sec->data = data;

      if (sec->fhdr)
        {
          memcpy (&sec->data[pos], sec->fhdr, sec->flen);
          pos += sec->flen;
          free (sec->fhdr);
        }

      fd = open (sec->path, O_RDONLY);
      if (fd < 0)
        {
          DSMCC_MERR ("(%s) (0x%04x) (0x%02x) (%d) (%d) (%d) (%s) (%d) (%d)",
                      dsmcc_type_string (sec->type), sec->tide,
                      sec->ver, sec->num, sec->last, sec->dlen,
                      sec->path, sec->off, sec->olen);
          return 0;
        }

      if (lseek (fd, sec->off, SEEK_SET) < 0)
        {
          DSMCC_MERR ("(%s) (0x%04x) (0x%02x) (%d) (%d) (%d) (%s) (%d) (%d)",
                      dsmcc_type_string (sec->type), sec->tide,
                      sec->ver, sec->num, sec->last, sec->dlen,
                      sec->path, sec->off, sec->olen);
          close (fd);

          return 0;
        }

      if (read (fd, &sec->data[pos], sec->olen) < 0)
        {
          DSMCC_MERR ("(%s) (0x%04x) (0x%02x) (%d) (%d) (%d) (%s) (%d) (%d)",
                      dsmcc_type_string (sec->type), sec->tide,
                      sec->ver, sec->num, sec->last, sec->dlen,
                      sec->path, sec->off, sec->olen);
          close (fd);

          return 0;
        }

      close (fd);

      pos += sec->olen;

      sec->crc = dsmcc_crc ((char *) sec->data, pos);

      ui_tmp = sec->crc;
      dsmcc_write_4 (sec->data, &pos, ui_tmp);

      sec->len = pos;
    }

  return sec;
}


void
dsmcc_sec_free (DsmccSec *sec)
{
  if (sec->data)
    {
      free (sec->data);
    }

  free (sec);
}


void
dsmcc_sec_update (DsmccSec *sec)
{
  unsigned char uc_tmp = 0;
  unsigned int  ui_tmp = 0;
  unsigned int  pos    = 5;

  sec->ver++;

  uc_tmp = ((sec->ver << 1) | DSMCC_SEC_VER_MASK);
  dsmcc_write_1 (sec->data, &pos, uc_tmp);

  pos = sec->len - 4;

  sec->crc = dsmcc_crc ((char *) sec->data, pos);

  ui_tmp = sec->crc;
  dsmcc_write_4 (sec->data, &pos, ui_tmp);
}
