#define _DEFAULT_SOURCE
#define _BSD_SOURCE

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <malloc.h>

// NOTE: You should NOT include <stdlib.h> in your final implementation.
// malloc.h turns every call to malloc/calloc/free in this file into a call
// to mymalloc/mycalloc/myfree, so calling them here would recurse forever.

#include <debug.h> // definition of debug_printf

extern void *sbrk(intptr_t increment);

/*
 * each header is stored immediately before its user memory
 * blocks stay in the list in the order sbrk returned them
 * this allocator does not split blocks or combine adjacent free blocks
 */
typedef struct block {
  size_t size;
  struct block *next;
  int free;
} block_t;

static block_t *head = NULL;

/*
 * find the first reusable block large enough for size
 * assumes size is nonzero
 * returns NULL when no block fits.
 */
static block_t *find_first_fit(size_t size) {
  assert(size > 0);

  for (block_t *block = head; block != NULL; block = block->next) {
    assert(block->size > 0);
    assert(block->free == 0 || block->free == 1);
    if (block->free && block->size >= size) {
      return block;
    }
  }
  return NULL;
}

/*
 * add a newly obtained block to the end of the allocation-order list
 * assumes block is initialized and is not already in the list
 */
static void append_block(block_t *block) {
  assert(block != NULL);
  assert(block->size > 0);
  assert(block->next == NULL);
  assert(block->free == 0);

  if (head == NULL) {
    head = block;
    return;
  }

  block_t *tail = head;
  while (tail->next != NULL) {
    tail = tail->next;
  }

  tail->next = block;
}

/*
 * find the header immediately before a user pointer
 * assumes ptr was returned by this allocator and is not NULL
 */
static block_t *block_from_ptr(void *ptr) {
  assert(ptr != NULL);
  return (block_t *)((char *)ptr - sizeof(block_t));
}

/*
 * allocate size bytes using first fit, growing the heap only when necessary
 * returns a pointer to user memory, or NULL for a zero/invalid size or failure
 */
void *mymalloc(size_t s) {
  if (s == 0) {
    return NULL;
  }

  debug_printf("Malloc %zu bytes\n", s);

  if (s > SIZE_MAX - sizeof(block_t) ||
      s > (size_t)INTPTR_MAX - sizeof(block_t)) {
    return NULL;
  }

  block_t *block = find_first_fit(s);
  if (block == NULL) {
    size_t request_size = sizeof(block_t) + s;
    void *new_block = sbrk((intptr_t)request_size);
    if (new_block == (void *)-1) {
      return NULL;
    }

    block = (block_t *)new_block;
    block->size = s;
    block->next = NULL;
    block->free = 0;
    append_block(block);
  } else {
    block->free = 0;
  }

  return (void *)((char *)block + sizeof(block_t));
}

/*
 * allocate nmemb zeroed elements of size bytes each.
 * returns NULL for a zero dimension, multiplication overflow, or allocation
 * failure; otherwise returns the zero-initialized memory
 */
void *mycalloc(size_t nmemb, size_t s) {
  if (s != 0 && nmemb > SIZE_MAX / s) {
    return NULL;
  }

  size_t total_size = nmemb * s;
  void *ptr = mymalloc(total_size);
  debug_printf("Calloc %zu bytes\n", total_size);
  if (ptr == NULL) {
    return NULL;
  }

  memset(ptr, 0, total_size);
  return ptr;
}

/*
 * mark an allocator-owned block as reusable
 * assumes any non-NULL pointer was returned by mymalloc or mycalloc
 */
void myfree(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  block_t *block = block_from_ptr(ptr);
  assert(block->size > 0);
  assert(block->free == 0 || block->free == 1);
  if (block->free) {
    return;
  }

  block->free = 1;
  debug_printf("Freed %zu bytes\n", block->size);
}
