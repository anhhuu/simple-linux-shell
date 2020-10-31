#include <unistd.h>
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/wait.h> 

#include "common.h"
#include "execution.h"


void execWithArgs(char** argv) 
{ 
    // Forking a child 
    pid_t pid = fork();  
  
    if (pid == -1) 
    {
    	//error
        printf("\nFailed forking child..\n"); 
        return; 
    } else if (pid == 0) 
    {
        //"ls" "-l"
    	//child
        if (execvp(argv[0], argv) < 0) 
        { 
            printf("\nCould not execute command..\n"); 
        } 
        exit(0);
    } else 
    {
    	//parrent
        wait(NULL);
        return; 
    } 
}

void execWithArgsPiped(char** argvPipe1, char** argvPipe2) 
{ 
    int pipefd[2];  
    pid_t p1, p2; 
  
    if (pipe(pipefd) < 0) 
    { 
        printf("\nPipe could not be initialized"); 
        return; 
    } 
    p1 = fork(); 
    if (p1 < 0) 
    { 
        printf("\nCould not fork"); 
        return; 
    } 
  
    if (p1 == 0) 
    { 
        // Child 1 executing.. 
        // It only needs to write at the write end 
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 
  
        if (execvp(argvPipe1[0], argvPipe1) < 0) 
        { 
            printf("\nCould not execute command 1..\n"); 
            exit(0); 
        }

    } else 
    {   
        // Parent executing 
        p2 = fork(); 
  
        if (p2 < 0) { 
            printf("\nCould not fork\n"); 
            return; 
        } 
  
        // Child 2 executing.. 
        // It only needs to read at the read end 
        if (p2 == 0) { 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO); 
            close(pipefd[0]); 
            printf("\n");
            if (execvp(argvPipe2[0], argvPipe2) < 0) 
            { 
                printf("\nCould not execute command 2..\n"); 
                exit(0); 
            } 
            printf("\n");
        } else { 
            // parent executing, waiting for two children 
            //wait(p2); 
            wait(NULL);
        }
    } 
} 

void execWithArgsAndRedirecting(char** argv, char* fileName, int stream)
{
    // Forking a child 
    pid_t pid = fork();  
              
    if (pid == -1) 
    {
        //error
        printf("\nFailed forking child..\n"); 
        return; 
    }
    else if (pid == 0) 
    {      
        FILE* f = NULL;
        if(stream == REDIRECTING_OUTPUT_EXEC)
        {
            f = fopen(fileName, "w");
            dup2(fileno(f), STDOUT_FILENO);
        } else if(stream == REDIRECTING_INPUT_EXEC)
        {
            f = fopen(fileName, "r");
            dup2(fileno(f), STDIN_FILENO);
        }
        close(f);
         //child
        if (execvp(argv[0], argv) < 0) 
        { 
            printf("\nCould not execute command..\n"); 
        } 
        exit(0);
    } else 
    {
        //parrent
        wait(NULL);
    } 
}