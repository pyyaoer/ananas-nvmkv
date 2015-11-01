#Assuming that libnvm-primitives.so and libnvmkv.so are in /usr/lib/nvm
#Assuming that libvsl.so is in /usr/lib/fio
#Assuming that public header files are in /usr/include/nvm
#Assuming that device is /dev/fioa
LDFLAGS= -L/usr/lib/nvm -L/usr/lib/fio -lvsl -lnvmkv -lnvm-primitives -ldl -lpthread

CPPFLAGS=-std=gnu99

all: nvm_kv_sample1 nvm_kv_sample2
	LD_LIBRARY_PATH=/usr/lib/nvm ./nvm_kv_sample1 "/dev/fioa"
	LD_LIBRARY_PATH=/usr/lib/nvm ./nvm_kv_sample2 "/dev/fioa"

nvm_kv_sample1: nvm_kv_sample1.o
	$(CC) -o $@ $< $(LDFLAGS)

nvm_kv_sample2: nvm_kv_sample2.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o nvm_kv_sample1 nvm_kv_sample2
