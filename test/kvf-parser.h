
#define NVMKV_KVF_MAX_DEV_NAME_LEN 128

typedef struct{
	int poolNum;
	int cacheSize;
	char devPath[NVMKV_KVF_MAX_DEV_NAME_LEN];
}kvfInitParser;

int kvfinit_parser(const char* file, kvfInitParser* kp);
