#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "main.h"

#define URL_BUFFER_SIZE 160

int main() {
    char cwd[URL_BUFFER_SIZE];
    getcwd(cwd, sizeof(cwd));
    
    const char* files[] = { strcat(cwd, "/Makefile") };
    char url[URL_BUFFER_SIZE];
    url[URL_BUFFER_SIZE-1] = '\0';
    
    if (init_host(files, 1, url, URL_BUFFER_SIZE)) {
        printf("Failed to init torrent host.");
        return 1;
    }

    printf("Magic URL: %s\n", url);

    return 0;
}
