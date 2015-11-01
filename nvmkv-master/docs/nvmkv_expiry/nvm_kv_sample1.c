
#define _GNU_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <nvm/nvm_kv.h>
#include <nvm/nvm_error.h>

#define MAX_DEV_NAME_SIZE 256
#define SECTOR_SIZE 512
#define KEY_LEN 9
#define VALUE_LEN 512


int main(int argc, char **argv)
{
    char                  device_name[MAX_DEV_NAME_SIZE];
    char                  pool_tag[16];
    char                 *key_str = NULL;
    char                 *key_str2 = NULL;
    char                 *key = NULL;
    char                 *value_str = NULL;
    char                 *value_str2 = NULL;
    nvm_kv_key_info_t     key_info;
    nvm_kv_pool_info_t    pool_info;
    nvm_kv_store_info_t   store_info;
    nvm_kv_pool_metadata_t   pool_md;
    int                   optind = 0;
    int                   ret = 0;
    int                   value_len = 0;
    int                   it_id = -1;
    int                   fd = 0;
    int                   pool_id = 0;
    int                   kv_id = 0;
    int                   next_element;
    uint32_t              key_len = 0;
    uint32_t              version = 0;
    uint64_t              cache_size = 4096;
    bool                  read_exact = false;

    if (argc < 2)
    {
        printf("usage:\n");
        printf("./kv_sample <device_name>\n");
        return -1;
    }

    /* Opening the underneath device. device_name must be /dev/fioX where X can be a - z */
    strncpy(device_name, argv[++optind], MAX_DEV_NAME_SIZE);
    if (strncmp(device_name, "/dev/fio", strlen("/dev/fio")) != 0)
    {
        printf("***** Incorrect device name %s *****\n", device_name);
        return -1;
    }

    fd = open(device_name, O_RDWR | O_DIRECT);
    if (fd < 0)
    {
        printf("could not open file %s errno %d\n", device_name, errno);
        return -1;
    }
    printf("Device id = %d\n", fd);


    /*********************/
    /* Open the KV Store */
    /*********************/
    kv_id = nvm_kv_open(fd, version, NVM_KV_MAX_POOLS,
                        KV_GLOBAL_EXPIRY, cache_size);
    if (kv_id < 0)
    {
        printf("nvm_kv_open: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf("nvm_kv_open: success\n");
    printf("\tcreated KV store, kv_id = %d\n", kv_id);


    /************************************/
    /* Get the KV store info            */
    /* Note: this is a blocking API and */
    /* could take a while on a large    */
    /* store.                           */
    /************************************/
    ret = nvm_kv_get_store_info(kv_id, &store_info);
    if (ret < 0)
    {
        printf("nvm_kv_get_store_info: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf("nvm_kv_get_store_info: success\n");
    printf("\tversion              = %lu\n", store_info.version);
    printf("\tnum_pools            = %lu\n", store_info.num_pools);
    printf("\tmax_pools            = %lu\n", store_info.max_pools);
    printf("\texpiry_mode          = %lu\n", store_info.expiry_mode);
    printf("\tglobal_expiry        = %lu\n", store_info.global_expiry);
    printf("\tnum_keys             = %llu\n", store_info.num_keys);
    printf("\tfree_space(in bytes) = %llu\n", store_info.free_space);


    /********************/
    /* Create a KV pool */
    /********************/
    strncpy(pool_tag, "sample_pool", strlen("sample_pool"));
    pool_id = nvm_kv_pool_create(kv_id, (nvm_kv_pool_tag_t *)pool_tag);
    if (pool_id < 0)
    {
        printf("nvm_kv_pool_create: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf("nvm_kv_pool_create: success\n");
    printf("\tcreated pool with pool_id = %d and tag = %s\n", pool_id, pool_tag);


    /********************************/
    /* Get the pool metadata info   */
    /********************************/
    ret = nvm_kv_get_pool_metadata(kv_id, &pool_md, 1, pool_id);
    if (ret < 0)
    {
        printf("nvm_kv_get_pool_metadata: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf("nvm_kv_get_pool_metadata: success ret = %d\n", ret);
    printf("\tpool_id              = %lu\n", pool_md.pool_id);
    printf("\tpool_tag             = %s\n", &pool_md.pool_tag);


    /********************************************/
    /* Create two key buffers of length KEY_LEN */
    /* Note: NVM_KV_MAX_KEY_SIZE 128            */
    /* Fill the key buffers.                    */
    /********************************************/
    key_str = (char *) malloc (sizeof(char) * KEY_LEN);
    if (!key_str)
    {
        printf("memory allocation failed\n");
        goto test_exit;
    }
    key_str2 = (char *) malloc (sizeof(char) * KEY_LEN);
    if (!key_str2)
    {
        printf("memory allocation failed\n");
        goto test_exit;
    }
    strcpy(key_str, "abc_test");
    strcpy(key_str2, "xyz_test");


    /************************************************/
    /* Create two value buffers of length value_len */
    /* The buffers must be sector aligned           */
    /* Note: NVM_KV_MAX_VALUE_SIZE 1047552          */
    /* Fill the value buffers                       */
    /************************************************/
    value_len = VALUE_LEN;
    posix_memalign((void**) &value_str, SECTOR_SIZE, value_len);
    memset(value_str, 'a', value_len);

    posix_memalign((void**) &value_str2, SECTOR_SIZE, value_len);
    memset(value_str2, 'b', value_len);


    /****************************************/
    /* Put the KV pair (key_str, value_str) */
    /****************************************/
    ret = nvm_kv_put(kv_id, pool_id, (nvm_kv_key_t *) key_str,
                    strlen((char *) key_str), value_str, value_len, 0, false, 0);
    if (ret < 0)
    {
        printf("nvm_kv_put: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf ("nvm_kv_put: success\n");

    /******************************************/
    /* Put the KV pair (key_str2, value_str2) */
    /******************************************/
    ret = nvm_kv_put(kv_id, pool_id, (nvm_kv_key_t *) key_str2,
                    strlen((char *) key_str2), value_str2, value_len, 0, false, 0);
    if (ret < 0)
    {
        printf("nvm_kv_put: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf("nvm_kv_put: success\n");


    /*****************************/
    /* Get the current pool info */
    /*****************************/
    ret = nvm_kv_get_pool_info(kv_id, pool_id, &pool_info);
    if (ret < 0)
    {
        printf("nvm_kv_get_pool_info: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf("nvm_kv_get_pool_info: success\n");
    printf("\tversion     = %lu\n", pool_info.version);
    printf("\tpool_status = %lu\n", pool_info.pool_status);


    /**********************/
    /* Close the KV Store */
    /**********************/
    ret = nvm_kv_close(kv_id);
    if (ret < 0)
    {
        printf("nvm_kv_close: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf("nvm_kv_close: success\n");


test_exit:
    free(key);
    free(value_str);
    free(value_str2);
    free(key_str);
    free(key_str2);
    close(fd);

    printf("\n\n");
    exit(0);
}

