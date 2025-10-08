#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(const int argc, char* argv[]) {
    const int fd = open("output.txt", O_CREAT|O_WRONLY|O_TRUNC, 
                    S_IRWXU);

    assert(fd > -1);

    puts("Forking...\n");
    const int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Fork failed.\n");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        printf("Writing to output from child (pid %d)\n", (int) getpid());
        write(fd, "Hello from the child process\n", 30);
        puts("Exiting child process...\n");
        exit(EXIT_SUCCESS);
    } else {
        // Without this wait, the parent may or may not write first
        // adding the wait makes the behaviour deterministic
        const int wait_rc = wait(NULL);
        printf("Writing to output from parent (wait_rc %d) (pid %d)\n", 
               wait_rc, (int) getpid());
        write(fd, "Hello from the parent process\n", 31);
    }

    return 0;
}
