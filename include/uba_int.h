#ifndef UBA_INT
#define UBA_INT

typedef struct uba_header uba;

struct uba_header {
	int size;          // 0 <= size && size < limit
	int limit;         // 0 < limit
	int* data;         // len(data) == limit
};

// Implementation of exported functions
int uba_len(uba* A);

int uba_get(uba* A, int i);

void uba_set(uba* A, int i, int x);

uba* uba_new(int size);

void uba_resize(uba* A, int new_limit);

void uba_add(uba* A, int x);

int uba_rem(uba* A);

void uba_print(uba* A);

#endif