#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//#define MAX_COMMANDS 100
#define MAX_ARG_LENGTH 100

void execute_external_command(char *input_string) {
    int pipe_fds[2 * (MAX_COMMANDS - 1)]; // Array to hold pipe file descriptors
    int num_pipes = 0; // Count of pipes
    char *commands[MAX_COMMANDS]; // To hold individual commands

    // Split the input string into commands based on the pipe symbol '|'
    char *command = strtok(input_string, "|");
    while (command != NULL && num_pipes < MAX_COMMANDS - 1) {
        commands[num_pipes++] = command; // Store each command
        command = strtok(NULL, "|");
    }
    commands[num_pipes] = NULL; // Null-terminate the commands array

    for (int i = 0; i < num_pipes - 1; i++) {
        // Create a pipe
        if (pipe(pipe_fds + i * 2) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_pipes; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            char *args[MAX_COMMANDS];
            int j = 0;
            char *token = strtok(commands[i], " "); // Tokenize the command by spaces

            while (token != NULL && j < MAX_COMMANDS - 1) {
                args[j++] = token; // Store each token in the arguments array
                token = strtok(NULL, " ");
            }
            args[j] = NULL; // Null-terminate the arguments array

            // If not the last command, redirect output to the next pipe
            if (i < num_pipes - 1) {
                dup2(pipe_fds[i * 2 + 1], STDOUT_FILENO);
            }
            // If not the first command, redirect input from the previous pipe
            if (i > 0) {
                dup2(pipe_fds[(i - 1) * 2], STDIN_FILENO);
            }

            // Close all pipe file descriptors in the child process
            for (int k = 0; k < num_pipes - 1; k++) {
                close(pipe_fds[k * 2]);
                close(pipe_fds[k * 2 + 1]);
            }

            // Execute the command
            if (execvp(args[0], args) == -1) {
                perror("Execution failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Parent process closes all pipe file descriptors
    for (int i = 0; i < num_pipes - 1; i++) {
        close(pipe_fds[i * 2]);
        close(pipe_fds[i * 2 + 1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_pipes; i++) {
        int status;
        wait(&status);
    }
}

