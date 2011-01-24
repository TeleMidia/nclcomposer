// biop module

#ifndef _header_dsmcc_bmod_
#define _header_dsmcc_bmod_


#define DSMCC_BMOD_SIZE                      0x1D
#define DSMCC_BMOD_INFO_LEN                  0x15
#define DSMCC_BMOD_INFO_MOD_TIMEOUT    0xCAFEDEAD
#define DSMCC_BMOD_INFO_BLOCK_TIMEOUT  0xCAFEDEAD
#define DSMCC_BMOD_INFO_MIN_BLOCK_TIME 0xCAFEDEAD
#define DSMCC_BMOD_INFO_TAPS_COUNT           0x01
#define DSMCC_BMOD_INFO_TAP_ID             0x0000
#define DSMCC_BMOD_INFO_TAP_USE            0x0017
#define DSMCC_BMOD_INFO_TAP_SEL_LEN          0x00


typedef struct _DsmccBmod
{
  unsigned short  mid;
  unsigned int    len;
  unsigned char   ver;
  unsigned char   ctag;
  unsigned char  *bmod;
  unsigned int    size;
} DsmccBmod;


DsmccBmod *
dsmcc_bmod_new (unsigned short  mid,
                unsigned int    msize,
                unsigned char   mver,
                unsigned char   ctag,
                unsigned char  *mld,
                unsigned int    mldlen,
                unsigned char  *mcpd,
                unsigned char   mcpdlen);


void
dsmcc_bmod_free (DsmccBmod *bmod);


unsigned int
dsmcc_bmod_dump_write (DsmccBmod *bmod,
                       int        fd);


DsmccBmod *
dsmcc_bmod_dump_read (int fd);


#endif
