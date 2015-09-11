/*******************************************************************
 Copyright 2014 - 2019, Huawei Tech. Co., Ltd.
 ALL RIGHTS RESERVED

 Filename      : pool.c
 Author        : KVF group
 Creation time : 2014.11.1
 Description   : this file implments the pool operations of kvf.

 Version       : 0.9.0
 ********************************************************************/

#include "../include/kvf/kvf.h"

/**
 	/brief
 	     create_pool - create a pool with @name and @config_path by specifying the type(@kvf).
 	@kvf: the kvf type
 	@name: the name of the pool
 	@config_path: the path of the config
 	@pool: the pool structure

    /note
     the create pool function will directly call the specified pool create method, and then
     add the pool into the kvf @pool_list.
 */
s32 pool_create(kvf_type_t* kvf, const char* name, const char* config_path, pool_t* pool)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvf);
	KVF_CHECK_NAME(name);
	KVF_CHECK_INVALID_PARAM_POINTER(config_path);
	KVF_CHECK_INVALID_PARAM_POINTER(pool);

	//1) create the pool
	s32 ret = kvf->pool_ops->create(name, config_path, pool);

	//2) add the pool to kvf
	if (RET_OK == ret)
	{
		list_add(&pool->link, &kvf->pool_list);
	}

	return ret;
}

/**
 	/brief
 	     destroy_pool - destroy the pool.
 	@pool: the pool structure

    /note
         the pool wil be removed from the pool list after being destroyed.
 */
s32 pool_destroy(pool_t * pool)
{
	KVF_CHECK_INVALID_PARAM_POINTER(pool);

	s32 ret =  pool->kvf->pool_ops->destroy(pool);

	if (RET_OK == ret)
	{
		list_del(&pool->link);
		INIT_LIST_HEAD(&pool->link);
	}

	return ret;
}

/**
 	/brief
 	     open - open the pool.
 	@pool: the pool structure

    /note
     	 before one can perform CRUD and other operations of a pool, one should open it.
     	 this operation will init the necessary resources that CRUD&other opeartions needed,
     	 such as sockets connections etc.
 */
s32 pool_open(pool_t * pool)
{
	KVF_CHECK_INVALID_PARAM_POINTER(pool);

	return pool->kvf->pool_ops->open(pool);
}

/**
 	/brief
 	     close - close the pool.
 	@pool: the pool structure

    /note
 */
s32 pool_close(pool_t* pool)
{
	KVF_CHECK_INVALID_PARAM_POINTER(pool);

	return pool->kvf->pool_ops->close(pool);
}

/**
 	/brief
 	     pool_set_prop - set the property of a pool.
 	@pool: the pool structure
 	@name: property name
 	@value: property value

    /note
 */
s32 pool_set_prop(const pool_t* pool, const char* name, const char* value)
{
	KVF_CHECK_INVALID_PARAM_POINTER(pool);
	KVF_CHECK_NAME(name);
	KVF_CHECK_INVALID_PARAM_POINTER(value);

	return pool->kvf->pool_ops->set_prop(pool, name, value);
}

/**
 	/brief
 	     get_pool_prop - get the property of a pool.
 	@pool: the pool structure
 	@name: property name
 	@value: buffer to hold value

    /note
     	 the caller should alloc a buffer to hold the value whose size should match the restriction of the KVF.
 */
s32 pool_get_prop(const pool_t* pool, const char* name, char** value)
{
	KVF_CHECK_INVALID_PARAM_POINTER(pool);
	KVF_CHECK_NAME(name);
	KVF_CHECK_INVALID_PARAM_POINTER(value);

	return pool->kvf->pool_ops->get_prop(pool, name, value);
}

/**
 	/brief
 	     get_pool_stats - get the statistics of a pool.
 	@pool: the pool structure
 	@stats: the pool_stats_t structure

    /note
 */
s32 pool_get_stats(const pool_t* pool, pool_stats_t* stats)
{
	KVF_CHECK_INVALID_PARAM_POINTER(pool);
	KVF_CHECK_INVALID_PARAM_POINTER(stats);

	return pool->kvf->pool_ops->get_stats(stats);
}
