// module content type descriptor

#ifndef _header_dsmcc_mctd_
#define _header_dsmcc_mctd_


#define DSMCC_MCTD_SIZE 0x02
#define DSMCC_MCTD_TAG  0x72


unsigned char *
dsmcc_mctd_new (char         *type,
                unsigned int *len);


#endif
