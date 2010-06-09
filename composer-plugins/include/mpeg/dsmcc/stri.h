// dsm stream info

#ifndef _header_dsmcc_stri_
#define _header_dsmcc_stri_


#define DSMCC_STRI_SIZE  0x0C
#define DSMCC_STRI_AUDIO 0x01
#define DSMCC_STRI_VIDEO 0x02
#define DSMCC_STRI_DATA  0x04


unsigned char *
dsmcc_stri_new (unsigned char *data,
                unsigned int   dlen,
                unsigned int   dsec,
                unsigned int   dmsec,
                unsigned char  dmask,
                unsigned int  *len);


#endif
