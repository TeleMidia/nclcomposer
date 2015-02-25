// dsm conn binder

#ifndef _header_dsmcc_conn_
#define _header_dsmcc_conn_


#define DSMCC_CONN_SIZE         0x06
#define DSMCC_CONN_ID_TAG 0x49534F40


unsigned char *
dsmcc_conn_new (unsigned int    tid,
                unsigned short  atag,
                unsigned int   *len);


#endif
