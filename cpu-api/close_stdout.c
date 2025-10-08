#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    const int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Failed to fork\n");
        exit(EXIT_FAILURE);
    }

    if (rc == 0) {
        close(STDOUT_FILENO);
        printf("Child process PID %d\n", (int) getpid());
        exit(EXIT_SUCCESS);
    } else {
        printf("Parent of %d PID %d\n", rc, (int) getpid());
    }

    return 0;
}
