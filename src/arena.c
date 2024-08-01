#include "arena.h"
#include <assert.h>
#include <stdlib.h>

Region region_alloc_alloc(size_t capacity) {
    void *data = malloc(capacity);
    assert(data != NULL);

    Region r = {
        .capacity = capacity,
        .data = data,
        .size = 0U
    };
    return r;
}

void region_alloc_free(Region* r) {
    free(r->data);
    r->size = 0U;
    r->capacity = 0U;
}

void* region_alloc(Region* r, size_t size) {
    assert(r->size+size <= r->capacity);
    void* res = &r->data[r->size];
    r->size += size;
    return res;
}

void region_reset(Region* r) {
    r->size = 0;
}

void region_rollback(Region* r, size_t size) {
    assert((int)r->size - (int)size >= 0);
    r->size -= size;
}

