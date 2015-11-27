#include <cstdlib>
#include <cassert>

int main() {
	void *t = malloc(100);
	assert(t != nullptr);

	void *t_realloc = realloc(t, 100 * 1024);
	assert(t_realloc != nullptr);
	t = t_realloc;

	free(t);
	return 0;
}
