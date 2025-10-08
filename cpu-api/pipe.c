#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        fprintf(stderr, "Failed to create pipe\n");
        exit(EXIT_FAILURE);
    }

    const int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Failed to fork\n");
        exit(EXIT_FAILURE);
    }

    if (rc == 0) {
        close(pipefd[1]);

        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        char input[100];

        while (fgets(input, sizeof(input), stdin) != NULL) {
            printf("Child recieved: %s", input);
        }

        exit(EXIT_SUCCESS);
    } else {
        close(pipefd[0]);

        const char *msg = "Hello from parent!\n";
        write(pipefd[1], msg, strlen(msg));
        close(pipefd[1]);

        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}

