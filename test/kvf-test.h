#ifndef KVF_TEST_H
#define KVF_TEST_H

#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdbool.h"

#include "kvf.h"
#include "nvm/nvm_kv.h"

#define MAX_DEV_NAME_SIZE 256

s32 nvmkv_pool_create(const char* name, const char* config_path, pool_t* pool);
s32 nvmkv_pool_destroy(pool_t* pool);
s32 nvmkv_kvlib_init(kvf_type_t* kvf, const char* config_file);
s32 nvmkv_kvlib_shutdown(kvf_type_t* kvf);

extern pool_operations_t nvmkv_pool_ops;
extern kvf_operations_t nvmkv_kvlib_ops;
extern kvf_type_t nvmkv_kv_lib;

#endif
