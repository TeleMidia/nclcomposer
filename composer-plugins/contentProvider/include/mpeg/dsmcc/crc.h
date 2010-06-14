//crc32 calculation

#ifndef _header_dsmcc_crc_
#define _header_dsmcc_crc_

#include "list.h"


void
dsmcc_crc_init ();


unsigned int
dsmcc_crc (char         *data,
           unsigned int  len);


unsigned int
dsmcc_crc_file (char *file);


unsigned int
dsmcc_crc_dir (DsmccList *ents);


#endif
