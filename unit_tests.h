#pragma once
#include <iostream>
#include <stdexcept>

// Custom assertion macros
#define ASSERT_TRUE(cond) \
    if (!(cond)) throw std::runtime_error("ASSERT_TRUE failed: " #cond);

#define ASSERT_EQ(a,b) \
    if (!((a) == (b))) throw std::runtime_error("ASSERT_EQ failed: " #a " != " #b);

#define ASSERT_GT(a,b) \
    if (!((a) > (b))) throw std::runtime_error("ASSERT_GT failed: " #a " <= " #b);

// Forward declaration of test runner
int run_unit_tests();