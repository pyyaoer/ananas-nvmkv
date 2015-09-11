/*******************************************************************
 Copyright 2014 - 2019, Huawei Tech. Co., Ltd.
 ALL RIGHTS RESERVED

 Filename      : kvf_api.h
 Author        : KVF group
 Creation time : 2015.01.05
 Description   : this file define the API of kvf

 Version       : 0.9.0
 ********************************************************************/

#ifndef __KVF_API_H__
#define __KVF_API_H__

/***** Includes *****/
#include "kvf.h"

#ifdef __cplusplus
extern "C"{
#endif

/***** APIs *****/

/*
 	 this section defines the basic operations of the kvf.
 */

/** \brief
 	 	 get a kvf instance
    \note
 */
s32 get_kvf(const char* name, kvf_type_t* type);

/** \brief
 	 	 init a kvf instance
    \note
 */
s32 kvf_init(kvf_type_t* type, char * config_file);

/** \brief
 	 	 shutdown a kvf instance
    \note
 */
s32 kvf_shutdown(kvf_type_t* type);

/** \brief
 	 	 set a property of a kvf instance
    \note
 */
s32 kvf_set_prop(kvf_type_t* type, const char* name, char* value);

/** \brief
 	 	 set a property of a kvf instance
    \note
 */
s32 kvf_get_prop(kvf_type_t* type, const char* name, char* value);

/** \brief
 	 	 alloc a buf
    \note
 */
void* kvf_alloc_buf(kvf_type_t* type, u32 size, s32 flag);

/** \brief
 	 	 free a buf
    \note
 */
void kvf_free_buf(kvf_type_t* type, void** buf);

/** \brief
 	 	 error code to str
    \note
 */
char* kvf_get_errstr(kvf_type_t* type, s32 errcode);

/** \brief
 	 	 get the statistics
    \note
 */
s32 kvf_get_stats(kvf_type_t* type, kvf_stats_t* kvf_stats_t);

/** \brief
 	 	 start a transaction
    \note
 */
s32 kvf_trans_start(kvf_type_t* type, kv_trans_id_t** t_id);

/** \brief
 	 	 commit a transaction
    \note
 */
s32 kvf_trans_commit(kv_trans_id_t* t_id);

/** \brief
 	 	 abort a transaction
    \note
 */
s32 kvf_trans_abort(kv_trans_id_t* t_id);

/** \brief
 	 	 create a pool
    \note
 */
s32 pool_create(const char* name, const char* config_path, pool_t * p);

/** \brief
 	 	 destroy a pool
    \note
 */
s32 pool_destroy(pool_t * p);

/** \brief
 	 	 open a pool
    \note
 */
s32 pool_open(pool_t* p);

/** \brief
 	 	 close a pool
    \note
 */
s32 pool_close(pool_t* p);

/** \brief
 	 	 set the property of a pool
    \note
 */
s32 pool_set_prop(const pool_t* p, const char* name, const char* value);

/** \brief
 	 	 get the property of a pool
    \note
 */
s32 pool_get_prop(const pool_t* p, const char* name, char** value);

/** \brief
 	 	 get the statistics of a pool
    \note
 */
s32 pool_get_stats(pool_stats_t* stats);

/** \brief
 	 	 put a key-value
    \note
 */
s32 put(pool_t* p, const string_t* key, const string_t* value, const kv_props_t* props, const put_options_t* putopts);

/** \brief
 	 	 get a key-value
    \note
 */
s32 get(pool_t* p, const string_t* key, string_t* value, const kv_props_t* props, const get_options_t* getopts);

/** \brief
 	 	 del a key-value
    \note
 */
s32 del(pool_t* p, const string_t* key, const kv_props_t* props, const del_options_t* delopts);

/** \brief
 	 	 batch put a key-value
    \note
 */
s32 mput(pool_t* p, const kv_array_t* kvarray, const kv_props_t* props, const put_options_t* putopts);

/** \brief
 	 	 batch get a key-value
    \note
 */
s32 mget(pool_t* p, kv_array_t* kvarray, const kv_props_t* props, const get_options_t* getopts);

/** \brief
 	 	 batch del a key-value
    \note
 */
s32 mdel(pool_t* p, const kv_array_t* kvarray, const kv_props_t* props, const del_options_t* delopts);

/** \brief
 	 	 open a iterator of a pool
    \note
 */
s32 open_iter(const pool_t* p, const string_t* key_regex, s32 limit, s32 timeout, kv_iter_t* it);

/** \brief
 	 	 iteration
    \note
 */
s32 next_iter(pool_t* p, kv_iter_t* it, kv_array_t* kvarray);

/** \brief
 	 	 close a iterator
    \note
 */
s32 close_iter(pool_t* p, kv_iter_t* it);

/** \brief
 	 	 copy objs from @src to @dest which matches the @regex
    \note
 */
s32 xcopy_obj(const pool_t* src,const pool_t* dest, const string_t* regex);

#ifdef __cplusplus
}
#endif

#endif	//__KVF_API_H__
