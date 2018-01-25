#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char** argv) {
    char instruction[1000];
    struct rusage r;
    long previous_sec = 0;
    long previous_usec = 0;
    long previous_switch = 0;
    while (1) {
        printf("[SimpleShell] > ");
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
            
            if (getrusage(RUSAGE_CHILDREN, &r) < 0) {
                printf("could not get stats on process\n");
            }else {
                printf("\nuser cpu time:\t%ld seconds, %ld microseconds\n", r.ru_utime.tv_sec - previous_sec, r.ru_utime.tv_usec - previous_usec);
                printf("involuntary context switches:\t%ld\n\n", r.ru_nivcsw - previous_switch);
                previous_sec = r.ru_utime.tv_sec;
                previous_usec = r.ru_utime.tv_usec;
                previous_switch = r.ru_nivcsw;
            }
        }
    }
    return 0;
}