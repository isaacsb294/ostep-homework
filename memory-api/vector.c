#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

struct Int32Vector {
    uint32_t length;
    int32_t *items;
};

struct Int32Vector i32_vector_create() {
    return (struct Int32Vector) {
        .length = 0,
        .items = (int32_t *) malloc(sizeof(int32_t))
    };
}

void i32_vector_add(struct Int32Vector *vector, int32_t value) {
    if (vector->length == 0) {
        vector->items[0] = value;
        ++vector->length;
        return;
    }

    ++vector->length;

    int32_t *new_alloc = realloc(
        vector->items, 
        sizeof(int32_t) * vector->length);

    if (new_alloc) {
        vector->items = new_alloc;
        vector->items[vector->length - 1] = value;
    }
}

int main(int argc, char *argv[]) {
    struct Int32Vector vector = i32_vector_create();

    i32_vector_add(&vector, 10);
    i32_vector_add(&vector, 678);
    i32_vector_add(&vector, 4209);

    for (uint32_t i = 0; i < 3; i++) {
        printf("Item #%d in the array has value %d.\n", 
            (i + 1), vector.items[i]);
    }

    printf("The total length of the vector is %d.\n", vector.length);

    free(vector.items);

    return 0;
}
