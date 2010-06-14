//biop file message

#ifndef _header_dsmcc_fil_
#define _header_dsmcc_fil_


#define DSMCC_FIL_SIZE           0x13
#define DSMCC_FIL_DATA_OFF       0x04
#define DSMCC_FIL_OBJ_INFO_LEN 0x0008
#define DSMCC_FIL_SCL_COUNT      0x00


typedef struct _DsmccFil
{
  unsigned int   key;
  unsigned char *fil;
  unsigned int   size;
  unsigned int   len;
  char          *path;
} DsmccFil;


DsmccFil *
dsmcc_fil_new (char          *path,
               unsigned int   key,
               unsigned char *data,
               unsigned int   dlen,
               unsigned int   sin);


void
dsmcc_fil_free (DsmccFil *fil);


#endif
