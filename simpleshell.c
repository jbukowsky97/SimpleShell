#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    char instruction[1000];
    while (1) {
        printf("SIMPLESHELL > ");
        //get input from user and store in instruction
        fgets(instruction, 1000, stdin);
        //remove '\n' from passphrase becuase fgets includes '\n'
        if (strlen(instruction) > 0 && instruction[strlen(instruction) - 1] == '\n') {
            instruction[strlen(instruction) - 1] = '\0';
        }

        if (strlen(instruction) <= 0) {
            printf("Invalid command\n");
            continue;
        }

        if (strcmp(instruction, "quit") == 0) {
            break;
        }

        char tempInstruction[strlen(instruction)];
        for (int i = 0; i < strlen(instruction); i++) {
            tempInstruction[i] = instruction[i];
        }

        int numTokens = 0;
        
        char* token = strtok(tempInstruction, " ");

        while(token != NULL) {
            numTokens++;
         
            token = strtok(NULL, " ");
        }

        char* args[numTokens + 1];

        char* command = strtok(instruction, " ");

        args[0] = command;
        for (int i = 1; i < numTokens; i++) {
            args[i] = strtok(NULL, " ");
        }
        args[numTokens] = NULL;

        pid_t pid;
        int status;
        if ((pid = fork()) < 0) {
            perror("fork failure");
            exit(1);
        }
        else if (pid == 0) {
            //child
            if (execvp(command, args) < 0) {
                perror("exec failed");
                exit(1);
            }else {
                exit(0);
            }
        }
        else {
            //parent
            pid = wait(&status);
        }
    }   
    return 0;
}