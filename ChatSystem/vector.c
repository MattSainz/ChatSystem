#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

void vector_init(Vector *vector) {
  // initialize size and capacity
  vector->size = 0;
  vector->capacity = VECTOR_INITIAL_CAPACITY;

  // allocate memory for vector->data
  *vector->data = malloc(sizeof(Node) * vector->capacity);
}

void vector_append(Vector *vector, Node *value) {
  // make sure there's room to expand into
  vector_double_capacity_if_full(vector);

  // append the value and increment vector->size
  vector->data[vector->size++] = value;
}

Node* vector_get(Vector *vector, int index) {
  if (index >= vector->size || index < 0) {
    printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
    exit(1);
  }
  return vector->data[index];
}

void vector_double_capacity_if_full(Vector *vector) {
  if (vector->size >= vector->capacity) {
    // double vector->capacity and resize the allocated memory accordingly
    vector->capacity *= 2;
    *vector->data = realloc(vector->data, sizeof(Node) * vector->capacity);
  }
}

void vector_free(Vector *vector) {
  int i = 0;
  Node* doomed;
  while( i < vector->capacity)
  {
    doomed = vector_get(vector, i);
    if( doomed->session != NULL)
    {
      free(doomed->session);
    }
    i++;
  }
  free(vector->data);
}
