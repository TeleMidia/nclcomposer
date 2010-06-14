// - utility functions

#ifndef _header_dsmcc_util_
#define _header_dsmcc_util_

#include "def.h"


char *
dsmcc_type_string (ObjectType type);


void
dsmcc_write_1 (unsigned char *data,
               unsigned int  *pos,
               unsigned char  value);


void
dsmcc_write_2 (unsigned char  *data,
               unsigned int   *pos,
               unsigned short  value);


void
dsmcc_write_3 (unsigned char *data,
               unsigned int  *pos,
               unsigned int   value);


void
dsmcc_write_4 (unsigned char *data,
               unsigned int  *pos,
               unsigned int   value);


unsigned int
dsmcc_tid (unsigned int ver,
           unsigned int id,
           unsigned int up);


unsigned int
dsmcc_get_comp (char  *path,
                char **comp,
                char  *sep);


#endif
