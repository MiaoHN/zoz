#include "zoz.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "vm.h"

/**
 * @brief save repl command and execute status
 *
 */
typedef struct Repler {
  char* input;
  int   status;

  VM vm;
} Repler;

#define REPL_STATUS_RUNNING 0
#define REPL_STATUS_ERROR   1
#define REPL_STATUS_EXIT    2

static void show_prompt(Repler* repler) {
  if (repler->status == REPL_STATUS_RUNNING) {
    printf(":)");
  } else if (repler->status == REPL_STATUS_ERROR) {
    printf(":(");
  }

  printf(" >> ");
}

/**
 * @brief get user input command
 * @todo multiple line with '\' support
 */
static void get_line(Repler* repler) {
  static char buf[512];
  int         pos = 0;

  while ((buf[pos] = getchar()) != '\n') {
    ++pos;
  }

  buf[pos] = '\0';

  repler->input = buf;
}

/**
 * @brief run user's input command
 * @details notice that 'exit'/'exit()' and 'help'/'help()' should be specified
 * because they are not zoz functions
 *
 * @param input
 */
static void run_command(Repler* repler) {
  // specify exit and help
  if (!strcmp(repler->input, "exit") || !strcmp(repler->input, "exit()")) {
    printf("Bye\n");
    repler->status = REPL_STATUS_EXIT;
    return;
  }
  if (!strcmp(repler->input, "version") ||
      !strcmp(repler->input, "version()")) {
    printf("zoz version 0.0.1\n");
    repler->status = REPL_STATUS_RUNNING;
    return;
  }
  if (!strcmp(repler->input, "help") || !strcmp(repler->input, "help()")) {
    printf("zoz version 0.0.1\n");
    printf("type 'exit' to exit program\n");
    repler->status = REPL_STATUS_RUNNING;
    return;
  }

  // compile raw source code
  CompileResult compile_result = compile(repler->input);
  if (compile_result.status == COMPILE_STATUS_ERROR) {
    // printf("command '%s' not found!\n", repler->input);
    repler->status = REPL_STATUS_ERROR;
    return;
  } else {
    // run codes in vm
  }
}

void repl() {
  Repler repler;

  while (repler.status != REPL_STATUS_EXIT) {
    show_prompt(&repler);

    get_line(&repler);

    run_command(&repler);
  }
}