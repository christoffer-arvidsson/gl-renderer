#ifndef ARENA_H_
#define ARENA_H_

#include <stdlib.h>

typedef struct {
    size_t capacity;
    size_t size;
    char *data;
} Region;

Region region_alloc_alloc(size_t capacity);
void* region_alloc(Region* r, size_t size);
void region_reset(Region* r);
void region_rollback(Region* r, size_t size);


#endif  // ARENA_H 
