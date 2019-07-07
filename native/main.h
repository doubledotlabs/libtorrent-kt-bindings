#ifndef __LIBTORRENT_KT_BINDING_H
#define __LIBTORRENT_KT_BINDING_H

#ifdef __cplusplus
extern "C" {
#endif

void printHello();

void init_host(char* url);
void init_magnet(const char* url);

#ifdef __cplusplus
}
#endif
#endif
