#pragma once

#include <new>
#include <limits>
#include <cstddef>

extern "C" void *__libc_malloc(size_t size);
extern "C" void *__libc_free(void *ptr);
extern "C" void *__libc_realloc(void *ptr, size_t new_size);
extern "C" void *__libc_calloc(size_t num, size_t size);

/**
 * This is an allocator using __libc_* functions.
 * It can be used in the malloc() function.
 */
template <class T> struct libc_allocator {
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using pointer = T *;
	using const_pointer = const T *;
	using reference = T &;
	using const_reference = const T &;
	using value_type = T;

	template <class U> struct rebind { typedef libc_allocator<U> other; };
	libc_allocator() throw() {}
	libc_allocator(const libc_allocator &) throw() {}

	template <class U> libc_allocator(const libc_allocator<U> &) throw() {}

	~libc_allocator() throw() {}

	pointer address(reference x) const { return &x; }
	const_pointer address(const_reference x) const { return &x; }

	pointer allocate(size_type s, void const * = 0) {
		if (s == 0) return nullptr;
		pointer temp = (pointer)__libc_malloc(s * sizeof(T));
		if (temp == nullptr) throw std::bad_alloc();
		return temp;
	}

	void deallocate(pointer p, size_type) { __libc_free(p); }

	size_type max_size() const throw() { return std::numeric_limits<size_t>::max() / sizeof(T); }

	void construct(pointer p, const T &val) { new ((void *)p) T(val); }

	void destroy(pointer p) { p->~T(); }
};
