#include "stdio.h"
#include "stdlib.h"
#define clean() printf("\033[H\033[J")

void shellGreet() {
    clean();
    printf("***************Novel Shell****************\n");
    printf("  Authors: Tanner Palin and Will Shapiro\n");
    printf("******************************************\n");
}

void scanInput() { // TODO: Scan user input in, do not need to parse yet.
    printf("%s : %s\n", getenv("USER"), getenv("PWD"));
    clean();
}

int main() {
    // TODO: Do main function things.
    shellGreet();
    scanInput();
    while(1) {}
    return 0;
}