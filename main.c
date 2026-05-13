#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysh.h"

int main() {

    char input[MAX_INPUT];

    while (1) {

        cleanup_background_jobs();

        printf("mysh> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        if (strlen(input) == 1) {
            continue;
        }

        char original_input[MAX_INPUT];
        strcpy(original_input, input);

        original_input[strcspn(original_input, "\n")] = '\0';

        Command cmd;
        parse_command(input, &cmd);

        execute_command(&cmd, original_input);

        free_command(&cmd);
    }

    return 0;
}