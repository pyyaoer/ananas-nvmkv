#include "kvf-test.h"

s32 nvmkv_pool_create(const char* name, const char* config_path, pool_t* pool){

        int             pool_id = -1;
        kvf_type_t*     kvf = pool->kvf;

	if (strlen(name) > NVMKV_KVF_MAX_POOL_NAME_LEN){
		printf("Pool name %s too long!\n", name);
		return -1;
	}

        pool_id = nvm_kv_pool_create(kvf->kvfid, (nvm_kv_pool_tag_t *)name);
        if (pool_id < 0){
                printf("nvm_kv_pool_create: failed, errno = %d\n", errno);
                return -1;
        }
        pool->pool_id = pool_id;
        pool->pool_name = (char*)name;
        printf("kvf pool %s created! pool_id = %d\n", name, pool_id);
        return RET_OK;
}

s32 nvmkv_pool_destroy(pool_t* pool){
        int                     ret = -1;
        kvf_type_t*             kvf = pool->kvf;
        nvm_kv_pool_info_t      pool_info;

        ret = nvm_kv_pool_delete(kvf->kvfid, pool->pool_id);
        if (ret < 0){
                printf("nvm_kv_pool_delete: failed, errno = %d\n", errno);
                return -1;
        }

        while (1){
                ret = nvm_kv_get_pool_info(kvf->kvfid, pool->pool_id, &pool_info);
                if (ret < 0){
                        printf("nvm_kv_get_pool_info failed with errno %d\n", errno);
                        return -1;
                }
                if (pool_info.pool_status == POOL_NOT_IN_USE){
                        printf("kvf pool %s deleted completely!\n", pool->pool_name);
                        break;
                }
        }
        printf("kvf pool %s destroyed!\n", pool->pool_name);
        return RET_OK;
}

s32 nvmkv_pool_open(pool_t* pool){
	return RET_OK;
}

s32 nvmkv_pool_close(pool_t* pool){
	return RET_OK;
}

s32 nvmkv_kvlib_init(kvf_type_t* kvf, const char* config_file){
	int     fd = -1;
	int     kvfid = -1;
	int     cache_size = 4096;
	char    device_name[NVMKV_KVF_MAX_DEV_NAME_LEN];

        strncpy(device_name, "/dev/fioa", NVMKV_KVF_MAX_DEV_NAME_LEN);
        fd = open(device_name, O_RDWR);
        if (fd < 0){
                printf("Cannot open file %s, errno: %d\n", device_name, errno);
                return -1;
        }
        printf("Device fd = %d\n", fd);
        kvf->devfd = fd;

        kvfid = nvm_kv_open(fd, 0, NVM_KV_MAX_POOLS, KV_GLOBAL_EXPIRY, cache_size);
        if (kvfid < 0){
                printf("nvm_kv_open failed, errno: %d\n", errno);
                close(fd);
                return -1;
        }
        printf("kvf %s created! kvfid = %d\n", kvf->name, kvfid);
        kvf->kvfid = kvfid;

        return RET_OK;
}

s32 nvmkv_kvlib_shutdown(kvf_type_t* kvf){
        int     fd = kvf->devfd;
        int     kvfid = kvf->kvfid;
        int     ret = -1;

        ret = nvm_kv_close(kvfid);
        if (ret < 0){
                printf("nvm_kv_close: failed, errno = %d\n", errno);
                return -1;
        }

        close(fd);
        printf("kvf %s shutdown!\n", kvf->name);
        return RET_OK;
}

s32 nvmkv_kvlib_getstats(kvf_type_t* kvf, kvf_stats_t* kvfstats){
	int			ret = -1;
	int			kvfid = kvf->kvfid;
	nvm_kv_store_info_t	store_info;

	memset(kvfstats, 0, sizeof(kvf_stats_t));
	ret = nvm_kv_get_store_info(kvfid, &store_info);
	if (ret < 0){
		printf("nvm_kv_get_store_info: failed, errno = %d\n", errno);
		return -1;
	}
	kvfstats->refcnt = 0;
	kvfstats->errs = 0;
	kvfstats->buftotal = -1;
	kvfstats->bufallocated = -1;
	kvfstats->bufffree = store_info.free_space;

	return RET_OK;
}

s32 nvmkv_kv_put(pool_t* pool, const string_t* key, const string_t* value, const kv_props_t* props, const put_options_t* putopts){
	int	ret = -1;
	kvf_type_t* kvf = pool->kvf;

	ret = nvm_kv_put(kvf->kvfid, pool->pool_id, (nvm_kv_key_t *) key->data, key->len, value->data, value->len, 0, true, 0);
	if (ret < 0){
		printf("nvm_kv_put: failed, errno = %d\n", errno);
		return -1;
	}
	printf("kvf put kv into pool %s!\n\tkey:\t%s\n\tvalue:\t%s\n", pool->pool_name, key->data, value->data);
}

s32 nvmkv_kv_get(pool_t* pool, const string_t* key, string_t* value, const kv_props_t* props, const get_options_t* getopts){
        int     		ret = -1;
        kvf_type_t* 		kvf = pool->kvf;
	nvm_kv_key_info_t	key_info;

        ret = nvm_kv_get(kvf->kvfid, pool->pool_id, (nvm_kv_key_t *) key->data, key->len, value->data, value->len, true, &key_info);
        if (ret < 0){
                printf("nvm_kv_get: failed, errno = %d\n", errno);
                return -1;
        }
        printf("kvf get kv from pool %s!\n\tkey:\t%s\n\tvalue:\t%s\n", pool->pool_name, key->data, value->data);
}

s32 nvmkv_kv_del(pool_t* pool, const string_t* key, const kv_props_t* props, const del_options_t* delopts){
        int     ret = -1;
        kvf_type_t* kvf = pool->kvf;

        ret = nvm_kv_delete(kvf->kvfid, pool->pool_id, (nvm_kv_key_t *) key->data, key->len);
        if (ret < 0){
                printf("nvm_kv_del: failed, errno = %d\n", errno);
                return -1;
        }
        printf("kvf del kv from pool %s!\n\tkey:\t%s\n", pool->pool_name, key->data);
}

s32 nvmkv_kvit_open(const pool_t* pool, const string_t* key_regex, s32 limit, s32 timeout, kv_iter_t* it){
	int 		ret = -1;
	kvf_type_t*	kvf = pool->kvf;

	if (limit <= 0){
		printf("limit should be positive!\n");
		return -1;
	}
	
	ret = nvm_kv_begin(kvf->kvfid, pool->pool_id);
	if (ret < 0){
		printf("nvm_kv_begin: failed, errno = %d\n", errno);
		return -1;
	}
	printf("kvf iter in pool %s! iter id:%d\n", pool->pool_name, ret);
	it->id = ret;
	it->limit = limit;
	it->timeout = timeout;
	return RET_OK;
}

s32 nvmkv_kvit_next(pool_t* pool, struct kv_iter* it, kv_array_t* kvarray){
	int			i = 0;
	int			ret = -1;
	kvf_type_t*     	kvf = pool->kvf;
	string_t*		kv_key;
	string_t*		kv_value;
	nvm_kv_key_info_t	key_info;

	if (kvarray->kv_elements != NULL){
		if (kvarray->kv_num > 0){
			for(i = 0; i < kvarray->kv_num; i++){
				free(kvarray->kv_elements[i].kv_key.data);
				free(kvarray->kv_elements[i].kv_value.data);
			}
		}
		free(kvarray->kv_elements);
		kvarray->kv_elements = NULL;
	}
	kvarray->kv_num = 0;
	kvarray->kv_elements = malloc(it->limit * sizeof(kv_t));

	for(i = 0; i < it->limit; i++){
		// set key info
		kv_key = &(kvarray->kv_elements[i].kv_key);
		kv_key->len = NVMKV_KVF_MAX_KEY_LEN;
		kv_key->data = malloc(NVMKV_KVF_MAX_KEY_LEN);

		// set value info
		kv_value = &(kvarray->kv_elements[i].kv_value);
		kv_value->len = NVMKV_KVF_MAX_VAL_LEN;
		kv_value->data = malloc(NVMKV_KVF_MAX_VAL_LEN);

		// get the current kv
		ret = nvm_kv_get_current(kvf->kvfid, it->id, (nvm_kv_key_t *)kv_key->data, &kv_key->len, kv_value->data, kv_value->len, &key_info);
		if (ret < 0){
			printf("nvm_kv_get_current: failed, errno = %d\n", errno);
			break;
		}
		kv_key->len = key_info.key_len;
		kv_value->len = key_info.value_len;
		kvarray->kv_num ++;

		// get the next kv
		ret = nvm_kv_next(kvf->kvfid, it->id);
		if (ret != 0){
			if (errno != -NVM_ERR_OBJECT_NOT_FOUND){
				printf("nvm_kv_next: failed, errno = %d\n", errno);
			}
			break;
		}
	}
	return RET_OK;
}

s32 nvmkv_kvit_close(pool_t* p, struct kv_iter* it){
	int		ret = -1;
	kvf_type_t*	kvf = p->kvf;

	ret = nvm_kv_iteration_end(kvf->kvfid, it->id);
	if (ret < 0){
		printf("nvm_kv_iteration_end: failed, errno = %d\n", errno);
		return -1;
	}
	return RET_OK;
}

pool_operations_t nvmkv_pool_ops = {
        .create = nvmkv_pool_create,
        .destroy = nvmkv_pool_destroy,
        .open = nvmkv_pool_open,
        .close = nvmkv_pool_close,
        .set_prop = NULL,
        .get_prop = NULL,
        .get_stats = NULL
};

kvf_operations_t nvmkv_kvlib_ops = {
        .init = nvmkv_kvlib_init,
        .shutdown = nvmkv_kvlib_shutdown,
        .set_prop = NULL,
        .get_prop = NULL,
        .alloc_buf = NULL,
        .free_buf = NULL,
        .get_errstr = NULL,
        .get_stats = nvmkv_kvlib_getstats,
        .trans_start = NULL,
        .trans_commit = NULL,
        .trans_abort = NULL
};

kv_operations_t nvmkv_kv_ops = {
	.put = nvmkv_kv_put,
	.get = nvmkv_kv_get,
	.del = nvmkv_kv_del,
	.mput = NULL,
	.mget = NULL,
	.mdel = NULL,
	.async_put = NULL,
	.async_update = NULL,
	.async_get = NULL,
	.async_del = NULL,
	.iter_open = nvmkv_kvit_open,
	.iter_next = nvmkv_kvit_next,
	.iter_close = nvmkv_kvit_close,
	.iter_pos_deserialize = NULL,
	.iter_pos_serialize = NULL,
	.xcopy = NULL
}; 

kvf_type_t nvmkv_kvlib_std = {
        .name = "nvm_kvlib",
        .kvf_ops = &nvmkv_kvlib_ops,
        .pool_ops = &nvmkv_pool_ops
};

pool_t nvmkv_pool_std = {
	.kv_ops = &nvmkv_kv_ops
};
