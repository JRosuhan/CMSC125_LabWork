#ifndef MYSH_H
#define MYSH_H

#include <stdbool.h>
#include <sys/types.h>

// Limits for input size, arguments, and background jobs
#define MAX_ARGS 256
#define MAX_INPUT 1024
#define MAX_JOBS 100

// Stores a parsed user command
// Includes command name, arguments, and redirection/background info
typedef struct {
    char *command;              // main command (e.g., "ls")
    char *args[MAX_ARGS];      // arguments for execvp()
    char *input_file;          // input redirection (<)
    char *output_file;         // output redirection (>, >>)
    bool append;               // append mode for >>
    bool background;           // background execution (&)
} Command;

// Stores background job information
typedef struct {
    int job_id;                // shell-assigned job ID
    pid_t pid;                 // process ID
    char command[MAX_INPUT];   // original command string
} BackgroundJob;

// Parse raw input into structured command
void parse_command(char *input, Command *cmd);

// Execute command (built-in or external)
void execute_command(Command *cmd, char *original_input);

// Free allocated memory in command structure
void free_command(Command *cmd);

// Check and clean finished background jobs
void cleanup_background_jobs();

// Handle built-in commands (cd, pwd, exit)
int handle_builtin(Command *cmd);

#endif