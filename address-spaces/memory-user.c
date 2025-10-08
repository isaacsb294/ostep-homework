#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Uint32Array {
    uint32_t *items;
    uint32_t length;
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage %s <memory in mb> <loops>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uint32_t megabytes = atoi(argv[1]);
    uint32_t interations = atoi(argv[2]);
    uint64_t bytes = megabytes * 1024 * 1024;

    struct Uint32Array array = {
        .items = malloc(bytes),
        .length = (uint32_t) (bytes / sizeof(uint32_t))
    };

    if (array.items == NULL) {
        fprintf(stderr, "Malloc failure!\n");
        exit(EXIT_FAILURE);
    }

    printf("Starting with %d megabytes set...\n", megabytes);

    for (int i; i < interations; i++) {
        for (int j; j < array.length; j++) {
            array.items[j] += 1;
            printf("%d", array.items[j]);
        }
    }

    printf("Finished\n");
    free(array.items);

    return 0;
}
