#ifndef MYSH_H
#define MYSH_H

#include <stdbool.h>
#include <sys/types.h>

#define MAX_ARGS 256
#define MAX_INPUT 1024
#define MAX_JOBS 100

typedef struct {
    char *command;
    char *args[MAX_ARGS];
    char *input_file;
    char *output_file;
    bool append;
    bool background;
} Command;

typedef struct {
    int job_id;
    pid_t pid;
    char command[MAX_INPUT];
} BackgroundJob;

void parse_command(char *input, Command *cmd);
void execute_command(Command *cmd, char *original_input);
void free_command(Command *cmd);
void cleanup_background_jobs();
int handle_builtin(Command *cmd);

#endif