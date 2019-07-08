#ifndef __LIBTORRENT_KT_BINDING_H
#define __LIBTORRENT_KT_BINDING_H

#ifdef __cplusplus
extern "C" {
#endif

int init_host(const char** files, const unsigned int files_count, char* url_buffer, const unsigned int url_buffer_size);
int init_magnet(const char* url);

#ifdef __cplusplus
}
#endif
#endif
