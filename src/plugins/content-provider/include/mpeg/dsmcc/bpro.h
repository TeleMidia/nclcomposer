//biop profile body

#ifndef _header_dsmcc_bpro_
#define _header_dsmcc_bpro_


#define DSMCC_BPRO_SIZE             0x0A
#define DSMCC_BPRO_LITE_OFF         0x02
#define DSMCC_BPRO_ID_TAG     0x49534F06
#define DSMCC_BPRO_BYTE_ORDER       0x00
#define DSMCC_BPRO_LITE_COUNT       0x02


unsigned char *
dsmcc_bpro_new (unsigned int    tid,
                unsigned short  atag,
                unsigned int    key,
                unsigned short  mid,
                unsigned int    cid,
                unsigned int   *len);


#endif
