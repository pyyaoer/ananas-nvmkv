/*******************************************************************
 Copyright 2014 - 2019, Huawei Tech. Co., Ltd.
 ALL RIGHTS RESERVED

 Filename      : kvf_list.h
 Author        : KVF group
 Creation time : 2015.01.05
 Description   : this file define the list structure of kvf

 Version       : 0.9.0
 ********************************************************************/

#ifndef __KVF_LIST_H__
#define __KVF_LIST_H__

#ifdef __cplusplus
extern "C"{
#endif

/** \brief	struct kvf_list
 	 double linked list
    \note
 */
typedef struct list{
    struct list *next, *prev;
}kvf_list_t;

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) do { \
     struct kvf_list (name) = LIST_HEAD_INIT(name);\
} while (0)

#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

static inline void __list_add(struct list* new_head, struct list* prev, struct list* next)
{
	next->prev = new_head;
	new_head->next = next;
	new_head->prev = prev;
	prev->next = new_head;
}

static inline void list_add(struct list* new_head, struct list* head)
{
	__list_add(new_head, head, head->next);
}

static inline void list_del(struct list* entry)
{
	entry->prev->next = entry->next;
	entry->next->prev = entry->prev;
}

#define list_empty(head) (head->next == head)

#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member))); \

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); \
            pos = pos->next)

#define list_for_del_each(pos, head) \
    for (pos = (head)->next; pos != (head); \
            pos = (head)->next)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

#define list_for_each_prev(pos, head) \
            for (pos = (head)->prev; pos != (head); pos = pos->prev)


#ifdef __cplusplus
}
#endif

#endif	//__KVF_LIST_H__
