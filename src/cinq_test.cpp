#include "cinq_test.hpp"

int main(int argc, char **argv)
{
    auto tests = make_tests();
    int failed = 0;
    for (test t : tests)
    {
        if (t.func())
        {
            cout << "[    ] ";
        }
        else
        {
            cout << "[fail] ";
            failed++;
        }
        cout << t.name << "\n";
    }
    
    if (failed == 0)
    {
        printf("%lu tests passed :-)\n", tests.size());
    }
    else
    {
        printf("Failed %d / %lu tests\n", failed, tests.size());
    }
    
    return failed;
}

vector<test> make_tests()
{
    vector<test> tests;
    
    tests.push_back(test("where() std::vector", []
    {
        std::vector<int> my_vector { 1, 4, 6, 3, -6, 0, -3, 2 };
        auto result = cinq::from(my_vector)
                      .where([](int x) { return x > 0; })
                      .to_vector();
        std::vector<int> answer { 1, 4, 6, 3, 2 };
        return (result == answer);
    }));
    
    tests.push_back(test("where() std::list", []
    {
        std::list<int> my_list { 1, 4, 6, 3, -6, 0, -3, 2 };
        auto result = cinq::from(my_list)
                      .where([](int x) { return x > 0; })
                      .to_vector();
        std::vector<int> answer { 1, 4, 6, 3, 2 };
        return (result == answer);
    }));
    
    tests.push_back(test("where() std::array", []
    {
        std::array<int, 8> my_array = { 1, 4, 6, 3, -6, 0, -3, 2 };
        auto result = cinq::from(my_array)
                      .where([](int x){ return x > 0;})
                      .to_vector();
        std::vector<int> answer { 1, 4, 6, 3, 2 };
        return (result == answer);
    }));
    
    return tests;
}
