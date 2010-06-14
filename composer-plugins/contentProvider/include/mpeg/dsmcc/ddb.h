//download data block

#ifndef _header_dsmcc_ddb_
#define _header_dsmcc_ddb_


#define DSMCC_DDB_SIZE       0x06
#define DSMCC_DDB_MSID     0x1003
#define DSMCC_DDB_RESERVED   0xFF
#define DSMCC_DDB_MAX_SIZE 0x0FE2


unsigned char *
dsmcc_ddb_new (unsigned int    did,
               unsigned short  mid,
               unsigned char   mver,
               unsigned short  bnr,
               unsigned char  *data,
               unsigned int    dlen,
               unsigned int   *len);


#endif
