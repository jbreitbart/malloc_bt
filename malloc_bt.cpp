#include "malloc.h"

#include <cstdio>

extern "C" void *__libc_malloc(size_t size);
extern "C" void *__libc_free(void *ptr);

extern "C" void *malloc(size_t size) {
	printf("malloc size: %zu\n", size);
	const size_t new_size = size + sizeof(size_t);
	char *temp = (char *)__libc_malloc(new_size);
	*((size_t *)temp) = size;
	return (void *)(temp + sizeof(size_t));
}

extern "C" void free(void *ptr) {
	if (ptr == 0) return;

	char *n = (char *)ptr;
	void *real_ptr = (void *)((char *)ptr - sizeof(size_t));
	// get the size of the memory freed

	size_t *size_ptr = (size_t *)(n - sizeof(size_t));
// write zeros
#pragma simd
	for (size_t i = 0; i < *(size_ptr); ++i) {
		n[i] = 0;
	}

	*size_ptr = 0;
	// call _free()
	__libc_free(real_ptr);
}
