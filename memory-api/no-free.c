#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    int32_t *x = (int32_t *) malloc(sizeof(int32_t));

    *x = 100;

    printf("%p has value %d\n", x, *x);

    return 0;
}
