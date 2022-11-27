#include <stdio.h>

#include "zoz.h"

int main(int argc, char const *argv[]) {
  if (argc == 1) {
    repl();
  } else {
    printf("Now only support repl mode!\n");
    printf("Please retry with command '%s'\n", argv[0]);
  }
  return 0;
}