#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(const int argc, char* argv[])
{
    int x = 100;

    printf("x: %d (pid:%d)\n", x, (int) getpid());
    puts("Forking...\n");

    const int child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "fork failed.\n");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        printf("x: %d (child pid: %d)\n", x, (int) getpid());
        x = 800;
        printf("New x: %d, (child pid:%d)\n", x, (int) getpid());
        puts("Exiting child process\n");
        exit(EXIT_SUCCESS);
    } else {
        const int rc_wait = wait(NULL);
        printf("x: %d (rc_wait: %d) (parent of %d) (pid %d)\n", 
               x, rc_wait, child_pid, (int) getpid());
    }

    return 0;
}
