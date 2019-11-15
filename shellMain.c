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
    strcpy(varMap[0].varValue,"novsh >");     // varMap[0] will be used to hold the prompt prefix.
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

char* removeWhiteSpace(char *instructionString) {   // Function makes command line input easier to parse.
    int argCount = 0;

    while(isspace(*instructionString)) {    // Destroying leading whitespaces in command line.
        instructionString++;
    }
    instructionString[strlen(instructionString) - 1] = ' '; // Destroying the /n at the end of input.

    return instructionString; 

}

char** allocateArgs (char ** args) { // Allocating and initializing space for cmd line args.
    for(int i=0; i < 256; i++) {
        args[i] = malloc(sizeof(char) * 256);
    }
}

char * scanInput(char *prompt) {    //Scan user input in, parsing is done in another function.
    char *userInput;
    userInput =  malloc(sizeof(userInput)*(256));
    
    printf("%s ", prompt); // Displays prompt variable
    
    
    fgets(userInput, 256, stdin);
    if(feof(stdin)) {
        printf("\n");
        exit(0);
        }
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
        *delimiter = NULL;          // Seg fault protection.
        cmdLine = delimiter + 1;
        while((*cmdLine == ' ') && *cmdLine) { cmdLine++;} // Skip more whitespace.
    }
    args[argNum] = NULL; // End args with a NULL "argument" so we can use execvp().
    checkTokens(args, argNum);
    for(int i = 0; args[i] != NULL; i++) {
        if(strcmp(args[i], "<bg>") == 0) {
            isBG = 1;
            printf("background process detected!\n");
        }
    }
    return isBG;    // Returns 1 if command is meant to run in background, returns 0 if not.
}
/* End of command line input and parsing methods */

/* Start of command execution methods */
int checkBuiltIns(char** args) {
    if(strcmp(args[0], "!") == 0) {
        return 1;
    }
    else if((args[1] != NULL) && (args[2] != NULL) && (strcmp(args[1], "=") == 0)) {
        addVar(args[0], args[2]);
        return 1;
    }
    else if((strcmp(args[0], "newprompt") == 0) && (args[0] != NULL)) {
        addVar("PROMPT", args[1]);
        return 1;
    }
    else if((strcmp(args[0], "dir") == 0) && (args[1] != NULL)) { // Needs Testing.
        int success = chdir(args[1]);
        if(success == 0) {
            return 1;
        }
        printf("ERROR: Directory not found!\n");
        return 1;
    }
    else if(strcmp(args[0], "listprocs") == 0) {
        displayProcesses();
        return 1;
    }
    return 0; // Return 0 if none of the built-in commands are found.
}


int executeLine (char * userInput) {
    char commandLine[256];      // Holds the command line
    char * arguments[256];      // Holds the tokens from the command line
    char * parameters[256];
    pid_t proID;                // Behold the mighty process ID.
    int* await;                  // Status after waiting for foreground process.

    allocateArgs(parameters);
    
    strcpy(commandLine, removeWhiteSpace(userInput));
    
    int proType = parseInput(commandLine, arguments); // Decides if <bg> was used and parses.
    
    if(arguments[0] == NULL) {
        return 0;                 // No commands in line? Just return all good.
    }

    if(strcmp(arguments[0], "bye") == 0) {
        return -1;
    }
    int builtIn = checkBuiltIns(arguments);

    int q = 0;
    for(int i = 1; (i < 256) && (arguments[i] != NULL); i++) {
        strcpy(parameters[q], arguments[i]);
        q++;
    }
    parameters[q] = 0x0;
    q = 0;

    if(builtIn == 0 && (strcmp(arguments[0], "bye") != 0)) {                      // Not a built-in command, must fork and exec.
        // TODO: fork and exec stuff
        printf("test: %s\n", arguments[0]);
    if((strcmp(arguments[0], "run") == 0)) {
        if((proID = fork()) == 0) { 
            if((execvp(arguments[1], parameters)) == -1) {
                printf("ERROR: Command not found!\n");
                exit(0);
            }
        }

        if(proType == 1) {
            newProcess("Running in background.", proID);
        }
        else if(proType == 0) {                  // If process is to run in the foreground.
            waitpid(proID, await,0);
            if(await == -1) {
                printf("ERROR: Error during child process runtime.\n");
            }
        }
    }
    else if((strcmp(arguments[0], "assignto") == 0) && (arguments[1] != NULL )) {
        for(int j = 2; ( j < 256 ) && (arguments[j] != NULL) ;j++) {
            strcpy(parameters[q], arguments[j]);
            q++;
        }
        parameters[q] = 0x0;
        q = 0;

        int pipefd[2];
        pipe(pipefd);
    
        if((proID = fork()) == 0) { 
            close(pipefd[0]);    // close reading end in the child

            dup2(pipefd[1], 1);  // send stdout to the pipe
            dup2(pipefd[1], 2);  // send stderr to the pipe

            close(pipefd[1]);    // this descriptor is no longer needed
            if((execvp(arguments[2], parameters)) == -1) {
                printf("ERROR: Command not found!\n");
                exit(0);
            }
        }
        else {
            char assignBuf[256];
            close(pipefd[1]);
            while (read(pipefd[0], assignBuf, sizeof(assignBuf)) != 0)
            {
            }
            for(int i = 0; i < sizeof(assignBuf); i++) {
            }
            addVar(arguments[1], assignBuf);
        }
                          
            waitpid(proID, await,0);        // Process is to run in the foreground.
            if(await == -1) {
                printf("ERROR: Error during child process runtime.\n");
            }
        }  
    }
    return 0;
}

/* End of command execution methods */
int main() {
    int quit = 0;
    char * inputToConsole;

    varMapInit();
    initPros();
    shellGreet();
    
    while(quit != -1) {

        inputToConsole = scanInput(varMap[0].varValue);
        if(strncmp(inputToConsole, "bye",3) == 0){
            break;
        }
        executeLine(inputToConsole);
    }
    return 0;
}
