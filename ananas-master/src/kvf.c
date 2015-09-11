/*******************************************************************
 Copyright 2014 - 2019, Huawei Tech. Co., Ltd.
 ALL RIGHTS RESERVED

 Filename      : kvf.c
 Author        : KVF group
 Creation time : 2014.11.1
 Description   : this file implments the kvf operations.

 Version       : 0.9.0
 ********************************************************************/

#include "../include/kvf/kvf.h"
#include "../include/kvf/kvf_err_code.h"

/*
 * the global kvf list.
 */
static struct list g_kvf_list;

static kvf_type_t* _kvf_find_impl(const char* name)
{
	kvf_list_t* pos = NULL;
	kvf_type_t* node = NULL;
	list_for_each(pos, &g_kvf_list)
	{
		node = list_entry(pos, kvf_type_t, kvf_node);
		//ok, find the kvf instance
		if (strncmp(node->name, name, strlen(node->name)) == 0)
		{
			return node;
		}
	}

	return NULL;
}

/**
 	/brief
 	     kvf_register - register the kvf instance to the kv framework
 	@kvf:  kvf to register

    /note
 */
s32 kvf_register(kvf_type_t * kvf)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvf);

	if (_kvf_find_impl(kvf->name) != NULL)
	{
		return KVF_ALREADY_EXISTED;
	}

	list_add(&kvf->kvf_node, &g_kvf_list);

	return RET_OK;
}

/**
 	/brief
 	     kvf_unregister - unregister the kvf instance from the kv framework
 	@kvf:  kvf to unregister

    /note
 */
s32 kvf_unregister(kvf_type_t * kvf)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvf);

	if (_kvf_find_impl(kvf->name) != NULL)
	{
		return KVF_NOT_EXISTED;
	}

	//TODO: check the reference
	list_del(&(kvf->kvf_node));
	INIT_LIST_HEAD(&(kvf->kvf_node));

	return RET_OK;
}

/**
 	/brief
 	     find_kvf - find a kvf instance, NULL if not find.
 	@name: the name of the kvf

    /note
		it's not allowed modify the kvf outside.
 */
kvf_type_t* get_kvf(const char* name)
{
	KVF_CHECK_POINTER_PARAM_WITHRET(name, NULL);

	return _kvf_find_impl(name);
}

/**
 	/brief
 	     kvf_load - Key Value Framework  initialization.
 	@config_file: the name of the kvf

    /note
		it's not allowed modify the kvf outside.
 */
void kvf_load(const char* config_file)
{
	KVF_CHECK_POINTER_PARAM(config_file);

	INIT_LIST_HEAD(&g_kvf_list);

	//TODO: log
}

/**
 	/brief
 	     kvf_unload - Key Value Framework  uninitialization.

    /note
		the caller should free .
 */
void kvf_unload()
{
	kvf_list_t* pos = NULL;
	kvf_type_t* node = NULL;
	list_for_del_each(pos, &g_kvf_list)
	{
		//1) remove from the list
		node = list_entry(pos, kvf_type_t, kvf_node);

		list_del(&(node->kvf_node));
		INIT_LIST_HEAD(&(node->kvf_node));

		//2) the caller should gurantee that all resources has been released before unload the KVF.
	}

	INIT_LIST_HEAD(&g_kvf_list);
	//TODO: log
}

/**
 	/brief
 	     init_kvf - init the kvf instance
 	@kvf:  		  the kvf to init
 	@config_file: the config file

    /note
     for now, just call the kvf's init directly.
     the config file should be recognized by the specified kvf instance itself.
 */
s32 kvf_init(kvf_type_t* kvf, const char* config_file)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvf);
	KVF_CHECK_INVALID_PARAM_POINTER(config_file);

	return kvf->kvf_ops->init(config_file);
}

/**
 	/brief
 	     shutdown_kvf - shutdown the kvf instance
 	@kvf:  the kvf to init

    /note
     for now, just call the kvf's shutdown directly.
 */
s32 kvf_shutdown(kvf_type_t* kvf)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvf);
	return kvf->kvf_ops->shutdown();
}

/**
 	/brief
 	     set_kvf_prop - set a property of @kvf
 	@kvf:   the kvf_type struct
 	@name:  the property name
 	@value: the property value

    /note
     for now, just call the kvf's set_prop directly.
 */
s32 kvf_set_prop(kvf_type_t* kvf, const char* name, char* value)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvf);
	KVF_CHECK_NAME(name);
	KVF_CHECK_INVALID_PARAM_POINTER(value);
	//TODO: check the value length?

	return kvf->kvf_ops->set_prop(name, value);
}

/**
 	/brief
 	     get_kvf_prop - get a property of @kvf
 	@kvf:   the kvf_type struct
 	@name:  the property name
 	@value: the property value

    /note
     for now, just call the kvf's get_prop directly.
 */
s32 kvf_get_prop(kvf_type_t* kvf, const char* name, char* value)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvf);
	KVF_CHECK_INVALID_PARAM_POINTER(value);
	KVF_CHECK_NAME(name);

	//TODO: check the value length?
	return kvf->kvf_ops->get_prop(name, value);
}

/** /brief
 	     alloc_kvf_buf - alloc a buf by the kvf
 	@kvf:   the kvf_type struct
 	@size:  the size to alloc
 	@flag: the flags 0-not aligned, 1-4K aligned

    /note
     this function is used when the caller want to let the kvf to alloc the buffer,
     then the kvf should make zero-copy of the buffer.
 */
void* kvf_alloc_buf(kvf_type_t* kvf, u32 size, s32 flag)
{
	KVF_CHECK_POINTER_PARAM_WITHRET(kvf, NULL);

	//check flag is valid
	return (void*)(kvf->kvf_ops->alloc_buf(size, flag));
}

/**
 	/brief
 	     free_kvf_buf - free a buf
 	@kvf:   the kvf_type struct
 	@buf:   the buf to free

    /note
     for now, just call the kvf's free_buf directly.
 */
void kvf_free_buf(kvf_type_t* kvf, void** buf)
{
	KVF_CHECK_POINTER_PARAM(kvf);
	KVF_CHECK_POINTER_PARAM(buf);

	kvf->kvf_ops->free_buf(buf);
}

/**
 	/brief
 	     get_kvf_error - get kvf error string of the corresponding error code
 	@kvf:   the kvf_type struct
 	@buf:   error code

    /note
     for now, just call the kvf's free_buf directly.
 */
const char* kvf_get_errstr(kvf_type_t* kvf, s32 err_code)
{
	KVF_CHECK_POINTER_PARAM_WITHRET(kvf, NULL);

	return kvf->kvf_ops->get_errstr(err_code);
}

/**
 	/brief
 	     get_kvf_stats - get the kvf stastics
 	@kvf:   the kvf_type struct
 	@stats: the stats

    /note
 */
s32 kvf_get_stats(kvf_type_t* kvf, kvf_stats_t* stats)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvf);
	KVF_CHECK_INVALID_PARAM_POINTER(stats);
	return kvf->kvf_ops->get_stats(stats);
}

/**
 	/brief
 	     start_kvf_trans - start a transaction
 	@kvf:   the kvf_type struct
 	@stats: the transaction id

    /note
 */
s32 kvf_trans_start(kvf_type_t* kvf, kv_trans_id_t** t_id)
{
	KVF_CHECK_INVALID_PARAM_POINTER(kvf);
	KVF_CHECK_INVALID_PARAM_POINTER(t_id);

	s32 ret = kvf->kvf_ops->trans_start(t_id);
	(*t_id)->kvf = kvf;

	return ret;
}

/**
 	/brief
 	     commit_kvf_trans - commit a transaction
 	@stats: the transaction id

    /note
 */
s32 kvf_trans_commit(kv_trans_id_t* t_id)
{
	KVF_CHECK_INVALID_PARAM_POINTER(t_id);

	return t_id->kvf->kvf_ops->trans_commit(t_id);
}

/**
 	/brief
 	     abort_kvf_trans - abort a transaction
 	@stats: the transaction id

    /note
 */
s32 kvf_trans_abort(kv_trans_id_t* t_id)
{
	KVF_CHECK_INVALID_PARAM_POINTER(t_id);

	return t_id->kvf->kvf_ops->trans_abort(t_id);
}

