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

char * parseTokens(char *instructionString) {
    printf("this is the instructionString %s", instructionString);
    for(int i = 0; i > sizeof(instructionString); i++){
        
    }
    
    return instructionString; // TODO: Parse the user input into Tokens.

}
void anyText(){

} //TODO: 

void variableAssignment(){

} //TODO: 

char * scanInput() {
    char *userInput;
    userInput =  malloc(sizeof(userInput)*(100));
    char* user = getenv("USER");
    char* directory = getenv("PWD");
    printf("%s @ %s $ ", user, directory); // Displays user and current directory
    
    // TODO: Scan user input in, do not need to parse yet (Another function for parsing?).
    fgets(userInput,100, stdin);
    //printf("%ld",sizeof(userInput));    
    //printf("%s\n", userInput);

    
    return userInput;
}

int main() {
    int quit = 0;
    // TODO: Initialize variables buffers, make function calls.
    shellGreet();
    
    while(!quit) {
        char * inputToConsole = scanInput();
        // printf("%s\n", inputToConsole);
        if(strncmp(inputToConsole, "bye",3)==0){
            quit = 1;
        }
        char * successOrFailure = parseTokens(inputToConsole);

        }
    return 0;
}
