//Application Specification
#ifndef _header_dsmcc_apps_
#define _header_dsmcc_apps_


typedef enum _AppType
{
  at_dvbj = 1,
  at_html = 2
} AppType;


typedef enum _AppProto
{
  ap_oc = 1,
  ap_ip = 2,
} AppProto;


typedef struct _DsmccApps
{
  char           *name;
  unsigned int    orgid;
  unsigned short  appid;
  unsigned char   code;
  AppType         type;
  unsigned char   vmaj;
  unsigned char   vmin;
  unsigned char   vmic;
  AppProto        proto;
  unsigned int    remote;
  unsigned short  onid;
  unsigned short  tsid;
  unsigned short  sid;
  unsigned char   ctag;
  char           *par;
  char           *base;
  char           *ext;
  char           *init;
} DsmccApps;


DsmccApps *
dsmcc_apps_new (char *spec);


void
dsmcc_apps_print (DsmccApps *apps);


void
dsmcc_apps_free (DsmccApps *apps);


#endif
