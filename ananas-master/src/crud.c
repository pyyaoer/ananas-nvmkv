/*
 * crud.c
 *
 *  Created on: 2014-11-10
 *      Author: SDI
 */

#include "../include/kvf/kvf.h"

/**
 	/brief
 	     put - put a key-value into a pool.
 	@key: the key
 	@value: the value
 	@props: the properties of this key-value object
 	@putopts: put options

    /note
	   1, the put will copy the @key & @value if zero_copy is not set in putopts, otherwise the kv will not be copied.
 */
s32 put(pool_t* p, const string_t* key, const string_t* value, const kv_props_t* props, const put_options_t* putopts)
{
	KVF_CHECK_INVALID_PARAM_POINTER(key);
	KVF_CHECK_INVALID_PARAM_POINTER(value);
	KVF_CHECK_INVALID_PARAM_POINTER(props);

	return p->kv_ops->put(p, key, value, props, putopts);
}

/**
 	/brief
 	     get - get a key-value from a pool.
 	@key: the key
 	@value: the value
 	@props: the properties of this key-value object
 	@putopts: put options

    /note
		the memory alloced for the value should be free by the caller.
 */
s32 get(pool_t* p, const string_t* key, string_t* value, const kv_props_t* props, const get_options_t* getopts)
{
	KVF_CHECK_INVALID_PARAM_POINTER(key);
	KVF_CHECK_INVALID_PARAM_POINTER(value);
	KVF_CHECK_INVALID_PARAM_POINTER(props);

	return p->kv_ops->get(p, key, value, props, getopts);
}

/**
 	/brief
 	     del - del a key-value from a pool.
 	@key: the key
 	@value: the value
 	@props: the properties of this key-value object
 	@putopts: put options

    /note
		the memory alloced for the value should be free by the caller.
 */
s32 del(pool_t* p, const string_t* key, const kv_props_t* props, const del_options_t* delopts)
{
	KVF_CHECK_INVALID_PARAM_POINTER(key);
	KVF_CHECK_INVALID_PARAM_POINTER(props);

	return p->kv_ops->del(p, key, props, delopts);
}

/**
 	/brief
 	     mput - batch put key-value to a pool.
 	@kvarray: the key-value array
 	@props: the props of the kvarray
 	@putopts: put options

    /note
     	 1, all the key-value in mput should have same props&putopts.
     	 2, the mput will copy the @kvarray if KVPUT_ZERO_CPY is not set in putopts, otherwise the array will not be copied.
 */
s32 mput(pool_t* p, const kv_array_t* kvarray, const kv_props_t* props, const put_options_t* putopts)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvarray);
	KVF_CHECK_INVALID_PARAM_POINTER(props);

	return p->kv_ops->mput(p, kvarray, props, putopts);
}

/**
 	/brief
 	     mget - batch get key-value from a pool.
 	@kvarray: the key-value array
 	@props: the props of the kvarray
 	@putopts: put options

    /note
     	 1, all the key-value in mput should have same props&putopts.
     	 2, the pool will fill the value in the kvarray, the memory will be alloced by the pool,
     	    the caller must free the memory.
 */
s32 mget(pool_t* p, kv_array_t* kvarray, const kv_props_t* props, const get_options_t* getopts)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvarray);
	KVF_CHECK_INVALID_PARAM_POINTER(props);

	return p->kv_ops->mget(p, kvarray, props, getopts);
}

/**
 	/brief
 	     mdel - batch del key-value from a pool.
 	@kvarray: the key-value array
 	@props: the props of the kvarray
 	@putopts: put options

    /note
     	 1, all the key-value in mput should have same props&putopts.
 */
s32 mdel(pool_t* p, const kv_array_t* kvarray, const kv_props_t* props, const del_options_t* delopts)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvarray);
	KVF_CHECK_INVALID_PARAM_POINTER(props);

	return p->kv_ops->mdel(p, kvarray, props, delopts);
}

/**
 	/brief
 	     open_iter - open a iterator from a pool.
 	@pool: 		the pool to iterator
 	@key_regex: the key regex
 	@limit: 	the max count each iteration can hold
 	@timeout:	max time each iteration can perform
 	@it:		iterator handle

    /note
 */
s32 open_iter(pool_t* p, const pool_t* pool,const string_t* key_regex, u32 limit, u32 timeout, kv_iter_t* it)
{
	KVF_CHECK_INVALID_PARAM_POINTER(pool);
	KVF_CHECK_INVALID_PARAM_POINTER(key_regex);

	return p->kv_ops->iter_open(p, key_regex, limit, timeout, it);
}

/**
 	/brief
 	     next_iter - iterate a pool.
 	@it:		iterator handle
 	@kvarray:   kvarray to hold the results

    /note
      1, at most @it->limit kv will get in a round.
      2, the caller should free the kv's memory
 */
s32 next_iter(pool_t* p, kv_iter_t* it, kv_array_t* kvarray)
{
	KVF_CHECK_INVALID_PARAM_POINTER(it);
	KVF_CHECK_INVALID_PARAM_POINTER(kvarray);

	return p->kv_ops->iter_next(p, it, kvarray);
}

/**
 	/brief
 	     close_iter - close a pool.
 	@it:		iterator handle

    /note
 */
s32 close_iter(pool_t* p, kv_iter_t* it)
{
	KVF_CHECK_INVALID_PARAM_POINTER(it);

	return p->kv_ops->iter_close(p, it);
}

/**
 	/brief
 	   xcopy_obj - copy key-values from @src to @dest
 	@src:		the src pool
 	@dest:      the dest pool
 	@regex:		the key's regex

    /note
 */
s32 xcopy_obj(const pool_t* src,const pool_t* dest, const string_t* regex)
{
	KVF_CHECK_INVALID_PARAM_POINTER(src);
	KVF_CHECK_INVALID_PARAM_POINTER(dest);
	KVF_CHECK_INVALID_PARAM_POINTER(regex);

	return src->kv_ops->xcopy(src, dest, regex);
}
