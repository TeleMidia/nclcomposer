// network service access point

#ifndef _header_dsmcc_nsap_
#define _header_dsmcc_nsap_


#define DSMCC_NSAP_SIZE           0x14
#define DSMCC_NSAP_AFI            0x00
#define DSMCC_NSAP_TYPE           0x00
#define DSMCC_NSAP_SPEC     0x0100015A
#define DSMCC_NSAP_RESERVED 0xFFFFFFFF


unsigned char *
dsmcc_nsap_new (unsigned int    cid,
                unsigned short  onid,
                unsigned short  tsid,
                unsigned short  sid,
                unsigned int   *len);


#endif
