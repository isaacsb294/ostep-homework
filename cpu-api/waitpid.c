#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    const int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(EXIT_FAILURE);
    }

    if (rc == 0) {
        printf("Child process pid %d\n", (int) getpid());
        exit(EXIT_SUCCESS);
    } else {
        int wstatus = 0;
        const int waitpid_rc = waitpid(rc, &wstatus, 0);
        printf("Parent process (waitpid_rc: %d) pid %d\n", waitpid_rc, (int) getpid());
    }

    return 0;
}
