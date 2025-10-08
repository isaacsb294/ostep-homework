#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>

double get_time_in_microseconds(
    struct timeval start, 
    struct timeval end) {
    return (double) (end.tv_sec * 1000000 + end.tv_usec - 
    start.tv_sec * 1000000 - start.tv_usec);
}

void measure_syscall(const int num_loops) {
    int fd = open("./n.txt", O_CREAT | O_WRONLY, 
                  S_IRWXU);

    struct timeval start;
    struct timeval end;
    
    gettimeofday(&start, NULL);

    for (int i = 0; i < num_loops; i++) {
        write(fd, NULL, 0);
    }

    gettimeofday(&end, NULL);

    printf("Average write system call time taken: %f microseconds\n", 
           (double) (get_time_in_microseconds(start, end) / num_loops));

    close(fd);
}

void measure_context_switch(const int num_loops) {
    int pipefd[2];
    int pipefd2[2];

    if (pipe(pipefd) < 0) {
        fprintf(stderr, "Pipe failed.\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd2) < 0) {
        fprintf(stderr, "Pipe failed.\n");
        exit(EXIT_FAILURE);
    }

    const int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Fork failed.\n");
        exit(EXIT_FAILURE);
    }

    if (rc == 0) {
        char buffer = '0';
        close(pipefd[1]);
        close(pipefd2[0]);

        for (int i = 0; i < num_loops; i++) {
            if (read(pipefd[0], &buffer, 1) < 0) {
                fprintf(stderr, "Child process could not read from parent stdout\n");
                exit(EXIT_FAILURE);
            }

            if (write(pipefd2[1], &buffer, 1) < 0) {
                fprintf(stderr, "Child process could not write to own stdin\n");
                exit(EXIT_FAILURE);
            }
        }

        close(pipefd[0]);
        close(pipefd2[1]);
        exit(EXIT_SUCCESS);
    } else {
        struct timeval start;
        struct timeval end;

        gettimeofday(&start, NULL);
        char buffer = '0';

        close(pipefd[0]);
        close(pipefd2[1]);

        for (int i = 0; i < num_loops; i++) {
            if (write(pipefd[1], &buffer, 1) < 0) {
                fprintf(stderr, "Parent process failed to write to stdin.\n");
                exit(EXIT_FAILURE);
            }

            if (read(pipefd2[0], &buffer, 1) < 0) {
                fprintf(stderr, "Parent process failed to read from child stdout.\n");
                exit(EXIT_FAILURE);
            }
        }

        gettimeofday(&end, NULL);

        printf("Context switch time taken: %f microseconds\n",
               (double) (get_time_in_microseconds(start, end) / num_loops));
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number of times to loop>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const int num_loops = atoi(argv[1]);

    cpu_set_t cpu_set;
    int cpu = sched_getcpu();

    CPU_ZERO(&cpu_set);
    CPU_SET(cpu, &cpu_set);

    int rc = sched_setaffinity(getpid(), sizeof(cpu_set), &cpu_set);

    if (rc < 0) {
        fprintf(stderr, "Could not set cpu affinity mask\n");
        exit(EXIT_FAILURE);
    }

    measure_syscall(num_loops);
    measure_context_switch(num_loops);

    return 0;
}
