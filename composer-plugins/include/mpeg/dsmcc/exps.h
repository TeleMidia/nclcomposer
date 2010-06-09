//extra pids specification

#ifndef _header_dsmcc_exps_
#define _header_dsmcc_exps_


typedef struct _DsmccExps
{
  unsigned short  pid;
  unsigned char   ctag;
  unsigned char   root;
  char           *path;
} DsmccExps;


DsmccExps *
dsmcc_exps_new (char *spec);


DsmccExps *
dsmcc_exps_new_intern (unsigned short  pid,
                       unsigned char   ctag,
                       char           *path);


void
dsmcc_exps_print (DsmccExps *exps);


void
dsmcc_exps_free (DsmccExps *exps);


unsigned int
dsmcc_exps_dump_write (DsmccExps *exps,
                       int        fd);


DsmccExps *
dsmcc_exps_dump_read (int fd);


#endif
