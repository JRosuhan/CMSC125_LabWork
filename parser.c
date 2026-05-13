#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysh.h"

void parse_command(char *input, Command *cmd) {
    cmd->command = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = false;
    cmd->background = false;

    for (int i = 0; i < MAX_ARGS; i++) {
        cmd->args[i] = NULL;
    }

    int arg_index = 0;

    char *token = strtok(input, " \t\n");

    while (token != NULL) {

        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                cmd->input_file = strdup(token);
            }
        }
        else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                cmd->output_file = strdup(token);
                cmd->append = false;
            }
        }
        else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                cmd->output_file = strdup(token);
                cmd->append = true;
            }
        }
        else if (strcmp(token, "&") == 0) {
            cmd->background = true;
        }
        else {
            cmd->args[arg_index] = strdup(token);

            if (cmd->command == NULL) {
                cmd->command = cmd->args[arg_index];
            }

            arg_index++;
        }

        token = strtok(NULL, " \t\n");
    }

    cmd->args[arg_index] = NULL;
}

void free_command(Command *cmd) {
    for (int i = 0; cmd->args[i] != NULL; i++) {
        free(cmd->args[i]);
    }

    if (cmd->input_file != NULL) {
        free(cmd->input_file);
    }

    if (cmd->output_file != NULL) {
        free(cmd->output_file);
    }
}