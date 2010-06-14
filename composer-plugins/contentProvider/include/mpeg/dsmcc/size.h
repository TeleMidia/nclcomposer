// initial biop object sizes
                                                                                                                                                                                                                                
#ifndef _header_dsmcc_size_
#define _header_dsmcc_size_

#include "list.h"


unsigned int
dsmcc_size_init (void);


unsigned int
dsmcc_size_dir (DsmccList *ents);


unsigned int
dsmcc_size_fil (unsigned int dlen);


unsigned int
dsmcc_size_str (unsigned int dlen);


unsigned int
dsmcc_size_ste (unsigned int dlen);


#endif
