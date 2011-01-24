//message header

#ifndef _header_dsmcc_mhdr_
#define _header_dsmcc_mhdr_


#define DSMCC_MHDR_SIZE             0x0C
#define DSMCC_MHDR_PROTOCOL         0x11
#define DSMCC_MHDR_DSMCC_TYPE       0x03
#define DSMCC_MHDR_RESERVED         0xFF
#define DSMCC_MHDR_ADAP_LEN   0x00000000


unsigned char *
dsmcc_mhdr_new (unsigned short  msid,
                unsigned int    tid,
                unsigned short  mlen,
                unsigned int   *len);


#endif
