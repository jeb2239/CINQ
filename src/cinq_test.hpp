#ifndef __cinq_test_hpp__
#define __cinq_test_hpp__

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <deque>
#include <functional>
#include <initializer_list>
#include <cmath>
#include "cinq_enumerable.hpp"
#include "all_concepts.hpp"
#include "test_shared.hpp"
#include "test_performance.hpp"

using namespace std;
using namespace origin;

int test_correctness();
void test_performance();
vector<test> make_tests();

#endif
