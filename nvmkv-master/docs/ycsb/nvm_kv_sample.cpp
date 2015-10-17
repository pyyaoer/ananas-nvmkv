//----------------------------------------------------------------------------
// NVMKV
// |- Copyright 2012-2013 Fusion-io, Inc.

// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License version 2 as published by the Free
// Software Foundation;
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General
// Public License v2 for more details.
// A copy of the GNU General Public License v2 is provided with this package and
// can also be found at: http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA 02111-1307 USA.
//----------------------------------------------------------------------------
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
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

#include <cstdio>
#include "ReadYCSB.h"

#define MAX_DEV_NAME_SIZE 256
#define SECTOR_SIZE 512
#define KEY_LEN 128
#define VALUE_LEN 2048

using namespace std;
using namespace ReadYCSB;

int main(int argc, char **argv)
{
    char                  device_name[MAX_DEV_NAME_SIZE];
    char                  pool_tag[16];
    char                 *key_str = NULL;
    char                 *value_str = NULL;
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

    vector<node>          workloads;
    vector<node>          transactions;
    int                   i;

    if (argc < 2)
    {
        printf("usage:\n");
        printf("./kv_sample <device_name>\n");
        return -1;
    }

    key_len = KEY_LEN;
    posix_memalign((void**) &key_str, SECTOR_SIZE, key_len);

    value_len = VALUE_LEN;
    posix_memalign((void**) &value_str, SECTOR_SIZE, value_len);

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


    /********************/
    /* Create a KV pool */
    /********************/
    strcpy(pool_tag, "sample_pool");
    pool_id = nvm_kv_pool_create(kv_id, (nvm_kv_pool_tag_t *)pool_tag);
    if (pool_id < 0)
    {
        printf("nvm_kv_pool_create: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf("nvm_kv_pool_create: success\n");
    printf("\tcreated pool with pool_id = %d and tag = %s\n", pool_id, pool_tag);


    /******************************************/
    /* Put the KV pair from the YCSB workload */
    /******************************************/
    workloads = parse("./nvmkv/loaddata_a.dat");
    for (auto p = workloads.begin(); p != workloads.end(); ++p)
    {
        strcpy(key_str, p->key.c_str());
        strcpy(value_str, p->value.c_str());
        ret = nvm_kv_put(kv_id, pool_id, (nvm_kv_key_t *) key_str,
                strlen(key_str), value_str, strlen(value_str), 0, false, 0);
        if (ret < 0)
        {
            printf("nvm_kv_put: failed, errno = %d\n", errno);
            goto test_exit;
        }
    }
    printf("nvm_kv_put: success\n");


    /*************************************/
    /* Test the YCSB transactions        */
    /*************************************/
    transactions = parse("./nvmkv/transactions_b.dat");
    for (auto p = transactions.begin(); p != transactions.end(); ++p)
    {
        strcpy(key_str, p->key.c_str());
        strcpy(value_str, p->value.c_str());
        switch(p->type)
        {
            case 0:{
                ret = nvm_kv_put(kv_id, pool_id, (nvm_kv_key_t *) key_str,
                    strlen(key_str), value_str, strlen(value_str), 0, false, 0);
                }break;
            case 1:{
//                ret = nvm_kv_put(kv_id, pool_id, (nvm_kv_key_t *) key_str,
//                    strlen(key_str), value_str, strlen(value_str), 0, true, 0);
                ret = nvm_kv_exists(kv_id, pool_id, (nvm_kv_key_t *) key_str, strlen(key_str), &key_info);
                if (ret > 0)
                {
                    ret = nvm_kv_delete(kv_id, pool_id, (nvm_kv_key_t *) key_str,
                        strlen(key_str));
                    ret = nvm_kv_put(kv_id, pool_id, (nvm_kv_key_t *) key_str,
                        strlen(key_str), value_str, strlen(value_str), 0, false, 0);
                }
                }break;
            case 2:{
                ret = nvm_kv_get(kv_id, pool_id, (nvm_kv_key_t *) key_str,
                    strlen(key_str), value_str, value_len, true, &key_info);
                }break;
        }
        if (ret < 0)
        {
            const char* errinfo[2] = {"put", "get"};
            printf("nvm_kv_%s: failed, errno = %d\n", errinfo[p->type>1], errno);
            printf("%s\n", p->key.c_str());
            goto test_exit;
        }
    }


    /*****************************/
    /* Delete all KV pairs       */
    /* This could take a while   */
    /* on a large store          */
    /*****************************/
    printf("nvm_kv_delete_all: deleting all KV pairs\n");
    ret = nvm_kv_delete_all(kv_id);
    if (ret < 0)
    {
        printf("nvm_kv_delete_all: failed, errno = %d\n", errno);
        goto test_exit;
    }
    printf("nvm_kv_delete_all: success\n");


    /*****************************/
    /* Delete the current pool   */
    /*****************************/
    ret = nvm_kv_pool_delete(kv_id, pool_id);
    if (ret < 0)
    {
        printf("nvm_kv_pool_delete: failed, errno = %d\n", errno);
        goto test_exit;
    }

    //Wait till pools are completely deleted
    while (1)
    {
        ret = nvm_kv_get_pool_info(kv_id, pool_id, &pool_info);
        if (ret < 0)
        {
            printf("nvm_kv_get_pool_info failed with errno %d\n",
                    errno);
            goto test_exit;
        }
        if (pool_info.pool_status == POOL_NOT_IN_USE)
        {
            printf("pool deleted completely pool status = %d\n",
                    pool_info.pool_status);
            break;
        }
    }
    printf("nvm_kv_pool_delete: success\n");


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
    free(value_str);
    close(fd);
    exit(0);
}

