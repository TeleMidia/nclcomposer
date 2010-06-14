//module label descriptor

#ifndef _header_dsmcc_mld_
#define _header_dsmcc_mld_


#define DSMCC_MLD_SIZE    0x02
#define DSMCC_MLD_TAG     0x70
#define DSMCC_MLD_MAX_LEN 0xFF


unsigned char *
dsmcc_mld_new (char         *label,
               unsigned int *len);


#endif
