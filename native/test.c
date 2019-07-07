#include <stdio.h>

#include "main.h"

int main() {
    char url[99];
    init_host(url);

    printf("Magic URL: ");
    printf(url);
    printf("\n");
    
    return 0;
}
