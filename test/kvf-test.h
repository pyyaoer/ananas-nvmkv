#define _GNU_SOURCE

#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdbool.h"

#include "kvf.h"
#include "nvm/nvm_kv.h"

#define MAX_DEV_NAME_SIZE 256


s32 nvmkv_pool_create(const char* name, const char* config_path, pool_t* pool){
        return 0;
}

s32 nvmkv_pool_destroy(pool_t* pool){
        return 0;
}

s32 nvmkv_kvlib_init(kvf_type_t* kvf, const char* config_file){
	int	fd = -1;
	int	kvfid = -1;
	int	cache_size = 4096;
	char	device_name[MAX_DEV_NAME_SIZE];

        strncpy(device_name, "/dev/fioa", MAX_DEV_NAME_SIZE);
        //fd = open(device_name, O_RDWR | O_DIRECT);
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

	return 0;
}

s32 nvmkv_kvlib_shutdown(kvf_type_t* kvf){
	int	fd = kvf->devfd;
	int	kvfid = kvf->kvfid;
	int	ret = -1;

	ret = nvm_kv_close(kvfid);
	if (ret < 0){
		printf("nvm_kv_close: failed, errno = %d\n", errno);
		return -1;
	}

	close(fd);
        printf("kvf %s shutdown!\n", kvf->name);
        return 0;
}

pool_operations_t nvmkv_pool_ops = {
        .create = nvmkv_pool_create,
        .destroy = nvmkv_pool_destroy,
        .open = NULL,
        .close = NULL,
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
        .get_stats = NULL,
        .trans_start = NULL,
        .trans_commit = NULL,
        .trans_abort = NULL
};

kvf_type_t nvmkv_kv_lib = {
        .magic = 0,
        .flags = 0,
        .kvfid = 0,
        .devfd = 0,
	.name = "nvm_kvlib",
        .kvf_ops = &nvmkv_kvlib_ops,
        .pool_ops = &nvmkv_pool_ops
};

