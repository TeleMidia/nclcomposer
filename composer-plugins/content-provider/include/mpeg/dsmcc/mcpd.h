//module caching priority descriptor

#ifndef _header_dsmcc_mcpd_
#define _header_dsmcc_mcpd_


#define DSMCC_MCPD_SIZE         0x04
#define DSMCC_MCPD_TAG          0x71
#define DSMCC_MCPD_PRIO_HIGH    0xFF
#define DSMCC_MCPD_PRIO_LOW     0x00
#define DSMCC_MCPD_LEVEL_TRANS  0x01
#define DSMCC_MCPD_LEVEL_SEMI   0x02
#define DSMCC_MCPD_LEVEL_STATIC 0x03
#define DSMCC_MCPD_LEVEL_NONE   0x00


unsigned char *
dsmcc_mcpd_new (unsigned char  prio,
                unsigned char  level,
                unsigned int  *len);


#endif
