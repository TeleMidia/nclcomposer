// nptd.h - normal play time reference descriptor

#ifndef _header_dsmcc_nptd_
#define _header_dsmcc_nptd_

#define DSMCC_NPTD_SIZE                  0x14
#define DSMCC_NPTD_TAG                   0x17
#define DSMCC_NPTD_STC_RESERVED          0x00
#define DSMCC_NPTD_NPT_RESERVED    0x00000000
#define DSMCC_NPTD_MAX_STC      0x1FFFFFFFFLL
#define DSMCC_NPTD_MAX_BASE              0xFF
#define DSMCC_NPTD_MAX_NUM             0xFFFF
#define DSMCC_NPTD_MAX_DEN             0xFFFF


typedef struct _DsmccNptd
{
  unsigned int        npt;
  unsigned long long  stc;
  unsigned char       base;
  unsigned short      num;
  unsigned short      den;
  unsigned char       dis;
  unsigned char      *nptd;
  unsigned int        size;
} DsmccNptd;


DsmccNptd *
dsmcc_nptd_new (unsigned int       npt,
                unsigned long long stc,
                unsigned char      base,
                unsigned short     num,
                unsigned short     den,
                unsigned char      dis);


void
dsmcc_nptd_free (DsmccNptd *nptd);


#endif
