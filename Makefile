GXX_ARGS = -std=c++11 -lboost_system -lpthread -lboost_thread -ltorrent-rasterbar

.PHONY: native/test native/clean native clean;

all: native

native/main.o: native/main.cpp native/main.h
	/usr/bin/g++ -c native/main.cpp -g -fPIC -shared $(GXX_ARGS) -o native/main.o

native/test.o: native/test.c native/main.h
	/usr/bin/gcc -c native/test.c -g -o native/test.o

native/test: native/main.o native/test.o
	/usr/bin/g++ native/main.o native/test.o -g $(GXX_ARGS) -o native/test

native/clean:
	rm -f native/*.o native/test
	touch native/main.cpp

native: native/main.o native/test.o

clean: native/clean
