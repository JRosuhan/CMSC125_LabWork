#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "mysh.h"

BackgroundJob jobs[MAX_JOBS];
int job_count = 0;
int next_job_id = 1;

int handle_builtin(Command *cmd) {
    if (cmd->command == NULL) {
        return -1;
    }

    if (strcmp(cmd->command, "exit") == 0) {
        cleanup_background_jobs();
        exit(0);
    }
    else if (strcmp(cmd->command, "cd") == 0) {
        char *dir = cmd->args[1];
        if (dir == NULL) {
            dir = getenv("HOME");
            if (dir == NULL) {
                fprintf(stderr, "mysh: cd: HOME not set\n");
                return 0;
            }
        }
        if (chdir(dir) != 0) {
            perror("mysh: cd");
        }
        return 0;
    }
    else if (strcmp(cmd->command, "pwd") == 0) {
        char cwd[MAX_INPUT];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        }
        else {
            perror("mysh: pwd");
        }
        return 0;
    }

    return -1;
}

void execute_command(Command *cmd, char *original_input) {
    if (cmd->command == NULL) {
        return;
    }

    if (handle_builtin(cmd) == 0) {
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        if (cmd->input_file != NULL) {
            int fd = open(cmd->input_file, O_RDONLY);
            if (fd < 0) {
                perror("open input file failed");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (cmd->output_file != NULL) {
            int flags = O_WRONLY | O_CREAT;
            if (cmd->append) {
                flags |= O_APPEND;
            }
            else {
                flags |= O_TRUNC;
            }

            int fd = open(cmd->output_file, flags, 0644);
            if (fd < 0) {
                perror("open output file failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cmd->command, cmd->args);

        fprintf(stderr, "mysh: command not found: %s\n", cmd->command);
        exit(127);
    }
    else {
        if (cmd->background) {
            jobs[job_count].job_id = next_job_id;
            jobs[job_count].pid = pid;
            strcpy(jobs[job_count].command, original_input);

            printf("[%d] Started background job: %s (PID: %d)\n",
                   next_job_id,
                   original_input,
                   pid);

            job_count++;
            next_job_id++;
        }
        else {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code != 0) {
                    printf("Command exited with code %d\n", exit_code);
                }
            }
        }
    }
}

void cleanup_background_jobs() {

    int status;

    for (int i = 0; i < job_count; i++) {

        pid_t result = waitpid(jobs[i].pid, &status, WNOHANG);

        if (result > 0) {

            printf("\n[%d] Completed: %s (PID: %d)\n",
                   jobs[i].job_id,
                   jobs[i].command,
                   jobs[i].pid);

            for (int j = i; j < job_count - 1; j++) {
                jobs[j] = jobs[j + 1];
            }

            job_count--;
            i--;
        }
    }
}
