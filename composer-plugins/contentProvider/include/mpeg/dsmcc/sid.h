// stream identifier descriptor

#ifndef _header_dsmcc_sid_
#define _header_dsmcc_sid_


#define DSMCC_SID_SIZE 0x03
#define DSMCC_SID_TAG  0x52


typedef struct _DsmccSid
{
  unsigned int   size;
  unsigned char *data;
} DsmccSid;


DsmccSid *
dsmcc_sid_new (unsigned char ctag);


void
dsmcc_sid_free (DsmccSid *sid);


#endif
