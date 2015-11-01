#include "stdio.h"
#include "stdlib.h"
#include "kvf-test.h"
#include "kvf.h"
#include "kvf_api.h"

#define MAX_KEY_LEN 128
#define MAX_VAL_LEN 2048

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

int main(){

	kvf_type_t*	kvf = malloc(sizeof(kvf_type_t));
	pool_t*		pool_fst = malloc(sizeof(pool_t));
	pool_t*		pool_sec = malloc(sizeof(pool_t));
	kv_props_t*	props = malloc(sizeof(kv_props_t));

	string_t*	key_fst = gen_str("key_1st", MAX_KEY_LEN);
	string_t*	val_fst = gen_str("val_1st", MAX_VAL_LEN);
        string_t*       key_sec = gen_str("key_2nd", MAX_KEY_LEN);
        string_t*       val_sec = gen_str("val_2nd", MAX_VAL_LEN);
	string_t*	val_emp = gen_str(NULL, MAX_VAL_LEN);

	memcpy(kvf, &nvmkv_kvlib_std, sizeof(kvf_type_t));
	memcpy(pool_fst, &nvmkv_pool_std, sizeof(pool_t));
	memcpy(pool_sec, &nvmkv_pool_std, sizeof(pool_t));

	printf("\n\n");
	/* load & register
	 */
	kvf_load("");
	kvf_register(kvf);


	printf("\n\n");
	/* open kvf
	 * create pools
	 * put kv pairs
	 * close pools
	 * shutdown kvf
	 */
	kvf_init(kvf, "");
	pool_create(kvf, "fst_pool", "", pool_fst);
        pool_create(kvf, "sec_pool", "", pool_sec);
	put(pool_fst, key_fst, val_fst, props, NULL);
	put(pool_fst, key_sec, val_sec, props, NULL);
	pool_close(pool_fst);
	pool_close(pool_sec);
	kvf_shutdown(kvf);


	printf("\n\n");
	/* reopen kvf
	 * open pools
	 * get kv pairs
	 * del kv pairs
	 * destroy pools
	 * shutdown kvf
	*/
	kvf_init(kvf, "");
	pool_open(pool_fst);
	pool_open(pool_sec);
        get(pool_fst, key_fst, val_emp, props, NULL);
        get(pool_fst, key_sec, val_emp, props, NULL);
        del(pool_fst, key_fst, props, NULL);
        del(pool_fst, key_sec, props, NULL);
	pool_destroy(pool_fst);
	pool_destroy(pool_sec);
	kvf_shutdown(kvf);


	printf("\n\n");
	/* unregister & unload
	 */
	kvf_unregister(kvf);
	kvf_unload();

test_exit:
	del_str(key_fst);
	del_str(val_fst);
	del_str(key_sec);
	del_str(val_sec);
	del_str(val_emp);
	free(kvf);
	free(pool_fst);
	free(pool_sec);
	free(props);

	return 0;
}
