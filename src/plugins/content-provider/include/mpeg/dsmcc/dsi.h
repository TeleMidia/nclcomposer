//download server initiate

#ifndef _header_dsmcc_dsi_
#define _header_dsmcc_dsi_


#define DSMCC_DSI_SIZE             0x18
#define DSMCC_DSI_MSID           0x1006
#define DSMCC_DSI_SERVER_ID_BYTE   0xFF
#define DSMCC_DSI_SERVER_ID_LEN    0x14
#define DSMCC_DSI_COMPAT_DESC    0x0000


typedef struct _DsmccDsi
{
  unsigned int    ver;
  unsigned int    id;
  unsigned int    up;
  unsigned short  atag;
  unsigned int    key;
  unsigned short  mid;
  unsigned int    cid;
  unsigned char  *dsi;
  unsigned int    size;
} DsmccDsi;


DsmccDsi *
dsmcc_dsi_new (unsigned int   ver,
               unsigned int   id,
               unsigned int   up,
               unsigned short atag,
               unsigned int   key,
               unsigned short mid,
               unsigned int   cid,
               unsigned int   rtid);


void
dsmcc_dsi_free (DsmccDsi *dsi);


unsigned int
dsmcc_dsi_dump_write (DsmccDsi *dsi,
                      int       fd,
                      int       root);


DsmccDsi *
dsmcc_dsi_dump_read (int fd);


#endif
