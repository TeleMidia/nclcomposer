// encoding of packetized elementary stream

#include "../../../include/mpeg/dsmcc/pes.h"

#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/mpeg/dsmcc/util.h"


static unsigned char packet[DSMCC_PES_PACKET_SIZE];


DsmccPes *
dsmcc_pes_new (char           *pes,
               unsigned short  pid)
{
  DsmccPes *ret = 0;

  ret = (DsmccPes*)malloc (sizeof (DsmccPes));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->pes = strdup (pes);
  ret->pid = pid;
  ret->cc  = 0;

  return ret;
}


void
dsmcc_pes_free (DsmccPes *pes)
{
  if (pes->pes)
    {
      free (pes->pes);
    }

  free (pes);
}


int
dsmcc_pes_encode (DsmccPes      *pes,
                  unsigned char *data,
                  unsigned int   dlen,
                  int            fd)
{
  int           pos  = 0;
  unsigned char cc   = pes->cc;
  unsigned char pidu = (pes->pid >> 8);
  unsigned char pidl = (pes->pid & 0x00FF);

  if (dlen < 1)
    {
      DSMCC_MERR ("(0x%04x)", pes->pid);
      return 0;
    }

  if (dlen > DSMCC_PES_MAX_SEC_SIZE)
    {
      DSMCC_MERR ("(0x%04x)", pes->pid);
      return 0;
    }

  memset (packet, 0xFF, DSMCC_PES_PACKET_SIZE);
  packet[0] = DSMCC_PES_SYNC_BYTE;
  packet[1] = (DSMCC_PES_UNIT_START | pidu);
  packet[2] = pidl;
  packet[3] = (DSMCC_PES_AF_CONTROL | cc);
  packet[4] = 0;

  if (dlen >= (DSMCC_PES_PAYLOAD_SIZE - 1))
    {
      memcpy (packet + 5, data, DSMCC_PES_PAYLOAD_SIZE - 1);
    }
  else
    {
      memcpy (packet + 5, data, dlen);
    }

  write (fd, packet, DSMCC_PES_PACKET_SIZE);

  pos = DSMCC_PES_PAYLOAD_SIZE - 1;
  cc  = (cc + 1) % 16;

  while ((unsigned int)pos < dlen)
    {
      memset (packet, 0xFF, DSMCC_PES_PACKET_SIZE);
      packet[0] = DSMCC_PES_SYNC_BYTE;
      packet[1] = pidu;
      packet[2] = pidl;
      packet[3] = (DSMCC_PES_AF_CONTROL | cc);

      if ((dlen - pos) >= DSMCC_PES_PAYLOAD_SIZE)
        {
          memcpy (packet + 4, data + pos, DSMCC_PES_PAYLOAD_SIZE);
        }
      else
        {
          memcpy (packet + 4, data + pos, dlen - pos);
        }

      write (fd, packet, DSMCC_PES_PACKET_SIZE);

      pos += DSMCC_PES_PAYLOAD_SIZE;
      cc   = (cc + 1) % 16;
    }

  pes->cc = cc;

  return pos;
}


void
dsmcc_pes_stuff (DsmccPes *pes,
                 int       fd)
{
  unsigned char cc   = pes->cc;
  unsigned char pidu = (pes->pid >> 8);
  unsigned char pidl = (pes->pid & 0x00FF);

  while (cc)
    {
      memset (packet, 0xFF, DSMCC_PES_PACKET_SIZE);
      packet[0] = DSMCC_PES_SYNC_BYTE;
      packet[1] = pidu;
      packet[2] = pidl;
      packet[3] = (DSMCC_PES_AF_CONTROL | cc);

      write (fd, packet, DSMCC_PES_PACKET_SIZE);

      cc = (cc + 1) % 16;
    }

  pes->cc = cc;
}
