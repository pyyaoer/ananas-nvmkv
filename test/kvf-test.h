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

extern pool_operations_t nvmkv_pool_ops;
extern kvf_operations_t nvmkv_kvlib_ops;
extern kv_operations_t nvmkv_kv_ops;
extern kvf_type_t nvmkv_kvlib_std;
extern pool_t nvmkv_pool_std;

#endif
