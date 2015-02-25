// utility functions

#include "../../../include/mpeg/dsmcc/util.h"

#include <stdlib.h>
#include <string.h>


#define EMPTY_TOKEN "NULL"


char *
dsmcc_type_string (ObjectType type)
{
  switch (type)
    {
      case ot_dir:
        return "dir";
      case ot_fil:
        return "fil";
      case ot_str:
        return "str";
      case ot_ste:
        return "ste";
      default:
        return "unknown type";
    }
}


void
dsmcc_write_1 (unsigned char *data,
               unsigned int  *pos,
               unsigned char  value)
{
  data[(*pos)] = value;
  (*pos) += 1;
}


void
dsmcc_write_3 (unsigned char *data,
               unsigned int  *pos,
               unsigned int   value)
{
  data[(*pos)]   = (unsigned char)((value & 0x00FF0000) >> 16);
  data[(*pos)+1] = (unsigned char)((value & 0x0000FF00) >> 8);
  data[(*pos)+2] = (unsigned char)(value & 0x000000FF);
  (*pos) += 3;
}


void
dsmcc_write_2 (unsigned char  *data,
               unsigned int   *pos,
               unsigned short  value)
{
  data[(*pos)]   = (unsigned char)((value & 0xFF00) >> 8);
  data[(*pos)+1] = (unsigned char)(value & 0x00FF);
  (*pos) += 2;
}


void
dsmcc_write_4 (unsigned char *data,
               unsigned int  *pos,
               unsigned int   value)
{
  data[(*pos)]   = (unsigned char)((value & 0xFF000000) >> 24);
  data[(*pos)+1] = (unsigned char)((value & 0x00FF0000) >> 16);
  data[(*pos)+2] = (unsigned char)((value & 0x0000FF00) >> 8);
  data[(*pos)+3] = (unsigned char)(value & 0x000000FF);
  (*pos) += 4;
}


unsigned int
dsmcc_tid (unsigned int ver,
           unsigned int id,
           unsigned int up)
{
  unsigned int tid  = 0;
  unsigned int morg = 0x80000000;
  unsigned int mver = ((ver % 0x00004000) << 16);
  unsigned int mid  = ((id % 0x00008000) << 1);
  unsigned int mup  = ((up % 0x00000002));

  tid = (morg | mver | mid | mup);

  return tid;
}


unsigned int
dsmcc_get_comp (char  *path,
                char **comp,
                char  *sep)
{
  char *token     = 0;
  char *tok_str   = 0;
  int   nr_comp   = 0;

  if (!path)
    {
      return 0;
    }

  tok_str = strdup (path);
  token   = strtok (tok_str, sep);

  while (token)
    {
      if (!strcmp (token, EMPTY_TOKEN))
        {
          comp[nr_comp++] = strdup ("");
        }
      else
        {
          comp[nr_comp++] = strdup (token);
        }

      token = strtok (0, sep);
    }

  free (tok_str);

  return nr_comp;
}
