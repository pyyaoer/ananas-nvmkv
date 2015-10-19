#include "stdio.h"
#include "kvf.h"
#include "nvm_kv.h"

s32 nvmkv_pool_create(const char* name, const char* config_path, pool_t* pool){
        return 0;
}

s32 nvmkv_pool_destroy(pool_t* pool){
        return 0;
}

s32 nvmkv_kvlib_init(kvf_type_t* kvf, const char* config_file){
        printf("kvf %s init!\n", kvf->name);
        return 0;
}

s32 nvmkv_kvlib_shutdown(kvf_type_t* kvf){
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
        .name = "nvm_kvlib",
        .kvf_ops = &nvmkv_kvlib_ops,
        .pool_ops = &nvmkv_pool_ops
};

