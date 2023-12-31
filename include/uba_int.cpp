#include <cstdlib>
#include <iostream>
#include <cassert>
#include <limits>

#include "uba_int.h"

using namespace std;

/* Implementation of exported functions for unbounded array.
 * This is a modification of the uba implementation from CMU 15122.
 */ 

/* Get the length of an uba */
int uba_len(uba *A)
{
    return A->size;
}

/* Get the ith element of an uba */
int uba_get(uba* A, int i)
{
    assert(0 <= i && i < uba_len(A));
    return A->data[i];
}

/* Set the ith element of an uba */
void uba_set(uba* A, int i, int x)
{
    assert(0 <= i && i < uba_len(A));
    A->data[i] = x;
}

/* Initializa a new uba with size = size */
uba* uba_new(int size)
{   
    assert(0 <= size);
    uba* A = static_cast<uba *>(malloc(sizeof(uba)));
    int limit = size == 0 ? 1 : size * 2;
    A->data = static_cast<int *>(calloc(limit, sizeof(int)));
    A->size = size;
    A->limit = limit;
    assert(uba_len(A) == size);
    return A;
}

/* Resize the given uba to have size = new_limit */
void uba_resize(uba* A, int new_limit)
/* A may not be a valid array since A->size == A->limit is possible! */
{
    assert(A != nullptr);
    assert(0 <= A->size && A->size < new_limit);

   int *B = static_cast<int *>(calloc(new_limit, sizeof(int)));

    for (int i = 0; i < A->size; i++)
    {
        B[i] = A->data[i];
        assert(0 <= i);
    }

    A->limit = new_limit;
    A->data = B;
}

/* Add an element to an uba */
void uba_add(uba* A, int x)
{
    A->data[A->size] = x;
    (A->size)++;

    if (A->size < A->limit) return;
    assert(A->limit <= (std::numeric_limits<int>::max()) / 2); // Fail if array would get too big
    uba_resize(A, A->limit * 2);
}

/* Remove an element from an uba */
int uba_rem(uba* A)
//@requires is_uba(A);
//@requires 0 < uba_len(A);
//@ensures is_uba(A);
{
    (A->size)--;
    int x = A->data[A->size];

    if (A->limit >= 4 && A->size <= A->limit / 4)
        uba_resize(A, A->limit / 2);

    return x;
}

/* Print out all elements of an uba */
void uba_print(uba* A)
//@requires is_uba(A);
{
    printf("[");
    for (int i = 0; i < A->size; i++)
    {
        printf("%d", A->data[i]);
        if (i + 1 != A->size) printf(", ");
    }
    printf("]");
}

