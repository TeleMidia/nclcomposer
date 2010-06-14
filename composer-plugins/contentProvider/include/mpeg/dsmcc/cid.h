//carousel identifier descriptor

#ifndef _header_dsmcc_cid_
#define _header_dsmcc_cid_


#define DSMCC_CID_SIZE    0x1B
#define DSMCC_CID_NSIZE   0x07
#define DSMCC_CID_TAG     0x13
#define DSMCC_CID_NFORMID 0x00
#define DSMCC_CID_FORMID  0x01
#define DSMCC_CID_TIMEOUT 0xFF
#define DSMCC_CID_NOCOMPR 0xFF


typedef struct _DsmccCid
{
  unsigned int   size;
  unsigned char *data;
} DsmccCid;


DsmccCid *
dsmcc_cid_new (unsigned int   cid,
               unsigned char  form,
               unsigned char  mver,
               unsigned short mid,
               unsigned int   msize,
               unsigned char  compr,
               unsigned int   osize,
               unsigned int   okey);


void
dsmcc_cid_free (DsmccCid *cid);


#endif
