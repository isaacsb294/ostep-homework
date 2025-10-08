#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

volatile int child_finished = 0;

void sigchld_handler(const int sig) {
    puts("Child process finished\n");
    child_finished = 1;
}

int main(const int argc, char* argv[]) {
    signal(SIGCHLD, sigchld_handler);

    puts("Forking...");
    const int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Failed to fork.\n");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        puts("Hello\n");
        exit(EXIT_SUCCESS);
    } else {
        while (!child_finished) {};
        puts("Goodbye\n");
    }

    return 0;    
}
