LDFLAGS= -I/path/to/ananas/include/kvf -L/path/to/ananas/lib/.libs -lkvf

all: kvf-test
	LD_LIBRARY_PATH=/path/to/ananas/lib/.libs ./kvf-test

kvf-test: kvf-test.h

clean:
	rm -f *.o kvf-test
