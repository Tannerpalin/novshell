#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include<sys/wait.h>
#define clean() printf("\033[H\033[J")

/* Start of the in-shell variable storage methods */
struct variableMap { // Map to store variables entered into terminal.
    char varKey[512];
    char varValue[512];
}varMap[50];

void varMapInit () {    // Initializes our variable map.
    strcpy(varMap[0].varValue,"$");     // varMap[0] will be used to hold the prompt prefix.
    strcpy(varMap[0].varKey,"PROMPT");
    for(int j = 1; j < 50; j++) {
        strcpy(varMap[j].varValue,"");
        strcpy(varMap[j].varKey,"");
    }
    strcpy(varMap[7].varValue,"Valuefound");
    strcpy(varMap[7].varKey,"test");
}

int getVarIndex (char* name) { // Returns the index of a variable if key already exists.
    int index = -1;
    for (int i = 0; i < 50; i++) {
        if(strcmp(varMap[i].varKey, name) == 0) {
            return i;
        }
    }
    return index; // Returns a -1 if there is not an existing variable key.
}

char* getVarVal (char* name) {
    int varIndex = getVarIndex(name);
    if(varIndex < 0) {
        char * error;
        perror("ERROR: Could not find variable!"); // If variable not found, error!
        error = "error_finding_value";
        return error;
    }
    char* value = malloc(sizeof(varMap[varIndex].varValue));
    strcpy(value, varMap[varIndex].varValue);
    return value;
}

void addVar (char* name, char* value) { // Adds a new variable or updates preexisting one.
    int varIndex = getVarIndex(name);
    if(strcmp(varMap[0].varKey, name) == 0) { // Update prompt and return if PROMPT is var name.
        strcpy(varMap[0].varValue, value);
        return;
    }
    if (varIndex > 0) { // Update variable since the variable name exists.
        strcpy(varMap[varIndex].varValue, value);
        strcpy(varMap[varIndex].varKey, name);
    }
    else if(varIndex == -1) { // New variable, need to find a space and add it.
        int j = 1;
        while(!strcmp(varMap[j].varKey, "")){
            j++;
        }
        strcpy(varMap[j].varValue, value);
        strcpy(varMap[j].varKey, name);
    }
}
/* End of the in-shell variable storage methods */

/* Start of the background process storage methods */
struct procStatus {
    int proId;          // The all mighty process id
    int onExit;         // Status upon completion of execution
    char status[512];   // Holds a simple status message
}processes[50];

void initPros () {        // Initialize all the processes with placeholder values.
    for(int i =0; i < 50; i++) {
        strcpy(processes[i].status,"");
        processes[i].proId = -1;
        processes[i].onExit = -1;
    }
}

int getProcess (int pid) {  // Gets the index of a certain process based on the process id.
    for(int i = 0; i < 50; i++) {
        if(processes[i].proId == pid) {
            return i;
        }
    }
    return -1;              // Returns -1 if not found (failed to find process).
}

void displayProcesses () {
    for(int j = 0; (j < 50) && (processes[j].proId != -1) ;j++) {
        printf("Process %d: pid=%d status=%s\n", j, processes[j].proId, processes[j].status);
    }
}

void storePros () { // Populates the process array of structs with a current status.
    int * proPtr;

    for(int i = 0; (i < 50); i++) { // Might need to add another condition (processes != -1)
        int status = waitpid(processes[i].proId, proPtr, WNOHANG);
        if(status == 0) {
            continue;
        }

        if(WIFEXITED(*proPtr)) {
            strcpy(processes[i].status, "Child process exited with status");
            processes[i].onExit = (WEXITSTATUS(*proPtr));
        }
        else if(WIFSTOPPED(*proPtr)) {
            strcpy(processes[i].status, "Child process has stopped for some reason");
        }
        else if(WIFSIGNALED(*proPtr)) {
            strcpy(processes[i].status, "Child process received signal that was not handled (Terminated)");
        }
        else {
            strcpy(processes[i].status, "Child process has become sentient and rebeled.");
        }
    }
}

void newProcess(char* status, int id) {
    int index = 0;
    while(processes[index].proId != -1) { // Incrementing index until we hit fresh space.
        index++;
    }
    strcpy(processes[index].status, status);
    processes[index].proId = id;
}
/* End of the background process methods */

/* Start of command line input and parsing methods */
void shellGreet() { // Simply clears screen and displays authors.
    clean();
    printf("***************Novel Shell****************\n");
    printf("* Authors: Tanner Palin and Will Shapiro *\n");
    printf("******************************************\n");
}

char* removeWhiteSpace(char *instructionString) { // Makes command line easier to parse.
    printf("this is the user input: %s\n", instructionString);
    int argCount = 0;

    while(isspace(*instructionString)) {    // Destroying leading whitespaces in command line.
        instructionString++;
    }
    instructionString[strlen(instructionString) - 1] = ' '; // Destroying the /n at the end.

    return instructionString; 

}

char** allocateArgs (char ** args) { // Allocating and initializing space for cmd line args.
    for(int i=0; i < 256; i++) {
        args[i] = malloc(sizeof(char) * 256);
    }
}

char * scanInput(char *prompt) {
    char *userInput;
    userInput =  malloc(sizeof(userInput)*(256));
    
    printf("%s ", prompt); // Displays prompt variable
    
    // TODO: Scan user input in, do not need to parse yet (Another function for parsing?).
    fgets(userInput, 256, stdin);
    
    return userInput;
}

void checkTokens(char ** args, int numArgs) { // Checking to see if token = variable, replacing it if so.
    int varIndex = 0;
    char * varName;
    int varSize = 0;
    for(int i = 0; i < numArgs; i++) { // LIMITATION: Cannot use variable in quotes unless it is first item.
        varSize = strlen(args[i]);
        if(args[i][0] == '$') {
            varName = (char*)args[i] + 1;
            varIndex = getVarIndex(varName);
            if(varIndex == -1) {
                printf("ERROR: variable $%s does not exist.\n", varName);
            }
            else {
                strcpy(args[i], getVarVal(varName));
            }
        }
    }
}

int parseInput(char* cmdLine, char** args) {
    int argNum = 0;
    int isBG = 0;
    allocateArgs(args);

    char * delimiter;
    int argSize;
    while((delimiter = strchr(cmdLine, ' '))) { // While delimiter can be set to a space.
        if(cmdLine[0] == '"') {     // If we are receiving a quoted token.
            cmdLine++;
            delimiter = strchr(cmdLine, '"'); // Goes to end of quote.
        }
        argSize = delimiter - cmdLine;
        strncpy(args[argNum], cmdLine, argSize);
        argNum++;
        *delimiter = NULL;          // Seq fault protection.
        cmdLine = delimiter + 1;
        while((*cmdLine == ' ') && *cmdLine) { cmdLine++;} // Skip more whitespace.
    }
    args[argNum] = NULL; // End args with a NULL "argument".
    checkTokens(args, argNum);
    for(int i = 0; args[i] != NULL; i++) {
        if(strcmp(args[i], "<bg>") == 0) {
            isBG = 1;
            printf("background process detected!\n");
        }
    }
    return isBG;
}
/* End of command line input and parsing methods */

/* Start of command execution methods */
void executeLine (char * userInput) {
    char commandLine[256];      // Holds the command line
    char * arguments[256];      // Holds the tokens from the command line
    pid_t proID;                // Behold the mighty process ID.

    strcpy(commandLine, removeWhiteSpace(userInput));
    
    int proType = parseInput(commandLine, arguments); // Decides if <bg> was used and parses.
    if(arguments[0] == NULL) {
        return;
    }

}

/* End of command execution methods */
int main() {
    int quit = 0;
    char * inputToConsole;
    // TODO: Initialize variables, buffers, and make function calls.
    varMapInit();
    initPros();
    shellGreet();
    
    while(!quit) {

        inputToConsole = scanInput(varMap[0].varValue);
        if(strncmp(inputToConsole, "bye",3) == 0){
            quit = 1;
        }
        executeLine(inputToConsole);
    }
    return 0;
}
