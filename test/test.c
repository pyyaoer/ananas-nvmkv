#include "stdio.h"
#include "stdlib.h"
#include "kvf-test.h"
#include "kvf.h"
#include "kvf_api.h"

string_t* gen_str(char* c_str, int len){
	if (len <= 0)
		return NULL;
	string_t* s_str = malloc(sizeof(string_t));
	s_str->len = len;
	s_str->data = malloc(len);
	memset(s_str->data, 0, len);
	if (c_str != NULL){
		int c_len = strlen(c_str);
		s_str->len = (len < c_len) ? len : c_len;
		memcpy(s_str->data, c_str, s_str->len);
	}
	return s_str;
}

void del_str(string_t* s_str){
	if (s_str == NULL)
		return;
	free(s_str->data);
	free(s_str);
}

void print_kvf_stats(kvf_stats_t* kvf_stats){
	printf("refcnt: %d\n", kvf_stats->refcnt);
	printf("errs: %d\n", kvf_stats->errs);
	printf("buftotal: %lld\n", kvf_stats->buftotal);
	printf("bufallocated: %lld\n", kvf_stats->bufallocated);
	printf("bufffree: %lld\n", kvf_stats->bufffree);
}


/*
 * Basic progress test
 */
int smoke_test_0(){

	char*		pool_name = "st_0_pool";
	kvf_type_t*	kvf = malloc(sizeof(kvf_type_t));
	pool_t*		pool = malloc(sizeof(pool_t));
	kv_props_t*	props = malloc(sizeof(kv_props_t));

	string_t*	key_fst = gen_str("key_1st", NVMKV_KVF_MAX_KEY_LEN);
	string_t*	val_fst = gen_str("val_1nd", NVMKV_KVF_MAX_VAL_LEN);
	string_t*	key_sec = gen_str("key_2nd", NVMKV_KVF_MAX_KEY_LEN);
	string_t*	val_sec = gen_str("val_2nd", NVMKV_KVF_MAX_VAL_LEN);
	string_t*	val_emp = gen_str(NULL, NVMKV_KVF_MAX_VAL_LEN);

	memcpy(kvf, &nvmkv_kvlib_std, sizeof(kvf_type_t));
	memcpy(pool, &nvmkv_pool_std, sizeof(pool_t));

	kvf_load("");
	kvf_register(kvf);
	kvf_init(kvf, "kvfinitconf.txt");

	pool_create(kvf, pool_name, "", pool);

	put(pool, key_fst, val_fst, props, NULL);
	put(pool, key_sec, val_sec, props, NULL);
	get(pool, key_fst, val_emp, props, NULL);
	get(pool, key_sec, val_emp, props, NULL);
	del(pool, key_fst, props, NULL);
	del(pool, key_sec, props, NULL);


	pool_destroy(pool);

	kvf_shutdown(kvf);
	kvf_unregister(kvf);
	kvf_unload();

smoke_test_0_exit:
	del_str(key_fst);
	del_str(val_fst);
	del_str(key_sec);
	del_str(val_sec);
	del_str(val_emp);
	free(kvf);
	free(pool);
	free(props);
}

/*
 * Smoking test for kvf
 *
 * start kvf
 * -> create pool 
 * -> put kv
 * -> close pool
 * shutdown kvf
 *
 * restart kvf
 * -> reopen pool
 * -> read kv
 * -> destroy pool
 * shutdown kvf
 */
int smoke_test_1(){

	char*		pool_name = "st_1_pool";
	kvf_type_t*	kvf = malloc(sizeof(kvf_type_t));
	pool_t*		pool_fst = malloc(sizeof(pool_t));
	pool_t*		pool_sec = malloc(sizeof(pool_t));
	kv_props_t*	props = malloc(sizeof(kv_props_t));
	kvf_stats_t*	kvf_stats = malloc(sizeof(kvf_stats_t));

	string_t*	key_fst = gen_str("key_1st", NVMKV_KVF_MAX_KEY_LEN);
	string_t*	val_fst = gen_str("val_1st", NVMKV_KVF_MAX_VAL_LEN);
	string_t*	key_sec = gen_str("key_2nd", NVMKV_KVF_MAX_KEY_LEN);
	string_t*	val_sec = gen_str("val_2nd", NVMKV_KVF_MAX_VAL_LEN);
	string_t*	val_emp = gen_str(NULL, NVMKV_KVF_MAX_VAL_LEN);

	memcpy(kvf, &nvmkv_kvlib_std, sizeof(kvf_type_t));
	memcpy(pool_fst, &nvmkv_pool_std, sizeof(pool_t));
	memcpy(pool_sec, &nvmkv_pool_std, sizeof(pool_t));

	printf("\n\nSmoking test 1:\n");
	/* load & register
	 */
	kvf_load("");
	kvf_register(kvf);


	printf("\n\n");
	kvf_init(kvf, "");
	kvf_get_stats(kvf, kvf_stats);
	print_kvf_stats(kvf_stats);
	pool_create(kvf, pool_name, "", pool_fst);
	put(pool_fst, key_fst, val_fst, props, NULL);
	put(pool_fst, key_sec, val_sec, props, NULL);
	pool_close(pool_fst);
	kvf_shutdown(kvf);


	printf("\n\n");
	kvf_init(kvf, "");
	kvf_get_stats(kvf, kvf_stats);
	print_kvf_stats(kvf_stats);
	pool_create(kvf, pool_name, "", pool_sec);
	get(pool_sec, key_fst, val_emp, props, NULL);
	get(pool_sec, key_sec, val_emp, props, NULL);
	del(pool_sec, key_fst, props, NULL);
	del(pool_sec, key_sec, props, NULL);
	pool_destroy(pool_sec);
	printf("**\n");
	kvf_shutdown(kvf);


	printf("\n\n");
	kvf_unregister(kvf);
	kvf_unload();

smoke_test_1_exit:
	del_str(key_fst);
	del_str(val_fst);
	del_str(key_sec);
	del_str(val_sec);
	del_str(val_emp);
	free(kvf);
	free(pool_fst);
	free(pool_sec);
	free(props);
	free(kvf_stats);

	return 0;
}


/*
 * smoking test for iterator
 *
 * create or open a pool and put elements, then iter them
 * BUG: does't delete the pool from kvf
 */
int smoke_test_2(){
	int		i = 0;
	int		limit = 2;
	int		timeout = -1;

	kvf_type_t*	kvf = malloc(sizeof(kvf_type_t));
	pool_t*		pool = malloc(sizeof(pool_t));
	kv_props_t*	props = malloc(sizeof(kv_props_t));
	kv_iter_t*	kv_it = malloc(sizeof(kv_iter_t));
	kv_array_t*	kv_arr = malloc(sizeof(kv_array_t));

	string_t*	key_fst = gen_str("key_1st", NVMKV_KVF_MAX_KEY_LEN);
	string_t*	val_fst = gen_str("val_1st", NVMKV_KVF_MAX_VAL_LEN);
	string_t*	key_sec = gen_str("key_2nd", NVMKV_KVF_MAX_KEY_LEN);
	string_t*	val_sec = gen_str("val_2nd", NVMKV_KVF_MAX_VAL_LEN);
	string_t*	regex = gen_str("regex", NVMKV_KVF_MAX_VAL_LEN);

	memcpy(kvf, &nvmkv_kvlib_std, sizeof(kvf_type_t));
	memcpy(pool, &nvmkv_pool_std, sizeof(pool_t));
	memset(kv_arr, 0, sizeof(kv_array_t));

	printf("\n\nSmoking test 2:\n");
	kvf_load("");
	kvf_register(kvf);
	kvf_init(kvf, "");

	pool_create(kvf, "iter_pool", "", pool);

	put(pool, key_fst, val_fst, props, NULL);
	put(pool, key_sec, val_sec, props, NULL);

	open_iter(pool, regex, limit, timeout, kv_it);
	next_iter(pool, kv_it, kv_arr);
	printf("\titer num: %d\n", kv_arr->kv_num);
	for (i = 0; i < kv_arr->kv_num; i++){
		printf("\t\tkey:%s\tvalue:%s\n", kv_arr->kv_elements[i].kv_key.data, kv_arr->kv_elements[i].kv_value.data);
	}
	next_iter(pool, kv_it, kv_arr);
	printf("\titer num: %d\n", kv_arr->kv_num);
	for (i = 0; i < kv_arr->kv_num; i++){
		printf("\t\tkey:%s\tvalue:%s\n", kv_arr->kv_elements[i].kv_key.data, kv_arr->kv_elements[i].kv_value.data);
	}

	close_iter(pool, kv_it);

	kvf_shutdown(kvf);

smoke_test_2_exit:
	del_str(key_fst);
	del_str(val_fst);
	del_str(key_sec);
	del_str(val_sec);
	del_str(regex);
	free(kvf);
	free(pool);
	free(props);
	free(kv_it);
	if (kv_arr->kv_elements != NULL){
		if (kv_arr->kv_num > 0){
			for(i = 0; i < kv_arr->kv_num; i++){
				free(kv_arr->kv_elements[i].kv_key.data);
				free(kv_arr->kv_elements[i].kv_value.data);
			}
		}
		free(kv_arr->kv_elements);
	}
	free(kv_arr);

	return 0;
}

int main(void){
	smoke_test_0();
	//smoke_test_1();
	//smoke_test_2();
	return 0;
}
