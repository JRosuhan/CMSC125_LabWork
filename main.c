#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysh.h"

int main() {

    // Buffer for user input
    char input[MAX_INPUT];

    while (1) {

        // Check and clean finished background jobs before each prompt
        cleanup_background_jobs();

        // Display shell prompt
        printf("mysh> ");
        fflush(stdout);

        // Read user input
        // If Ctrl+D is pressed, exit shell loop
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        // Ignore empty input (just Enter key)
        if (strlen(input) == 1) {
            continue;
        }

        // Copy original input for logging/background display
        char original_input[MAX_INPUT];
        strcpy(original_input, input);

        // Remove trailing newline from copied input
        original_input[strcspn(original_input, "\n")] = '\0';

        // Parse raw input into structured command
        Command cmd;
        parse_command(input, &cmd);

        // Execute parsed command
        execute_command(&cmd, original_input);

        // Free dynamically allocated memory
        free_command(&cmd);
    }

    return 0;
}