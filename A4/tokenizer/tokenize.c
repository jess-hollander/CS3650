#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../vector/vect.h"
#include "tokens.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  char *buffer = NULL;
  size_t capacity = 0;
  ssize_t length = getline(&buffer, &capacity, stdin);

  if (length >= 0) {
    vect_t *tokens = tokenize(buffer);
    for (unsigned int i = 0; i < vect_size(tokens); i++) {
      puts(vect_get(tokens, i));
    }
    vect_delete(tokens);
  }

  free(buffer);
  return ferror(stdin) ? EXIT_FAILURE : EXIT_SUCCESS;
}
