// crc32 calculation

#include "../../../include/mpeg/dsmcc/crc.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/def.h"
#include "../../../include/mpeg/dsmcc/ent.h"


static unsigned int dsmcc_crc_tab[0x100];


void dsmcc_crc_init ()
{
  unsigned int i = 0;
  unsigned int j = 0;
  unsigned int c = 0;

  for (i = 0; i < 0x100; i++)
    {
      c = (i << 24);

      for (j = 0; j < 8; j++)
        {
          if (c & 0x80000000)
            {
              c = (c << 1) ^ 0x04C11DB7;
            }
          else
            {
              c = (c << 1);
            }
        }

      dsmcc_crc_tab[i] = c;
    }
}


unsigned int
dsmcc_crc (char         *data,
           unsigned int  len)
{
  unsigned int i   = 0;
  unsigned int crc = 0xFFFFFFFF;

  for (i = 0; i < len; i++)
    {
      crc = (crc << 8) ^ dsmcc_crc_tab[((crc >> 24) ^ data[i]) & 0xFF];
    }

  return crc;
}


unsigned int
dsmcc_crc_file (char *file)
{
  int           fd  = 0;
  unsigned int  i   = 0;
  unsigned int  crc = 0xFFFFFFFF;
  unsigned int  len = 0;
  unsigned char data[0xFFFF];

  fd = open (file, O_RDONLY, 0644);
  if (fd < 0)
    {
      DSMCC_ERR ();
      return 0;
    }

  while (1)
    {
      len = read (fd, data, 0xFFFF);
      if (len <= 0)
        {
          break;
        }

      for (i = 0; i < len; i++)
        {
          crc = (crc << 8) ^ dsmcc_crc_tab[((crc >> 24) ^ data[i]) & 0xFF];
        }
    }

  close (fd);

  return crc;
}


unsigned int
dsmcc_crc_dir (DsmccList *ents)
{
  DsmccLink    *link = 0;
  DsmccEnt     *ent  = 0;
  unsigned int  crc  = 0xFFFFFFFF;
  unsigned int  len  = 0;
  unsigned int  i    = 0;

  if (!ents)
    {
      return crc;
    }

  dsmcc_list_foreach (ents, link)
    {
      ent = (DsmccEnt*)link->data;

      len = strlen (ent->rpath);
      for (i = 0; i < len; i++)
        {
          crc = (crc << 8) ^ dsmcc_crc_tab[((crc >> 24) ^ ent->rpath[i]) & 0xFF];
        }

      if (ent->ext)
        {
          len = strlen (ent->ext->orig);
          for (i = 0; i < len; i++)
            {
              crc = (crc << 8) ^ dsmcc_crc_tab[((crc >> 24) ^ ent->ext->orig[i]) & 0xFF];
            }
        }
    }

  return crc;
}
