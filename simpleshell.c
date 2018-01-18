#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    char instruction[1000];
    while (1) {
        printf("SimpleShell > ");
        //get input from user and store in instruction
        fgets(instruction, 1000, stdin);
        //remove '\n' from passphrase becuase fgets includes '\n'
        if (strlen(instruction) > 0 && instruction[strlen(instruction) - 1] == '\n') {
            instruction[strlen(instruction) - 1] = '\0';
        }

        if (strlen(instruction) <= 0) {
            printf("Invalid command\n");
        }

        pid_t pid;
        if ((pid = fork()) < 0) {
            perror("fork failure");
            exit(1);
        }
        else if (pid == 0) {
            //child
            
        }
        else {
            //parent
        }
    }   
    return 0;
}