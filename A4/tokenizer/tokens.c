#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

static int is_whitespace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\n';
}

static int is_special(char ch) {
  return ch == '(' || ch == ')' || ch == '<' || ch == '>' ||
         ch == ';' || ch == '|';
}

static void add_token(vect_t *tokens, char *buffer, size_t *length) {
  buffer[*length] = '\0';
  vect_add(tokens, buffer);
  *length = 0;
}

vect_t *tokenize(const char *input) {
  assert(input != NULL);

  size_t input_length = strlen(input);
  char *buffer = malloc(input_length + 1);
  assert(buffer != NULL);

  vect_t *tokens = vect_new();
  size_t length = 0;
  int in_quotes = 0;
  int token_started = 0;

  for (size_t i = 0; i < input_length; i++) {
    char ch = input[i];

    if (in_quotes) {
      if (ch == '"') {
        in_quotes = 0;
      } else {
        buffer[length++] = ch;
      }
      continue;
    }

    if (ch == '"') {
      in_quotes = 1;
      token_started = 1;
    } else if (is_whitespace(ch)) {
      if (token_started) {
        add_token(tokens, buffer, &length);
        token_started = 0;
      }
    } else if (is_special(ch)) {
      if (token_started) {
        add_token(tokens, buffer, &length);
        token_started = 0;
      }
      buffer[0] = ch;
      length = 1;
      add_token(tokens, buffer, &length);
    } else {
      buffer[length++] = ch;
      token_started = 1;
    }
  }

  if (token_started) {
    add_token(tokens, buffer, &length);
  }

  free(buffer);
  return tokens;
}
