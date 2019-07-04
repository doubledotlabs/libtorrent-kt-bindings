.PHONY: libcpp

libcpp: libcpp-clean libcpp-build libcpp-test

libcpp-clean:
	rm libtorrent-data/*.o

libcpp-build:
	/usr/bin/g++ libtorrent-data/main.cpp -lpthread -o libtorrent-data/main.o

libcpp-test:
	./libtorrent-data/main.o
