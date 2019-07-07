#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "main.h"

int main() {
    char cwd[99];
    getcwd(cwd, sizeof(cwd));
    
    const char* files[] = { strcat(cwd, "/Makefile") };
    char url[99];
    
    init_host(files, 1, url);

    printf("Magic URL: %s\n", url);

    return 0;
}
