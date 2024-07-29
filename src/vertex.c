#include "vertex.h"
#include "arena.h"
#include <assert.h>
#include <string.h>

VertexBuffer vertex_buffer_alloc(Region* allocator, size_t capacity) {
    void* data = region_alloc(allocator, sizeof(Vertex) * capacity);
    VertexBuffer vb = {
        .data = (Vertex*)data,
        .count = 0U,
        .capacity = capacity
    };
    return vb;
}

void vertex_buffer_push(VertexBuffer* buf, Vertex* vertex) {
    assert(buf->count < buf->capacity);
    memcpy(&buf->data[buf->count], vertex, sizeof(*vertex));
    buf->count++;
}

