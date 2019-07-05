#include "main.h"
#include <iostream>

extern "C" {
    void printHello() {
        std::cout << "hi!" << std::endl;
    }
}

int main() {
}
