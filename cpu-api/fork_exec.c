#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    puts("Forking...");
    const int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Fork failed.\n");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        puts("Child process executing ls with execl\n");
        execl("/bin/ls",
              "ls", "-s", "-l", "-h", (char*) NULL);
        exit(EXIT_SUCCESS);
    } else {
        const int wait_rc = wait(NULL);
        printf("Parent process executing ls with execvp (wait_rc: %d) (pid: %d)\n", 
               wait_rc, (int) getpid());
        char *const ls_argv[5] = { "ls", "-s", "-l", "-h", (char*) NULL };
        execvp(ls_argv[0], ls_argv);
    }

    return 0;
}
