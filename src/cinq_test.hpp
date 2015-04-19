#ifndef __cinq_test_hpp__
#define __cinq_test_hpp__

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <deque>
#include <functional>
#include <initializer_list>

#include "cinq_enumerable.hpp"
#include "all_concepts.hpp"

using namespace std;
using namespace origin;

class test
{
public:
    test(string name, function<bool()> func)
    {
        this->name = name;
        this->func = func;
    }
    
    string name;
    function<bool()> func;
};

vector<test> make_tests();

#endif
