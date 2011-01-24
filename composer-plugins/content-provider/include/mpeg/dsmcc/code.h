//encoding of biop objects

#ifndef _header_dsmcc_code_
#define _header_dsmcc_code_

#include "dir.h"
#include "ent.h"
#include "fil.h"
#include "stdb.h"
#include "ste.h"
#include "str.h"


unsigned int
dsmcc_code_dir (DsmccEnt  *ent,
                DsmccDir **dir);


unsigned int
dsmcc_code_fil (DsmccEnt      *ent,
                DsmccFil     **fil,
                unsigned int   sin);


unsigned int
dsmcc_code_str (DsmccEnt  *ent,
                DsmccStr **str);


unsigned int
dsmcc_code_ste (DsmccEnt   *ent,
                DsmccStdb  *stdb,
                DsmccSte  **ste);


#endif
