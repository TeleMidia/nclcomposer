// encoding of packetized elementary stream

#ifndef _header_dsmcc_pes_
#define _header_dsmcc_pes_


#define DSMCC_PES_SYNC_BYTE          0x47
#define DSMCC_PES_PACKET_SIZE        0xBC
#define DSMCC_PES_UNIT_START         0x40
#define DSMCC_PES_PAYLOAD_SIZE       0xB8
#define DSMCC_PES_HEADER_OFF         0x04
#define DSMCC_PES_AF_CONTROL         0x10
#define DSMCC_PES_MAX_SEC_SIZE     0x1000
#define DSMCC_PES_MIN_PID          0x0001
#define DSMCC_PES_MAX_PID          0x1FFF
#define DSMCC_PES_MIN_CID      0x00000001
#define DSMCC_PES_MAX_CID      0xFFFFFFFE


typedef struct _DsmccPes
{
  char           *pes;
  unsigned short  pid;
  unsigned char   cc;
} DsmccPes;


DsmccPes *
dsmcc_pes_new (char           *pes,
               unsigned short  pid);


void
dsmcc_pes_free (DsmccPes *pes);


int
dsmcc_pes_encode (DsmccPes      *pes,
                  unsigned char *data,
                  unsigned int   dlen,
                  int            fd);


void
dsmcc_pes_stuff (DsmccPes *pes,
                 int       fd);


#endif
