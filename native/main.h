#ifndef __LIBTORRENT_KT_BINDING_H
#define __LIBTORRENT_KT_BINDING_H

#ifdef __cplusplus
extern "C" {
#endif

void printHello();

int init_host(const char** files, const unsigned int files_count, char* to_url);
int init_magnet(const char* url);

#ifdef __cplusplus
}
#endif
#endif
