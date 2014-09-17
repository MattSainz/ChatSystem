/* 
 * File:   vector.h
 * Author: Happy Bear Software
 *
 * Created on September 16, 2014, 9:12 PM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#define VECTOR_INITIAL_CAPACITY 100

typedef struct
{
  char* session;
  int port;
} Node;

// Define a vector type
typedef struct {
  int size;      // slots used so far
  int capacity;  // total available slots
  Node* data[];     // array of integers we're storing
} Vector;

void vector_init(Vector *vector);

void vector_append(Vector *vector, Node *value);

Node* vector_get(Vector *vector, int index);

void vector_set(Vector *vector, int index, int value);

void vector_double_capacity_if_full(Vector *vector);

void vector_free(Vector *vector);


#ifdef	__cplusplus
}
#endif

#endif	/* VECTOR_H */

