#pragma once

#include <cstddef>

extern "C" {
void *__libc_malloc(size_t size);
void *__libc_free(void *ptr);
void *__libc_realloc(void *ptr, size_t new_size);
void *__libc_calloc(size_t num, size_t size);
}
