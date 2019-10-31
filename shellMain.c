#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/wait.h>
#define clean() printf("\033[H\033[J")

void shellGreet() { // Simply clears screen and displays authors.
    clean();
    printf("***************Novel Shell****************\n");
    printf("  Authors: Tanner Palin and Will Shapiro\n");
    printf("******************************************\n");
}

void parseTokens() { // TODO: Parse the user input into Tokens.

}

void scanInput() {
    char* user = getenv("USER");
    char* directory = getenv("PWD");
    printf("%s @ %s \n", user, directory); // Displays user and current directory
    
    // TODO: Scan user input in, do not need to parse yet (Another function for parsing?).
    scanf("$%s", user);    
}

int main() {
    // TODO: Initialize variables buffers, make function calls.
    shellGreet();
    scanInput();
    while(1) {}
    return 0;
}