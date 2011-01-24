//biop tap

#ifndef _header_dsmcc_btap_
#define _header_dsmcc_btap_


#define DSMCC_BTAP_SIZE           0x11
#define DSMCC_BTAP_ID           0x0000
#define DSMCC_BTAP_USE          0x0016
#define DSMCC_BTAP_SEL_LEN        0x0A
#define DSMCC_BTAP_SEL_TYPE     0x0001
#define DSMCC_BTAP_TIMEOUT  0x00000000


unsigned char *
dsmcc_btap_new (unsigned int    tid,
                unsigned short  atag,
                unsigned int   *len);


#endif
