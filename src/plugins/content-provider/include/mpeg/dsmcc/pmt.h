// program map table

#ifndef _header_dsmcc_pmt_
#define _header_dsmcc_pmt_

#include "esi.h"
#include "list.h"


#define DSMCC_PMT_TID           0x02
#define DSMCC_PMT_HEADER_SIZE   0x0C
#define DSMCC_PMT_MAX_SIZE    0x03F4
#define DSMCC_PMT_SIZE_MASK   0xB000
#define DSMCC_PMT_SEC_NUM       0x00
#define DSMCC_PMT_SEC_LAST      0x00
#define DSMCC_PMT_VER_MASK      0xC0
#define DSMCC_PMT_PPID_MASK   0xE000
#define DSMCC_PMT_PINFO_LEN   0xF000
#define DSMCC_PMT_MAX_VER       0xFF


typedef struct _DsmccPmt
{
  unsigned char   tid;
  unsigned char   valid;
  unsigned short  prog;
  unsigned short  ppid;
  unsigned char   ver;
  unsigned short  size;
  unsigned int    crc;
  unsigned int    len;
  unsigned char  *data;
  DsmccList      *esi;
} DsmccPmt;


DsmccPmt *
dsmcc_pmt_new (unsigned char  valid,
               unsigned short prog,
               unsigned short ppid,
               unsigned char  ver);


void
dsmcc_pmt_free (DsmccPmt *pmt);


void
dsmcc_pmt_add_esi (DsmccPmt *pmt,
                   DsmccEsi *esi);


unsigned int
dsmcc_pmt_encode (DsmccPmt *pmt);


#endif
