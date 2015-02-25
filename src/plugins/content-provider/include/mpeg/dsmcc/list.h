// simple double linked list

#ifndef _header_dsmcc_list_
#define _header_dsmcc_list_


typedef void (*FreeFunc) (void *ptr);


typedef struct _DsmccLink
{
  void              *data;
  struct _DsmccLink *prev;
  struct _DsmccLink *next;
} DsmccLink;


typedef struct _DsmccList
{   
  DsmccLink    *first;
  DsmccLink    *last;
  unsigned int  size;
} DsmccList;


DsmccList *
dsmcc_list_new (void);


void
dsmcc_list_free (DsmccList *list,
                 FreeFunc   func);


void
dsmcc_list_clear (DsmccList *list,
                  FreeFunc   func);


unsigned int
dsmcc_list_push_front (DsmccList *list,
                       void      *data);


unsigned int
dsmcc_list_push_back (DsmccList *list,
                      void      *data);


void *
dsmcc_list_front (DsmccList *list);


void *
dsmcc_list_back (DsmccList *list);


void *
dsmcc_list_pop_front (DsmccList *list);


void *
dsmcc_list_pop_back (DsmccList *list);


unsigned int
dsmcc_list_remove (DsmccList *list,
                   void      *data);


unsigned int
dsmcc_list_prepend (DsmccList *list,
                    DsmccLink *before,
                    void      *data);


unsigned int
dsmcc_list_append (DsmccList *list,
                   DsmccLink *after,
                   void      *data);


DsmccLink *
dsmcc_list_find (DsmccList *list,
                 void      *data);


DsmccList *
dsmcc_list_concat (DsmccList *list,
                   DsmccList *append);


unsigned int
dsmcc_list_size (DsmccList *list);


#define dsmcc_list_foreach(list,link) \
          for(link=(list)->first;link;link=link->next)


#endif
