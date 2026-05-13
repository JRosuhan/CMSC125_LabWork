#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysh.h"

void parse_command(char *input, Command *cmd) {

    // Initialize all command fields to default values
    // before parsing a new user command
    cmd->command = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = false;
    cmd->background = false;

    // Initialize all argument slots to NULL
    // so execvp() knows where the argument list ends
    for (int i = 0; i < MAX_ARGS; i++) {
        cmd->args[i] = NULL;
    }

    int arg_index = 0;

    // Tokenize user input using spaces, tabs, and newline
    // Example:
    // "ls -l > out.txt"
    // becomes: ["ls", "-l", ">", "out.txt"]
    char *token = strtok(input, " \t\n");

    while (token != NULL) {

        // Handle input redirection
        // Example: wc -l < input.txt
        // Stores "input.txt" inside cmd->input_file
        if (strcmp(token, "<") == 0) {

            token = strtok(NULL, " \t\n");

            if (token != NULL) {
                cmd->input_file = strdup(token);
            }
        }

        // Handle output redirection overwrite
        // Example: echo hello > out.txt
        // Output file is replaced each run
        else if (strcmp(token, ">") == 0) {

            token = strtok(NULL, " \t\n");

            if (token != NULL) {
                cmd->output_file = strdup(token);
                cmd->append = false;
            }
        }

        // Handle append redirection
        // Example: echo hello >> out.txt
        // Output is appended instead of overwritten
        else if (strcmp(token, ">>") == 0) {

            token = strtok(NULL, " \t\n");

            if (token != NULL) {
                cmd->output_file = strdup(token);
                cmd->append = true;
            }
        }

        // Detect background execution symbol
        // Example: sleep 5 &
        // Command will run without blocking the shell
        else if (strcmp(token, "&") == 0) {
            cmd->background = true;
        }

        // Handle normal commands and arguments
        // Example:
        // ls -l /home
        // args[0] = "ls"
        // args[1] = "-l"
        // args[2] = "/home"
        else {

            cmd->args[arg_index] = strdup(token);

            // First token is treated as the command
            // Example: "ls"
            if (cmd->command == NULL) {
                cmd->command = cmd->args[arg_index];
            }

            arg_index++;
        }

        // Move to next token
        token = strtok(NULL, " \t\n");
    }

    // Null-terminate argument list
    // Required by execvp() to know where arguments end
    cmd->args[arg_index] = NULL;
}

void free_command(Command *cmd) {

    // Free dynamically allocated memory for arguments
    // Prevents memory leaks after each command execution
    for (int i = 0; cmd->args[i] != NULL; i++) {
        free(cmd->args[i]);
    }

    // Free memory allocated for input redirection file
    if (cmd->input_file != NULL) {
        free(cmd->input_file);
    }

    // Free memory allocated for output redirection file
    if (cmd->output_file != NULL) {
        free(cmd->output_file);
    }
}