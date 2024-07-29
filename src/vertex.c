#include "vertex.h"
#include <assert.h>
#include <string.h>

void vertex_buffer_push(VertexBuffer* buf, Vertex* vertex) {
    assert(buf->count < VERTEX_BUFFER_CAPACITY);
    memcpy(&buf->data[buf->count], vertex, sizeof(*vertex));
    buf->count++;
}

