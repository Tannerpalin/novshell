#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/wait.h>
#define clean() printf("\033[H\033[J")

struct variableMap { // Map to store variables entered into terminal.
    char varKey[512];
    char varValue[512];
}map[50];

struct procStatus {
    int proId;          // the process id
    char status[512];
    int onExit;         // Status upon completion of execution
}pro[50];

void initStatii () {
    for(int i =0; i < 50; i++) {
        strcpy(pro[i].status,"");
        pro[i].proId = -1;
        pro[i].onExit = -1;
    }
}

int getProcess (int pid) {  // Gets the index of a certain process based on process id.
    for(int i = 0; i < 50; i++) {
        if(pro[i].proId == pid) {
            return i;
        }
    }
    return -1;              // Returns -1 if not found (failed to find process)
}

void shellGreet() { // Simply clears screen and displays authors.
    clean();
    printf("***************Novel Shell****************\n");
    printf("  Authors: Tanner Palin and Will Shapiro\n");
    printf("******************************************\n");
}

char * parseTokens(char *instructionString) { // Breaks up input into tokens
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
    // TODO: Initialize variables, buffers, and make function calls.
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
