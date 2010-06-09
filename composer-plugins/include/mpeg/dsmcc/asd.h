//Application Signalling Descriptor
#ifndef _header_dsmcc_asd_
#define _header_dsmcc_asd_


#define DSMCC_ASD_SIZE 0x02
#define DSMCC_ASD_TAG  0x6F


typedef struct _DsmccAsd
{
  unsigned int   size;
  unsigned char *data;
} DsmccAsd;


DsmccAsd *
dsmcc_asd_new (void);


void
dsmcc_asd_free (DsmccAsd *asd);


#endif
