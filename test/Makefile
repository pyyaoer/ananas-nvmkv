#Assuming that libnvm-primitives.so and libnvmkv.so are in /usr/lib/nvm
#Assuming that libvsl.so is in /usr/lib/fio
#Assuming that public header files are in /usr/include/nvm
#Assuming that device is /dev/fioa

#Assuming that the root repository of kvf is /path/to/ananas
#If you have not yet run "make install", change it to your own path
#Otherwise, just justify PATH_TO_ANANAS_INC and PATH_TO_ANANAS_LIB

#PATH_TO_ANANAS= /path/to/ananas
PATH_TO_ANANAS= /home/xiaoyang/Documents/ananas-nvmkv/test-ananas

PATH_TO_ANANAS_INC= $(PATH_TO_ANANAS)/include/kvf
PATH_TO_ANANAS_LIB= $(PATH_TO_ANANAS)/lib/.libs
PATH_TO_NVMKV_INC=/usr/include/nvm
PATH_TO_NVMKV_LIB=/usr/lib/nvm
PATH_TO_VSL_LIB=/usr/lib/fio

LDFLAGS= -I$(PATH_TO_ANANAS_INC) -I$(PATH_TO_NVMKV_INC) -L$(PATH_TO_ANANAS_LIB) -L$(PATH_TO_NVMKV_LIB) -L$(PATH_TO_VSL_LIB)

LDLIBS= -lkvf -lnvmkv -lvsl -lnvm-primitives -ldl -lpthread

CPPFLAGS=-std=gnu99

all: test
	LD_LIBRARY_PATH=$(PATH_TO_ANANAS_LIB):$(PATH_TO_NVMKV_LIB) ./test

test: test.o kvf-test.o parser/test.tab.o parser/lex.yy.o
	$(CC) -o $@ $^ $(LDLIBS) $(LDFLAGS)

test.o: test.c kvf-test.h
	$(CC) -c $< $(LDFLAGS)

kvf-test.o: kvf-test.c kvf-test.h
	$(CC) -c $< $(LDFLAGS)

clean:
	rm -f *.o test
