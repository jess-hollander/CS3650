#include <stdio.h>
#include <stdlib.h>

extern long crunch(long, long);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Two arguments required.\n");
    return 1;
  }

  long result = crunch(atol(argv[1]), atol(argv[2]));

  if (result < 0) {
    printf("hat\n");
  } else if (result == 0) {
    printf("tea\n");
  } else {
    printf("beer\n");
  }

  return 0;
}
