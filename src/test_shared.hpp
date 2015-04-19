#ifndef __test_shared_hpp__
#define __test_shared_hpp__

#include <iostream>
#include <chrono>

using namespace std;

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

class test_perf
{
public:
    test_perf(string name, function<void()> func, int run_count = 100)
    {
        this->name = name;
        this->func = [=]()
        {
            using namespace std::chrono;
            
            auto begin = high_resolution_clock::now();
            for (int i = 0; i < run_count; i++) func();
            auto end = high_resolution_clock::now();
            return duration_cast<milliseconds>(end - begin).count();
        };
    }
    
    string name;
    function<int()> func;
};

#endif
