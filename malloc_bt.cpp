#include "malloc.h"

#ifdef __STDC_LIB_EXT1__
#error STDC_LIB_EXT1 is required!
#endif
#define __STDC_WANT_LIB_EXT1__ 1

#include <cstdio>
#include <cassert>
#include <cstring>

extern "C" void *__libc_malloc(size_t size);
extern "C" void *__libc_free(void *ptr);
extern "C" void *__libc_realloc(void *ptr, size_t new_size);

extern "C" void *malloc(size_t size) {
	printf("malloc size: %zu\n", size);
	const size_t new_size = size + sizeof(size_t);
	char *temp = static_cast<char *>(__libc_malloc(new_size));
	*reinterpret_cast<size_t *>(temp) = size;
	return static_cast<void *>(temp + sizeof(size_t));
}

extern "C" void free(void *ptr) {
	if (ptr == 0) return;

	char *ptr_ch = static_cast<char *>(ptr);

	size_t *size_ptr = reinterpret_cast<size_t *>(ptr_ch - sizeof(size_t));

	assert(0 = memset_s(ptr, *size_ptr, 0, *size_ptr));

	*size_ptr = 0;
	void *real_ptr = static_cast<void *>(ptr_ch - sizeof(size_t));
	__libc_free(real_ptr);
}

extern "C" void *realloc(void *ptr, size_t new_size) {
	printf("realloc %p - %zu\n", ptr, new_size);
	if (new_size == 0) return nullptr;
	if (ptr == NULL) return malloc(new_size);

	void *temp = malloc(new_size);

	size_t old_size = *(size_t *)(static_cast<char *>(ptr) - sizeof(size_t));
	size_t copy_size = (old_size < new_size) ? old_size : new_size;
	std::memcpy(temp, ptr, copy_size);

	free(ptr);
	return temp;
}

extern "C" void *calloc(size_t num, size_t size) {
	printf("calloc %zu - %zu\n", num, size);
	void *ptr = malloc(num * size);
	std::memset(ptr, 0, num * size);
	return ptr;
}
