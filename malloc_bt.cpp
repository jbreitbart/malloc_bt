#include "malloc.h"

#ifdef ZERO
#ifdef __STDC_LIB_EXT1__
#error STDC_LIB_EXT1 is required!
#endif
#define __STDC_WANT_LIB_EXT1__ 1
#include <cstring>
#endif

#ifdef RUNTIME
#include <chrono>
#endif

#include <iostream>
#include <cassert>

extern "C" void *__libc_malloc(size_t size);
extern "C" void *__libc_free(void *ptr);
extern "C" void *__libc_realloc(void *ptr, size_t new_size);
extern "C" void *__libc_calloc(size_t num, size_t size);

#ifdef RUNTIME
using msT = std::chrono::milliseconds;
using nsT = std::chrono::nanoseconds;
static nsT malloc_time = nsT(0);
static nsT free_time = nsT(0);
static nsT realloc_time = nsT(0);
static nsT calloc_time = nsT(0);
#endif

static long malloc_counter = 0;
static long free_counter = 0;
static long realloc_counter = 0;
static long calloc_counter = 0;

extern "C" void *malloc(size_t size) {
	++malloc_counter;

#ifdef RUNTIME
	auto begin = std::chrono::high_resolution_clock::now();
#endif

	size_t new_size = size;
#ifdef ZERO
	new_size += sizeof(size_t);
#endif
	char *temp = static_cast<char *>(__libc_malloc(new_size));
#ifdef ZERO
	*reinterpret_cast<size_t *>(temp) = size;
	temp += sizeof(size_t);
#endif

#ifdef RUNTIME
	auto dur = std::chrono::high_resolution_clock::now() - begin;
	malloc_time += std::chrono::duration_cast<nsT>(dur);
#endif

	return static_cast<void *>(temp);
}

extern "C" void free(void *ptr) {
	++free_counter;

	if (ptr == 0) return;

#ifdef RUNTIME
	auto begin = std::chrono::high_resolution_clock::now();
#endif

#ifdef ZERO
	char *ptr_ch = static_cast<char *>(ptr);
	size_t *size_ptr = reinterpret_cast<size_t *>(ptr_ch - sizeof(size_t));

	assert(0 = memset_s(ptr, *size_ptr, 0, *size_ptr));

	*size_ptr = 0;
	void *real_ptr = static_cast<void *>(ptr_ch - sizeof(size_t));
#else
	void *real_ptr = ptr;
#endif
	__libc_free(real_ptr);

#ifdef RUNTIME
	auto dur = std::chrono::high_resolution_clock::now() - begin;
	free_time += std::chrono::duration_cast<nsT>(dur);
#endif
}

extern "C" void *realloc(void *ptr, size_t new_size) {
	++realloc_counter;

	if (new_size == 0) return nullptr;

#ifdef RUNTIME
	auto begin = std::chrono::high_resolution_clock::now();
#endif

#ifdef ZERO
	if (ptr == NULL) {
		void *temp = malloc(new_size);

#ifdef RUNTIME
		auto dur = std::chrono::high_resolution_clock::now() - begin;
		realloc_time += std::chrono::duration_cast<nsT>(dur);
#endif

		return temp;
	}

	void *temp = malloc(new_size);

	size_t old_size = *(size_t *)(static_cast<char *>(ptr) - sizeof(size_t));
	size_t copy_size = (old_size < new_size) ? old_size : new_size;
	std::memcpy(temp, ptr, copy_size);

	free(ptr);
#else
	void *temp = __libc_realloc(ptr, new_size);
#endif

#ifdef RUNTIME
	auto dur = std::chrono::high_resolution_clock::now() - begin;
	realloc_time += std::chrono::duration_cast<nsT>(dur);
#endif

	return temp;
}

extern "C" void *calloc(size_t num, size_t size) {
#ifdef RUNTIME
	auto begin = std::chrono::high_resolution_clock::now();
#endif

#ifdef ZERO
	++calloc_counter;
	void *ptr = malloc(num * size);
	std::memset(ptr, 0, num * size);
#else
	void *ptr = __libc_calloc(num, size);
#endif

#ifdef RUNTIME
	auto dur = std::chrono::high_resolution_clock::now() - begin;
	calloc_time += std::chrono::duration_cast<nsT>(dur);
#endif
	return ptr;
}

__attribute__((destructor)) static void destructor() {
	std::cout << "===" << std::endl;
	std::cout << "Durations measured in ns. Showing ms." << std::endl;

	std::cout << "malloc:  \t" << malloc_counter
#ifdef RUNTIME
			  << "\t" << std::chrono::duration_cast<msT>(malloc_time).count() << " (ms)"
#endif
			  << std::endl;

	std::cout << "free:    \t" << free_counter
#ifdef RUNTIME
			  << "\t" << std::chrono::duration_cast<msT>(free_time).count() << " (ms)"
#endif
			  << std::endl;

	std::cout << "realloc: \t" << realloc_counter
#ifdef RUNTIME
			  << "\t" << std::chrono::duration_cast<msT>(realloc_time).count() << " (ms)"
#endif
			  << std::endl;

	std::cout << "calloc:  \t" << calloc_counter
#ifdef RUNTIME
			  << "\t" << std::chrono::duration_cast<msT>(calloc_time).count() << " (ms)"
#endif
			  << std::endl;
	std::cout << "===" << std::endl;
}
