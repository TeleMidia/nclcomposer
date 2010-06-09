// stream event descriptor

#ifndef _header_dsmcc_sted_
#define _header_dsmcc_sted_

#define DSMCC_STED_SIZE           0x0C
#define DSMCC_STED_TAG            0x1A
#define DSMCC_STED_RESERVED 0x00000000
#define DSMCC_STED_MAX_DESC       0xFF


typedef struct _DsmccSted
{
  unsigned short  id;
  unsigned int    npt;
  unsigned char  *sted;
  unsigned int    size;
} DsmccSted;


DsmccSted *
dsmcc_sted_new (unsigned short  id,
                unsigned int    npt,
                unsigned char  *data,
                unsigned int    dlen);


void
dsmcc_sted_free (DsmccSted *sted);


#endif
