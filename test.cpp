#include <cstdlib>

int main() {
	void *t = malloc(100);
	free(t);
	return 0;
}
