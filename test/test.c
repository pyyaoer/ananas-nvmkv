#include "stdio.h"
#include "stdlib.h"
#include "kvf-test.h"
#include "kvf.h"
#include "kvf_api.h"

int main(){

	kvf_type_t*	kvf = &nvmkv_kv_lib;
	pool_t*		pool = malloc(sizeof(pool_t));

	kvf_load("");
	kvf_register(kvf);

	kvf_init(kvf, "");

	pool_create(kvf, "sample_pool", "", pool);
	pool_destroy(pool);

	kvf_shutdown(kvf);

	kvf_unregister(kvf);
	kvf_unload();

	free(pool);

	return 0;
}
