//global configuration

#ifndef _header_dsmcc_conf_
#define _header_dsmcc_conf_

#ifndef WIN32
  #ifdef _MSC_VER
    #define WIN32
  #endif
#endif

#include <sys/stat.h>
#include "list.h"


void
dsmcc_conf_init (void);


void
dsmcc_conf_set_in (char *in);


char *
dsmcc_conf_get_in (void);


void
dsmcc_conf_set_oc_out (char *out);


char *
dsmcc_conf_get_oc_out (void);


void
dsmcc_conf_set_npt_out (char *out);


char *
dsmcc_conf_get_npt_out (void);


void
dsmcc_conf_set_ste_out (char *out);


char *
dsmcc_conf_get_ste_out (void);


void
dsmcc_conf_set_ait_out (char *out);


char *
dsmcc_conf_get_ait_out (void);


void
dsmcc_conf_set_pmt_out (char *out);


char *
dsmcc_conf_get_pmt_out (void);


void
dsmcc_conf_set_dsi_info (char *info);


char *
dsmcc_conf_get_dsi_info (void);


unsigned int
dsmcc_conf_get_dsi_info_len (void);


unsigned int
dsmcc_conf_set_pid (unsigned int pid);


unsigned short
dsmcc_conf_get_pid (void);


unsigned int
dsmcc_conf_set_cid (unsigned int cid);

unsigned int
dsmcc_conf_set_did (unsigned int did);

unsigned int
dsmcc_conf_get_cid (void);


unsigned int
dsmcc_conf_set_tag (unsigned char tag);


unsigned char
dsmcc_conf_get_tag (void);


void
dsmcc_conf_set_verb (unsigned int verb);


unsigned int
dsmcc_conf_get_verb (void);


void
dsmcc_conf_set_srg_obj (unsigned int obj);


unsigned int
dsmcc_conf_get_srg_obj (void);


void
dsmcc_conf_set_srg_rep (unsigned int rep);


unsigned int
dsmcc_conf_get_srg_rep (void);


void
dsmcc_conf_set_mount_rep (unsigned int rep);


unsigned int
dsmcc_conf_get_mount_rep (void);


void
dsmcc_conf_set_ste (unsigned int ste);


unsigned int
dsmcc_conf_get_ste (void);


void
dsmcc_conf_set_stuff (unsigned int stuff);


unsigned int
dsmcc_conf_get_stuff (void);


void
dsmcc_conf_set_exps (DsmccList *exps);


DsmccList *
dsmcc_conf_get_exps (void);


unsigned short
dsmcc_conf_get_exps_num (void);


void
dsmcc_conf_set_apps (DsmccList *apps);


DsmccList *
dsmcc_conf_get_apps (void);


void
dsmcc_conf_set_esis (DsmccList *esis);
                                                                                                              
                                                                                                              
DsmccList *
dsmcc_conf_get_esis (void);


unsigned int
dsmcc_conf_get_key (void);


unsigned short
dsmcc_conf_get_mid (void);


unsigned int
dsmcc_conf_get_did (void);


unsigned int
dsmcc_conf_get_id (void);


void
dsmcc_conf_set_up (void);


unsigned int
dsmcc_conf_get_up (void);


void
dsmcc_conf_set_dump_out (char* out);


char *
dsmcc_conf_get_dump_out (void);


void
dsmcc_conf_reset_dc (unsigned short pid,
                     unsigned char  ctag);


unsigned int
dsmcc_conf_dump_write (int fd);


unsigned int
dsmcc_conf_dump_read (int fd);

void
dsmcc_conf_set_tid (int tid);

int
dsmcc_conf_get_tid (void);

#endif
