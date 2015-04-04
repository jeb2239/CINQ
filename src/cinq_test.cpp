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
    
    tests.push_back(test("where() with index std::vector", []
    {
        std::vector<int> my_vector { 0, 1, 2, 3, 4 };
        auto result = cinq::from(my_vector)
                      .where([](int x, int index) { return index <= 2; })
                      .to_vector();
        std::vector<int> answer { 0, 1, 2 };
        return (result == answer);
    }));
    
    
    tests.push_back(test("where() with index std::list", []
    {
        std::list<int> my_list { 0, 1, 2, 3, 4 };
        auto result = cinq::from(my_list)
                      .where([](int x, int index) { return index <= 2; })
                      .to_vector();
        std::vector<int> answer { 0, 1, 2 };
        return (result == answer);
    }));
    
    tests.push_back(test("where() with index std::array", []
    {
        std::array<int, 8> my_array = { 0, 1, 2, 3, 4 };
        auto result = cinq::from(my_array)
                      .where([](int x, int index) { return index <= 2; })
                      .to_vector();
        std::vector<int> answer { 0, 1, 2 };
        return (result == answer);
    }));
    
    tests.push_back(test("count() std::array", []
    {
        std::array<int, 5> my_array = { 0, 1, 2, 3, 4 }; // how much space is allocated changes the 
        												// the reponse from count
        auto result = cinq::from(my_array)
                      .count([](int x) { return x <= 2; });

        std::array<int, 9> my_array2 = { 0, 1, 2, 3, 4 }; //note the difference
        auto result2 = cinq::from(my_array2)
                      .count([](int x) { return x <= 2; });
                      
        int answer =3, answer2=7;
        return ((result == answer)&&(result2==answer2));
    }));

    tests.push_back(test("count() std::vector", []
    {
        std::vector<int> my_vector = { 0, 1, 2, 3, 4 }; 
        auto result = cinq::from(my_vector)
                      .count([](int x) { return x <= 2; });

        
                      
        int answer =3;
        return (result == answer);
    }));

    tests.push_back(test("count() std::list", []
    {
        std::list<int> my_list = { 0, 1, 2, 3, 4 }; 
        auto result = cinq::from(my_list)
                      .count([](int x) { return x <= 2; });

        
                      
        int answer =3;
        return (result == answer);
    }));

    tests.push_back(test("all() std::vector", []
    {
        std::vector<int> my_vector { 0, 1, 2, 3, 4 };
        bool result1 = cinq::from(my_vector)
                      .all([](int x) { return x <= 2; });
                      
        bool answer1=false;

        bool result2 = cinq::from(my_vector)
        				.all([](int x){return x<=1000;});
        bool answer2=true;

        bool passed = (result1==answer1)&&(result2==answer2);


        return (passed);
    }));

    tests.push_back(test("all() std::array", []
    {
        std::array<int,5> my_array = { 0, 1, 2, 3, 4 };
        bool result1 = cinq::from(my_array)
                      .all([](int x) { return x <= 2; });
                      
        bool answer1=false;

        bool result2 = cinq::from(my_array)
        				.all([](int x){return x<=1000;});
        bool answer2=true;

        bool passed = (result1==answer1)&&(result2==answer2);


        return (passed);
    }));

    tests.push_back(test("all() std::list", []
    {
        std::list<int> my_list = { 0, 1, 2, 3, 4 };
        bool result1 = cinq::from(my_list)
                      .all([](int x) { return x <= 2; });
                      
        bool answer1=false;

        bool result2 = cinq::from(my_list)
        				.all([](int x){return x<=1000;});
        bool answer2=true;

        bool passed = (result1==answer1)&&(result2==answer2);


        return (passed);
    }));

    
     tests.push_back(test("count(void) std::array", []
    {
        std::array<int, 5> my_array = { 0, 1, 2, 3, 4 }; // how much space is allocated changes the 
                                                        // the reponse from count
        auto result = cinq::from(my_array)
                      .count();

        std::array<int, 9> my_array2 = { 0, 1, 2, 3, 4 }; //note the difference
        auto result2 = cinq::from(my_array2)
                      .count();
                      
        int answer =5, answer2=9;
        return ((result == answer)&&(result2==answer2));
    }));

    tests.push_back(test("count(void) std::vector", []
    {
        std::vector<int> my_vector = { 0, 1, 2, 3, 4 }; 
        auto result = cinq::from(my_vector)
                      .count();

        
                      
        int answer =5;
        return (result == answer);
    }));

    tests.push_back(test("count(void) std::list", []
    {
        std::list<int> my_list = { 0, 1, 2, 3, 4 }; 
        auto result = cinq::from(my_list)
                      .count();

        
                      
        int answer =5;
        return (result == answer);
    }));





    return tests;
}
