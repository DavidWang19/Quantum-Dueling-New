#ifndef UBA_INT
#define UBA_INT

/* Declaration of exported functions for unbounded array.
 * This is a modification of the uba implementation from CMU 15122.
 */ 

typedef struct uba_header uba;

// Define the actual uba structure
struct uba_header {
	int size;          // 0 <= size && size < limit
	int limit;         // 0 < limit
	int* data;         // len(data) == limit
};

/* Get the length of an uba */
int uba_len(uba* A);

/* Get the ith element of an uba */
int uba_get(uba* A, int i);

/* Set the ith element of an uba */
void uba_set(uba* A, int i, int x);

/* Initializa a new uba with size = size */
uba* uba_new(int size);

/* Resize the given uba to have size = new_limit */
void uba_resize(uba* A, int new_limit);

/* Add an element to an uba */
void uba_add(uba* A, int x);

/* Remove an element from an uba */
int uba_rem(uba* A);

/* Print out all elements of an uba */
void uba_print(uba* A);

#endif