#include "stdio.h"
#include "kvf-test.h"
#include "kvf.h"
#include "kvf_api.h"

int main(){

	kvf_load("");
	kvf_register(&nvmkv_kv_lib);

	kvf_init(&nvmkv_kv_lib, "");
	kvf_shutdown(&nvmkv_kv_lib);

	kvf_unregister(&nvmkv_kv_lib);
	kvf_unload();

	return 0;
}
