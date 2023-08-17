#pragma once
#include <iostream>
#include <time.h>

#define TEST(FUNCTION)                                                         \
  do {                                                                         \
    int start = clock();                                                       \
    FUNCTION;                                                                  \
    int end = clock();                                                         \
    std::cerr << "running time of " #FUNCTION " is " << (end - start) / 1000   \
              << "ms" << std::endl;                                            \
  } while (0)
