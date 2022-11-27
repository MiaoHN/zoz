#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEST_SUCCESS 1
#define TEST_FAILURE 0

typedef void (*TestFunc)(int* testResult);

typedef struct TestCase {
  TestFunc func;
  char*    name;
} TestCase;

typedef struct TestManager {
  TestCase** testCases;
  int        count;
} TestManager;

#define EXPECT_TRUE(x)                                           \
  if (!(x)) {                                                    \
    *testResult = TEST_FAILURE;                                  \
    printf("[==========] Failure: %s:%u\n", __FILE__, __LINE__); \
    printf("  Expected: True\n");                                \
    printf("    Actual: %s\n", (x) ? "True" : "False");          \
    return;                                                      \
  }

#define EXPECT_FALSE(x)                                          \
  if ((x)) {                                                     \
    *testResult = TEST_FAILURE;                                  \
    printf("[==========] Failure: %s:%u\n", __FILE__, __LINE__); \
    printf("  Expected: False\n");                               \
    printf("    Actual: %s\n", (x) ? "True" : "False");          \
    return;                                                      \
  }

#define EXPECT_STREQ(x, y)                                       \
  if (strcmp((x), (y))) {                                        \
    *testResult = TEST_FAILURE;                                  \
    printf("[==========] Failure: %s:%u\n", __FILE__, __LINE__); \
    printf("  Expected: '%s' == '%s'\n", x, y);                  \
    return;                                                      \
  }

#define EXPECT_EQ(x, y)                                          \
  if ((x) != (y)) {                                              \
    *testResult = TEST_FAILURE;                                  \
    printf("[==========] Failure: %s:%u\n", __FILE__, __LINE__); \
    printf("  Expected: %s == %s\n", #x, #y);                    \
    return;                                                      \
  }

#define EXPECT_NE(x, y)                                          \
  if ((x) == (y)) {                                              \
    *testResult = TEST_FAILURE;                                  \
    printf("[==========] Failure: %s:%u\n", __FILE__, __LINE__); \
    printf("  Expected: %s != %s\n", #x, #y);                    \
    return;                                                      \
  }

#define EXPECT_LE(x, y)                                          \
  if ((x) >= (y)) {                                              \
    *testResult = TEST_FAILURE;                                  \
    printf("[==========] Failure: %s:%u\n", __FILE__, __LINE__); \
    printf("  Expected: %s < %s\n", #x, #y);                     \
    return;                                                      \
  }

#define EXPECT_GT(x, y)                                          \
  if ((x) <= (y)) {                                              \
    *testResult = TEST_FAILURE;                                  \
    printf("[==========] Failure: %s:%u\n", __FILE__, __LINE__); \
    printf("  Expected: %s > %s\n", #x, #y);                     \
    return;                                                      \
  }

#define TEST(SET, NAME)                                                       \
  static void                     testRegister##SET##NAME();                  \
  void                            testRun##SET##NAME(int* testResult);        \
  extern TestManager              tests;                                      \
  __attribute((constructor)) void testRegister##SET##NAME() {                 \
    const size_t index    = tests.count++;                                    \
    const char*  name     = #SET "." #NAME;                                   \
    const size_t nameSize = strlen(name);                                     \
    TestCase*    testCase = (TestCase*)malloc(sizeof(TestCase));              \
    testCase->name        = (char*)name;                                      \
    testCase->func        = testRun##SET##NAME;                               \
    tests.testCases =                                                         \
        (TestCase**)realloc(tests.testCases, sizeof(TestCase) * tests.count); \
    tests.testCases[index] = testCase;                                        \
  }                                                                           \
  void testRun##SET##NAME(int* testResult)

extern TestManager tests;

void runTest(int argc, char** argv) {
  enum colors { RESET, GREEN, RED, YELLOW, BLUE };
  const char* colors[] = {"\033[0m", "\033[32m", "\033[31m", "\033[33m",
                          "\033[34m"};
  printf("%s[==========] Test Start%s\n", colors[BLUE], colors[RESET]);
  int    passCases = 0;
  double timeUsed  = 0.0;
  int    allPassed = 1;
  for (size_t i = 0; i < tests.count; ++i) {
    printf("[ Run      ] %s\n", tests.testCases[i]->name);
    int     testResult = TEST_SUCCESS;
    clock_t start, end;
    start = clock();
    tests.testCases[i]->func(&testResult);
    end       = clock();
    double dt = (double)(end - start) / CLOCKS_PER_SEC * 1000.0f;
    timeUsed += dt;
    if (testResult == TEST_FAILURE) {
      allPassed = 0;
      printf("%s[  FAILED  ]%s\n", colors[RED], colors[RESET]);
    }
    if (testResult == TEST_SUCCESS) {
      ++passCases;
      printf("%s[       OK ] (%.3fms)%s\n", colors[GREEN], dt, colors[RESET]);
    }
  }
  printf("%s[==========] Test Finish%s\n", colors[BLUE], colors[RESET]);
  printf("%s[xxxxxxxxxx]\n%s", allPassed ? colors[GREEN] : colors[RED],
         colors[RESET]);
  printf("[  PASSED  ] %d/%d\n", passCases, tests.count);
  printf("[   TIME   ] %.3fms\n", timeUsed);
  printf("[PERCENTAGE] %.2f%%\n", (float)passCases / tests.count * 100.0f);
}

TestManager tests = {0, 0};

#ifndef TEST_FORCE_MAIN
#define TEST_FORCE_MAIN

int main(int argc, char** argv) {
  runTest(argc, argv);
  return 0;
}

#endif  // TEST_FORCE_MAIN

#endif  // __TEST_H__