#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    int32_t *array = (int32_t *) malloc(sizeof(int32_t) * 10);

    free(array);

    printf("%d\n", array[9]);
    return 0;
}
