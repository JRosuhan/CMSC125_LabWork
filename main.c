#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#define MAX_LINE 1024

int main(void) {
    char line[MAX_LINE];
    char cwd[1024];

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (fgets(line, MAX_LINE, stdin) == NULL) {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            continue; 
        }

        if (strcmp(line, "exit") == 0) {
            break;
        }

    
        if (strcmp(line, "pwd") == 0) {
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("getcwd");
            }
            continue;
        }

        if (strncmp(line, "cd", 2) == 0) {
            char *dir = line + 2;
            while (*dir == ' ') dir++; 

            if (strlen(dir) == 0) {
               
#ifdef _WIN32
                dir = getenv("USERPROFILE"); 
#else
                dir = getenv("HOME");       
#endif
                if (dir == NULL) {
                    fprintf(stderr, "cd: Home directory not found\n");
                    continue;
                }
            }

            if (chdir(dir) != 0) {
                perror("chdir");
            }
            continue;
        }

        printf("You entered: %s\n", line);
    }

    return 0;
}
