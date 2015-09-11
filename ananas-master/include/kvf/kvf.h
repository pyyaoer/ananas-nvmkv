/*******************************************************************
 Copyright 2014 - 2019, Huawei Tech. Co., Ltd.
 ALL RIGHTS RESERVED

 Filename      : kvf.h
 Author        : KVF group
 Creation time : 2014.11.1
 Description   : this file define the basic types in kvf.

 Version       : 0.9.0
 ********************************************************************/

#ifndef __KVF_H__
#define __KVF_H__

/***** Includes *****/

#include <stdio.h>
#include <string.h>

#include "kvf_type.h"
#include "kvf_list.h"
#include "kvf_err_code.h"

#ifdef __cplusplus
extern "C" {
#endif

/***** Type definitions *****/

/** \brief	kvf flags
 	\note
 */
typedef enum kvf_flags
{
	KVF_NULL = 0,
	KVF_FLAG_STANDALONE = 1,
	KVF_FLAG_DISTRIBUTED = 2,
	KVF_FLAG_MULTIDATACENTER = 3,
} kvf_flags_t;

/** \brief	kvf magics
 	\note
 */
typedef enum kvf_magic
{
	KVF_MAGIC_NULL = 0,
	KVF_MAGIC_LDB = 0x0101,
	KVF_MAGIC_SEAGATE = 0x0201,
	KVF_MAGIC_SANDISK = 0x0301,
	KVF_MAGIC_WD = 0x0401,
} kvf_magic_t;

struct kvf_operations;
struct pool_operations;

/** \brief	kvf type
 	 the kvf type represents a kind of key value storage pool which provies the basic pool semantics and operations.
    \note
 */
typedef struct kvf_type
{
	s32 magic; 	//magic of the kvf
	s32 flags; 	//flags
	char* name; //the name should be unique

	struct kvf_operations* kvf_ops; //the kvf operations
	struct pool_operations* pool_ops; //the pool operations of the kvf

	struct list kvf_list; 	//all the kvf instance are linked together
	struct list pool_list; 	//all of pool instance of a kvf are linked together
	void* kvf_private;
} kvf_type_t;

/** \brief	kvf type help macro
 \note
 */
#define KVF_TYPE_CPCTOR(kvf1, kvf2)\
	do{\
		kvf2->flags = kvf1->flags;\
		kvf2->kvf_ops = kvf1->kvf_ops;\
		kvf2->kvf_pools = kvf1->kvf_pools;\
		kvf2->kvf_private = kvf1->kvf_private;\
		kvf2->magic = kvf1->magic;\
		kvf2->name = kvf1->name;\
		kvf2->pool_ops = kvf1->pool_ops;\
	}while(0)

#define KVF_NAME_MAX_LEN 1024

//TODO: check the name
#define KVF_CHECK_NAME(n)\
do{\
	if ( (NULL == n) || (strnlen(n, KVF_NAME_MAX_LEN) > (KVF_NAME_MAX_LEN - 1)) )\
	{\
		return INVALID_PARAM;\
	}\
}while(0)

#define KVF_CHECK_POINTER_PARAM(p)\
do{\
	if (NULL == (p))\
	{\
		return;\
	}\
}while(0)

#define KVF_CHECK_POINTER_PARAM_WITHRET(p, ret)\
do{\
	if (NULL == (p))\
	{\
		return (ret);\
	}\
}while(0)

#define KVF_CHECK_INVALID_PARAM_POINTER(p) KVF_CHECK_POINTER_PARAM_WITHRET(p, INVALID_PARAM)

/** \brief	kvf stats, some usefull statistics
 \note
 */
typedef struct kvf_stats {
	u32 refcnt;
	u32 errs;
	u64 buftotal;
	u64 bufallocated;
	u64 bufffree;
} kvf_stats_t;

/** \brief	kvf trans id
 \note
 */
typedef struct kvf_trans_id {
	u64 id;
	kvf_type_t* kvf;
	void* trans_private;
} kv_trans_id_t;

/** \brief	the kvf operations, each kvf should implement these operations.
 \note
 */
typedef struct kvf_operations {
	s32 (*init)(const char* config_file);
	s32 (*shutdown)();
	s32 (*set_prop)(const char* name, char* value);
	s32 (*get_prop)(const char* name, char* value);
	void* (*alloc_buf)(u32 size, s32 flag);
	void (*free_buf)(void** buf);
	const char* (*get_errstr)(s32 errcode);
	s32 (*get_stats)(kvf_stats_t * kvfstats);
	s32 (*trans_start)(kv_trans_id_t** t_id);
	s32 (*trans_commit)(kv_trans_id_t* t_id);
	s32 (*trans_abort)(kv_trans_id_t* t_id);
} kvf_operations_t;

/*
 * pool defination
 */
typedef struct pl_location_ipv4 {
	u8 addr[4];
	u32 port;
} pl_location_ipv4_t;

typedef struct pl_location_ipv6 {
	u8 addr[6];
	u32 port;
} pl_location_ipv6_t;

typedef struct pl_location_dev {
	u32 dev_no;
} pl_location_dev_t;

enum pl_location_type {
	PL_LOCATION_NULL = 0,
	PL_LOCATION_IPV4 = 1,
	PL_LOCATION_IPV6 = 2,
	PL_LOCATION_DEV = 3,
	PL_lOCATION_ID = 4,
};

typedef struct pool_location {
	u8 pool_loc_type;
	union {
		pl_location_ipv4_t loc_ipv4;
		pl_location_ipv6_t loc_ipv6;
		pl_location_dev_t loc_dev;
		void* id;
	} pool_loc;
} pool_location_t;

typedef struct pl_redundancy_mirror {
	u32 mirror_num;
} pl_redundancy_mirror_t;

typedef struct pl_redundancy_parity {
	u32 data_num;
	u32 parity_num;
} pl_redundancy_parity_t;

typedef struct pl_redundancy_multidc {
	u32 multidc_rto;
	u32 multidc_rpo;
} pl_redundancy_multidc_t;

enum pl_redundancy_type {
	PL_REDUNDANCY_NULL = 0,
	PL_REDUNDANCY_SINGLE = 1,
	PL_REDUNDANCY_MIRROR = 2,
	PL_REDUNDANCY_PARITY_FIX = 3,
	PL_REDUNDANCY_PARITY_MAXIMUM = 4,
	PL_REDUNDANCY_MULTIDC = 5,
};

typedef struct pool_redundancy {
	u32 pool_redundancy_type;
	union {
		pl_redundancy_mirror_t mir;
		pl_redundancy_parity_t parity;
		pl_redundancy_multidc_t multdc;
	} pool_redund;
} pool_redundancy_t;

typedef struct kv_operations obj_operations_t;

/** \brief
 	 	 the struct pool define the basic key-value storage resource, it can be open/close, also provide the CRUD operations
 	 and a set of other operations such as mput/mget/iterations etc.
 \note
 */
typedef struct pool {
	/*
	 * unique pool name
	 */
	char* pool_name;

	/*
	 * kvf type
	 */
	kvf_type_t* kvf;

	/*
	 *the location info
	 */
	pool_location_t* pool_location;

	/*
	 * space info
	 */
	u64 pool_physical_capacity, pool_physical_used, pool_physical_free;		//For physical storage statistics.
	u64 pool_logical_capacity, pool_logical_used, pool_logical_free;		//For logical storage statistics, especially thin feature, logical size maybe larger than physical.

	/*
	 * redundancy policy
	 */
	pool_redundancy_t* pool_availability;

	/*
	 * pool's sla
	 */
	u64 pool_latency, pool_throughput;
	u64 pool_obj_cksum_type, pool_obj_cksum_lengh;
	u64 pool_obj_compress_type;

	/*
	 * key-value operations
	 */
	struct kv_operations* kv_ops;

	/*
	 * pool list link
	 */
	struct list link;

	void* pool_private;
} pool_t;

typedef struct pool_stats {
	u64 kv_num;
	u64 kv_putnum, kv_getnum, kv_del;
	void* pool_stats_private;
} pool_stats_t;

/** \brief
 *
 *  the struct pool_operations represents the generic opertions of a pool.
 *
 */
typedef struct pool_operations {
	s32 (*create)(const char* name, const char* config_path, pool_t* pool);
	s32 (*destroy)(pool_t * pool);
	s32 (*open)(pool_t * pool);
	s32 (*close)(pool_t* pool);
	s32 (*set_prop)(const pool_t* pool, const char* name, const char* value);
	s32 (*get_prop)(const pool_t* pool, const char* name, char** value);
	s32 (*get_stats)(pool_stats_t* stats);
} pool_operations_t;

/**************************************************************************
 * this section define the put/get/del options.
 * the options defined here are used to control the behavior of the CRUD.
 **************************************************************************/
enum SET_FLAGS {
	KV_NULL = 0, /*clear or default OPTION*/
	KV_SET = 1, /*set the OPTION*/
};

/** \brief
 use this macro to set the options.
 */
#define SET_OPTION(opts, name) ((opts)->o_##name = KV_SET)
#define CLEAR_OPTION(opts, name) ((opts)->o_##name = KV_NULL)
#define TEST_OPTION(opts, name) (((opts)->o_##name) & KV_SET)

typedef struct put_options {
	s8 o_zero_copy;
	s8 o_write_through;
	void* o_private;
} put_options_t;

/** \brief
 init the put option
 */
#define PUT_OPTIONS_INIT(opts)\
do{\
	CLEAR_OPTION(opts, zero_copy);\
	CLEAR_OPTION(opts, write_through);\
	(opts)->o_private = NULL;\
}while(0)

typedef struct get_options {
	s8 o_ignore_any_faults;
	s8 o_zero_copy;
	void* o_private;
} get_options_t;

/** \brief
 init the get option
 */
#define GET_OPTIONS_INIT(opts)\
do{\
	CLEAR_OPTION(opts, zero_copy);\
	CLEAR_OPTION(opts, ignore_any_faults);\
	(opts)->o_private = NULL;\
}while(0)

typedef struct del_options {
	s8 o_mark_del;
	void* o_private;
} del_options_t;

/** \brief
 init the del option
 */
#define DEL_OPTIONS_INIT(opts)\
do{\
	CLEAR_OPTION(opts, mark_del);\
	(opts)->o_private = NULL;\
}while(0)

/** \brief
 *
 *  the struct kv represents a basic put/get/del unit.
 *
 */
typedef struct kv {
	string_t kv_key;
	string_t kv_value;
} kv_t;

/** \brief
 *
 *  an array of kv objs.
 *
 */
typedef struct kv_array {
	u32 kv_num;
	kv_t* kv_elements;
} kv_array_t;

/** \brief
 *
 *  properties of a kv obj.
 *
 */
typedef struct kv_props {
	void* kv_private;
} kv_props_t;

struct kv_iter;

typedef s32 (*async_crud_cb) (pool_t *, const string_t *, const string_t *, const kv_props_t *, void *, s32 err_code);

/** \brief
 *
 *  the struct kv_operations.
 *
 */
typedef struct kv_operations {
	/*
	 * basic CRUD
	 */
	s32 (*put)(pool_t* p, const string_t* key, const string_t* value,
			const kv_props_t* props, const put_options_t* putopts);
	s32 (*get)(pool_t* p, const string_t* key, string_t* value, const kv_props_t* props,
			const get_options_t* getopts);
	s32 (*del)(pool_t* p, const string_t* key, const kv_props_t* props,
			const del_options_t* delopts);

	/*
	 * batch CRUD
	 */
	s32 (*mput)(pool_t* p, const kv_array_t* kvarray, const kv_props_t* props,
			const put_options_t* putopts);
	s32 (*mget)(pool_t* p, kv_array_t* kvarray, const kv_props_t* props,
			const get_options_t* getopts);
	s32 (*mdel)(pool_t* p, const kv_array_t* kvarray, const kv_props_t* props,
			const del_options_t* delopts);

	/*
	* async operations
	*/	
	s32 (*async_put)(pool_t* pool, const string_t* key, const string_t* value, const kv_props_t* props, const put_options_t* putopts, async_crud_cb put_fn);
	s32 (*async_update)(pool_t* pool, const string_t* key, const string_t* value, const kv_props_t* props, const put_options_t* putopts,async_crud_cb put_fn);
	s32 (*async_get)(pool_t* pool, const string_t* key, string_t* value, const kv_props_t* props, const get_options_t* getopts, async_crud_cb  get_fn);
	s32 (*async_del)(pool_t*  pool, const string_t* key, const string_t* value, const kv_props_t* props, const del_options_t* delopts, async_crud_cb del_fn);		
	/*
	 * iterator
	 */
	s32 (*iter_open)(const pool_t* p, const string_t* key_regex, s32 limit,
			s32 timeout, struct kv_iter* it);
	s32 (*iter_next)(pool_t* p, struct kv_iter* it, kv_array_t* kvarray);
	s32 (*iter_close)(pool_t* p, struct kv_iter* it);
	
	s32 (*iter_pos_deserialize)(void** w, s8* buf, u32 len);
	s32 (*iter_pos_serialize)(void* w, s8** buf, u32* len);
	
	/*
	 * peer to peer copy
	 */
	s32 (*xcopy)(const pool_t* src, const pool_t* dest, const string_t* regex);
} kv_operations_t;

/********************************************************************
 * this section define the iterator.
 ********************************************************************/
/** \brief
 *
 *  the struct kv_iter represents a iterator of a kv pool.
 *
 */
typedef struct kv_iter {
	u64 id;
	s32 limit;
	s32 timeout;
	string_t key_regex;
	void* where;
} kv_iter_t;

/***** APIs *****/
s32 kvf_register(kvf_type_t * t);
s32 kvf_unregister(kvf_type_t * t);
kvf_type_t* get_kvf(const char* name);

#ifdef __cplusplus
}
#endif

#endif
