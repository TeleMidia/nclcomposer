//download info indication

#ifndef _header_dsmcc_dii_
#define _header_dsmcc_dii_

#include "list.h"


#define DSMCC_DII_SIZE                0x16
#define DSMCC_DII_MSID              0x1002
#define DSMCC_DII_WIN_SIZE            0x00
#define DSMCC_DII_ACK_PERIOD          0x00
#define DSMCC_DII_TC_DOWN_WIN   0x00000000
#define DSMCC_DII_TC_DOWN_SCEN  0x00000000
#define DSMCC_DII_COMPAT_DESC       0x0000
#define DSMCC_DII_PRIV_LEN          0x0000
#define DSMCC_DII_MAX_MODS_SIZE     0x0FD2


typedef struct _DsmccDii
{
  unsigned int   ver;
  unsigned int   id;
  unsigned int   up;
  unsigned int   did;
  DsmccList     *mods;
  unsigned int   mlen;
  unsigned char *dii;
  unsigned int   size;
  unsigned char  upd;
} DsmccDii;


DsmccDii *
dsmcc_dii_new (unsigned int  ver,
               unsigned int  id,
               unsigned int  up,
               unsigned int  did,
               DsmccList    *mods);


void
dsmcc_dii_free (DsmccDii *dii);


unsigned int
dsmcc_dii_dump_write (DsmccDii *dii,
                      int       fd);


DsmccDii *
dsmcc_dii_dump_read (int fd);


#endif
