GXX_ARGS = -std=c++11 -lboost_system -lpthread -lboost_thread -ltorrent-rasterbar

.PHONY: native/test native/clean;
FORCE: ;

native/main.o: native/main.cpp native/main.h
	/usr/bin/g++ native/main.cpp -fPIC -shared $(GXX_ARGS) -o native/main.o

native/test.o: native/test.c native/main.h
	/usr/bin/gcc -c native/test.c -o native/test.o

native/test: native/main.o native/test.o
	/usr/bin/g++ native/main.o native/test.o $(GXX_ARGS) -o native/test

native/clean:
	rm -f native/*.o native/test
	touch native/main.cpp

clean: native/clean
