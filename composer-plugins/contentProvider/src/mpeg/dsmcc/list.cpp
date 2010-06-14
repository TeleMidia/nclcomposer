// simple double linked list

#include "../../../include/mpeg/dsmcc/list.h"

#include <stdlib.h>

#include "../../../include/mpeg/dsmcc/util.h"


DsmccList *
dsmcc_list_new (void)
{
  DsmccList *ret = (DsmccList*)malloc (sizeof (DsmccList));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->first = 0;
  ret->last  = 0;
  ret->size  = 0;

  return ret;
}


void
dsmcc_list_free (DsmccList *list,
                 FreeFunc   func)
{
  dsmcc_list_clear (list, func);
  free (list);
}


void
dsmcc_list_clear (DsmccList *list,
                  FreeFunc   func)
{
  void *data = 0;

  while (dsmcc_list_size (list) > 0)
    {
      data = dsmcc_list_pop_front (list);

      if (func)
        {
          func (data);
        }
    }
}


static DsmccLink *
dsmcc_link_new (void *data)
{
  DsmccLink *ret = (DsmccLink*)malloc (sizeof (DsmccLink));
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  ret->prev = 0;
  ret->next = 0;
  ret->data = data;

  return ret;
}


unsigned int
dsmcc_list_push_front (DsmccList *list,
                       void      *data)
{
  DsmccLink *link = dsmcc_link_new (data);
  if (!link)
    {
      DSMCC_ERR ();
      return 0;
    }

  link->prev = 0;
  link->next = list->first;

  list->first = link;

  if (link->next)
    {
      link->next->prev = link;
    }

  if (!list->last)
    {
      list->last = link;
    }

  list->size++;

  return 1;
}


unsigned int
dsmcc_list_push_back (DsmccList *list,
                      void      *data)
{
  DsmccLink *link = dsmcc_link_new (data);
  if (!link)
    {
      DSMCC_ERR ();
      return 0;
    }

  link->prev = list->last;
  link->next = 0;

  list->last = link;

  if (link->prev)
    {
      link->prev->next = link;
    }

  if (!list->first)
    {
      list->first = link;
    }

  list->size++;

  return 1;
}


void *
dsmcc_list_front (DsmccList *list)
{
  if (list->first)
    {
      return list->first->data;
    }

  return 0;
}


void *
dsmcc_list_back (DsmccList *list)
{
  if (list->last)
    {
      return list->last->data;
    }

  return 0;
}


void *
dsmcc_list_pop_front (DsmccList *list)
{
  void      *data = 0;
  DsmccLink *link = list->first;

  if (!link)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (link == list->last)
    {
      list->first = 0;
      list->last  = 0;
    }
  else
    {
      list->first       = link->next;
      list->first->prev = 0;

      if (list->first->next)
        {
          list->first->next->prev = list->first;
        }
    }
    
  data = link->data;
  free (link);
  list->size--;

  return data;
}


void *
dsmcc_list_pop_back (DsmccList *list)
{
  void      *data = 0;
  DsmccLink *link = list->last;

  if (!link)
    {
      DSMCC_ERR ();
      return 0;
    }

  if (link == list->first)
    {
      list->last  = 0;
      list->first = 0;
    }
  else
    {
      list->last       = link->prev;
      list->last->next = 0;

      if (list->last->prev)
        {
          list->last->prev->next = list->last;
        }
    }

  data = link->data;
  free (link);
  list->size--;

  return data;
}


unsigned int
dsmcc_list_remove (DsmccList *list,
                   void      *data)
{
  DsmccLink *link = 0;

  dsmcc_list_foreach (list, link)
    {
      if (link->data == data)
        {
          if (link == list->first)
            {
              dsmcc_list_pop_front (list);
            }
          else if (link == list->last)
            {
              dsmcc_list_pop_back (list);
            }
          else
            {
              link->prev->next = link->next;
              link->next->prev = link->prev;

              list->size--;

              free (link);
            }

          return 1;
        }
    }

  return 0;
}


unsigned int
dsmcc_list_prepend (DsmccList *list,
                    DsmccLink *before,
                    void      *data)
{
  DsmccLink *link = 0;

  if (!before)
    {
      return dsmcc_list_push_back (list, data);
    }

  link       = dsmcc_link_new (data);
  link->next = before;

  if (before->prev)
    {
      before->prev->next = link;
      link->prev         = before->prev;
    }
  else
    {
      list->first = link;
    }

  before->prev = link;

  list->size++;

  return 1;
}


unsigned int
dsmcc_list_append (DsmccList *list,
                   DsmccLink *after,
                   void      *data)
{
  DsmccLink *link = 0;

  if (!after)
    {
      return dsmcc_list_push_front (list, data);
    }

  link       = dsmcc_link_new (data);
  link->prev = after;

  if (after->next)
    {
      after->next->prev = link;
      link->next        = after->next;
    }
  else
    {
      list->last = link;
    }

  after->next = link;

  list->size++;

  return 1;
}


DsmccLink *
dsmcc_list_find (DsmccList *list,
                 void      *data)
{
  DsmccLink *link = 0;

  dsmcc_list_foreach (list, link)
    {
      if (link->data == data)
        {
          break;
        }
    }

  return link;
}


DsmccList *
dsmcc_list_concat (DsmccList *list,
                   DsmccList *append)
{
  DsmccLink *link = 0;
  DsmccList *ret  = dsmcc_list_new();
  if (!ret)
    {
      DSMCC_ERR ();
      return 0;
    }

  dsmcc_list_foreach (list, link)
    {
      if (!dsmcc_list_push_back (ret, link->data))
        {
          DSMCC_ERR ();
          dsmcc_list_free (ret, 0);

          return 0;
        }
    }

  dsmcc_list_foreach (append, link)
    {
      if (!dsmcc_list_push_back (ret, link->data))
        {
          DSMCC_ERR ();
          dsmcc_list_free (ret, 0);

          return 0;
        }
    }

  return ret;
}


unsigned int
dsmcc_list_size (DsmccList *list)
{
  if (!list)
    {
      return 0;
    }

  return list->size;
}
