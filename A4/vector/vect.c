/**
 * Vector implementation.
 *
 * - Implement each of the functions to create a working growable array (vector).
 * - Do not change any of the structs
 * - When submitting, You should not have any 'printf' statements in your vector 
 *   functions.
 *
 * IMPORTANT: The initial capacity and the vector's growth factor should be 
 * expressed in terms of the configuration constants in vect.h
 */
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vect.h"

/** Main data structure for the vector. */
struct vect {
  char **data;             /* Array containing the actual data. */
  unsigned int size;       /* Number of items currently in the vector. */
  unsigned int capacity;   /* Maximum number of items the vector can hold before growing. */
};

static char *copy_string(const char *str) {
  assert(str != NULL);

  size_t len = strlen(str) + 1;
  char *copy = malloc(len);
  assert(copy != NULL);
  memcpy(copy, str, len);
  return copy;
}

/** Construct a new empty vector. */
vect_t *vect_new() {
  assert(VECT_INITIAL_CAPACITY > 0);
  assert(VECT_GROWTH_FACTOR > 1);

  vect_t *v = malloc(sizeof(*v));
  assert(v != NULL);
  v->data = malloc((size_t)VECT_INITIAL_CAPACITY * sizeof(*v->data));
  assert(v->data != NULL);
  v->size = 0;
  v->capacity = VECT_INITIAL_CAPACITY;
  return v;
}

/** Delete the vector, freeing all memory it occupies. */
void vect_delete(vect_t *v) {
  if (v == NULL) {
    return;
  }
  for (unsigned int i = 0; i < v->size; i++) {
    free(v->data[i]);
  }
  free(v->data);
  free(v);
}

/** Get the element at the given index. */
const char *vect_get(vect_t *v, unsigned int idx) {
  assert(v != NULL);
  assert(idx < v->size);

  return v->data[idx];
}

/** Get a copy of the element at the given index. The caller is responsible
 *  for freeing the memory occupied by the copy. */
char *vect_get_copy(vect_t *v, unsigned int idx) {
  assert(v != NULL);
  assert(idx < v->size);

  return copy_string(v->data[idx]);
}
/** Set the element at the given index. */
void vect_set(vect_t *v, unsigned int idx, const char *elt) {
  assert(v != NULL);
  assert(idx < v->size);

  char *replacement = copy_string(elt);
  free(v->data[idx]);
  v->data[idx] = replacement;
}

/** Add an element to the back of the vector. */
void vect_add(vect_t *v, const char *elt) {
  assert(v != NULL);

  char *copy = copy_string(elt);
  if (v->size == v->capacity) {
    assert(v->capacity < VECT_MAX_CAPACITY);
    unsigned int new_capacity;
    if (v->capacity > VECT_MAX_CAPACITY / VECT_GROWTH_FACTOR) {
      new_capacity = VECT_MAX_CAPACITY;
    } else {
      new_capacity = v->capacity * VECT_GROWTH_FACTOR;
    }
    assert(new_capacity > v->capacity);
    assert((size_t)new_capacity <= SIZE_MAX / sizeof(*v->data));

    char **new_data = realloc(v->data, (size_t)new_capacity * sizeof(*v->data));
    assert(new_data != NULL);
    v->data = new_data;
    v->capacity = new_capacity;
  }
  v->data[v->size] = copy;
  v->size++;
}

/** Remove the last element from the vector. */
void vect_remove_last(vect_t *v) {
  assert(v != NULL);

  assert(v->size > 0);
  free(v->data[v->size - 1]);
  v->data[v->size - 1] = NULL;
  v->size--;
}

/** The number of items currently in the vector. */
unsigned int vect_size(vect_t *v) {
  assert(v != NULL);

  return v->size;
}

/** The maximum number of items the vector can hold before it has to grow. */
unsigned int vect_current_capacity(vect_t *v) {
  assert(v != NULL);

  return v->capacity;
}
