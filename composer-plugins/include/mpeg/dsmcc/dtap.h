//dsm tap

#ifndef _header_dsmcc_dtap_
#define _header_dsmcc_dtap_


#define DSMCC_DTAP_SIZE              0x07
#define DSMCC_DTAP_ID              0x0000
#define DSMCC_DTAP_USE_STR_NPT     0x000B
#define DSMCC_DTAP_USE_STR_STAT_EV 0x000C
#define DSMCC_DTAP_USE_STR_EV      0x000D
#define DSMCC_DTAP_USE_STR_STAT    0x000E
#define DSMCC_DTAP_USE_BIOP_ES     0x0018
#define DSMCC_DTAP_USE_BIOP_PROG   0x0019
#define DSMCC_DTAP_SEL_LEN           0x00


unsigned char *
dsmcc_dtap_new (unsigned int    id,
                unsigned short  use,
                unsigned short  atag,
                unsigned int   *len);


#endif
