#ifndef KVF_TEST_H
#define KVF_TEST_H

#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdbool.h"
#include "stdlib.h"

#include "kvf.h"
#include "kvf-parser.h"
#include "nvm/nvm_kv.h"
#include "nvm/nvm_error.h"

#define NVMKV_KVF_SECTOR_SIZE 512

#ifndef NVMKV_KVF_MAX_DEV_NAME_LEN
#define NVMKV_KVF_MAX_DEV_NAME_LEN 128
#endif

#define NVMKV_KVF_MAX_POOL_NAME_LEN 15
#define NVMKV_KVF_MAX_KEY_LEN 128
#define NVMKV_KVF_MAX_VAL_LEN (4*NVMKV_KVF_SECTOR_SIZE)

extern pool_operations_t nvmkv_pool_ops;
extern kvf_operations_t nvmkv_kvlib_ops;
extern kv_operations_t nvmkv_kv_ops;
extern kvf_type_t nvmkv_kvlib_std;
extern pool_t nvmkv_pool_std;

#endif
