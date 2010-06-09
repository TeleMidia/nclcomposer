// global defines
                                                                                                                                                                                                                                
#ifndef _header_dsmcc_def_
#define _header_dsmcc_def_

#include <stdio.h>

#include "conf.h"


typedef enum _ObjectType
{
  ot_non = 0,
  ot_ext,
  ot_dir,
  ot_fil,
  ot_str,
  ot_ste,
  ot_srg
} ObjectType;


#define DSMCC_DATA_SRG 0x73726700
#define DSMCC_DATA_DIR 0x64697200
#define DSMCC_DATA_FIL 0x66696C00
#define DSMCC_DATA_STR 0x73747200
#define DSMCC_DATA_STE 0x73746500

#define DSMCC_FORMAT_STR "<DSMCC-MHP-TOOLS-FORMAT-OBJECT-STREAM-DESCRIPTION>"
#define DSMCC_FORMAT_STE "<DSMCC-MHP-TOOLS-FORMAT-OBJECT-STREAM-EVENT-DESCRIPTION>"
#define DSMCC_FORMAT_INF "<DSMCC-MHP-TOOLS-FORMAT-OBJECT-STREAM-INFORMATION>"
#define DSMCC_FORMAT_IDS "<DSMCC-MHP-TOOLS-FORMAT-OBJECT-STREAM-EVENT-NAMES-IDS>"

#define DSMCC_STREAM_MIN_ID     0x0001
#define DSMCC_STREAM_MAX_DIN_ID 0x3FFF
#define DSMCC_STREAM_MIN_SCH_ID 0x8000
#define DSMCC_STREAM_MAX_ID     0xBFFF

#define DSMCC_SRG_DEFAULT_LABEL   "Service-Gateway"
#define DSMCC_NPT_DEFAULT_BITRATE 0x00015F90


#define DSMCC_ERR()                                       \
  {                                                       \
    printf ("%s:%u *** ERROR ***\n", __FILE__, __LINE__); \
    fflush (0);                                           \
  }

#define DSMCC_MERR(x...)                                \
  {                                                     \
    printf ("%s:%u *** ERROR ***", __FILE__, __LINE__); \
    printf (" : "x);                                    \
    printf ("\n");                                      \
    fflush (0);                                         \
  }

#define DSMCC_WARN()                                        \
  {                                                         \
    printf ("%s:%u *** WARNING ***\n", __FILE__, __LINE__); \
    fflush (0);                                             \
  }

#define DSMCC_MWARN(x...)                                 \
  {                                                       \
    printf ("%s:%u *** WARNING ***", __FILE__, __LINE__); \
    printf (" : "x);                                      \
    printf ("\n");                                        \
    fflush (0);                                           \
  }

#define DSMCC_MSG(x...)        \
  {                            \
    if (dsmcc_conf_get_verb()) \
      {                        \
        printf (x);            \
        fflush (0);            \
      }                        \
  }


#endif
