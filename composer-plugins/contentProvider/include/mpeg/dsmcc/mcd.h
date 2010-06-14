//compressed module descriptor

#ifndef _header_dsmcc_mcd_
#define _header_dsmcc_mcd_


#define DSMCC_MCD_SIZE   0x07
#define DSMCC_MCD_TAG    0x09
#define DSMCC_MCD_METHOD 0x08


unsigned char *
dsmcc_mcd_new (unsigned int  orig,
               unsigned int *len);


#endif
