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
                      
        size_t answer =3, answer2=7;
        return ((result == answer)&&(result2==answer2));
    }));

    tests.push_back(test("count() std::vector", []
    {
        std::vector<int> my_vector = { 0, 1, 2, 3, 4 }; 
        auto result = cinq::from(my_vector)
                      .count([](int x) { return x <= 2; });

        
                      
        size_t answer =3;
        return (result == answer);
    }));

    tests.push_back(test("count() std::list", []
    {
        std::list<int> my_list = { 0, 1, 2, 3, 4 }; 
        auto result = cinq::from(my_list)
                      .count([](int x) { return x <= 2; });

        
                      
        size_t answer =3;
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
                      
        size_t answer =5, answer2=9;
        return ((result == answer)&&(result2==answer2));
    }));

    tests.push_back(test("count(void) std::vector", []
    {
        std::vector<int> my_vector = { 0, 1, 2, 3, 4 }; 
        auto result = cinq::from(my_vector)
                      .count();

        
                      
        size_t answer =5;
        return (result == answer);
    }));

    tests.push_back(test("count(void) std::list", []
    {
        std::list<int> my_list = { 0, 1, 2, 3, 4 }; 
        auto result = cinq::from(my_list)
                      .count();

        
                      
        size_t answer =5;
        return (result == answer);
    }));

    tests.push_back(test("take(int) std::vector", []
    {
        std::vector<int> my_vector ={ 0, 1, 2, 3, 4};
        auto result = cinq::from(my_vector)
                            .take(3).to_vector();

        std::vector<int> answer = {0,1,2};

        return (result==answer);
    }));
    
    tests.push_back(test("take(int) std::vector ensure_data", []
    {
        std::vector<int> my_vector ={ 0, 1, 2, 3, 4};
        auto temp = cinq::from(my_vector);
        temp.ensure_data();
        auto result = temp.take(3).to_vector();

        std::vector<int> answer = {0,1,2};

        return (result==answer);
    }));
    
    tests.push_back(test("take(int) std::vector ensure_data, count > size", []
    {
        std::vector<int> my_vector = { 0, 1 };
        auto temp = cinq::from(my_vector);
        temp.ensure_data();
        auto result = temp.take(3).to_vector();

        return (result == my_vector);
    }));
    
    tests.push_back(test("take(int) std::vector count > size", []
    {
        std::vector<int> my_vector = { 0, 1 };
        auto result = cinq::from(my_vector).take(3).to_vector();

        return (result == my_vector);
    }));
    
    tests.push_back(test("take(int) std::list", []
    {
        std::list<int> my_list = {0,1,2,3,4};
        auto result = cinq::from(my_list)
                            .take(3).to_vector();

       std::vector<int> answer={0,1,2};

       return (result==answer);
    }));
    
    tests.push_back(test("take(int) std::array", []
    {
        std::array<int,5> my_array = {0,1,2,3,4};
        auto result = cinq::from(my_array)
                            .take(3).to_vector();
        std::vector<int> answer={0,1,2};
        return (result==answer);
    }));
    
    tests.push_back(test("take(string).where() std::list", []
    {

        std::string a,b,c;
        a=b=c="str";
        
        std::list<std::string> my_list { a , b , c };
      
        auto result = cinq::from(my_list)
                      .where([](string x){ return x=="str"; })
                      .to_vector();
        std::vector<std::string> answer { a , b , c };
        return (result == answer);
    }));

    tests.push_back(test("take(string /*literals*/).where() std::vector ;", []
    {
        std::vector<string> my_vector {"cat","dog","rabbit","turtle"};

        auto result = cinq::from(my_vector)
                            .where([](string x){return (x=="cat"||x=="dog");})
                            .take(1)
                            .to_vector()[0];

        return result=="cat";
    }));
    
    tests.push_back(test("max() on int", []
    {
        vector<int> nums { -1, 26, 42, -10002, 0, 8 };
        int result = cinq::from(nums).max();
        return (result == 42);
    }));
    
    tests.push_back(test("max() on double", []
    {
        vector<double> nums { -0.1, 2.6, 4.2, -1000.2, 0.0, 0.8 };
        double result = cinq::from(nums).max();
        return (result == nums[2]); // 4.2
    }));
    
    tests.push_back(test("max() on strings with mapping function", []
    {
        vector<string> authors { "kevin chen", "jonathan barrios", "jonathan wang" };
        auto result = cinq::from(authors).max([](string x) { return x.length(); });
        return (result == authors[1].length()); // barrios
    }));
    
    tests.push_back(test("min() on int", []
    {
        vector<int> nums { -1, 26, 42, -10002, 0, 8 };
        int result = cinq::from(nums).min();
        return (result == -10002);
    }));
    
    tests.push_back(test("min() on double", []
    {
        vector<double> nums { -0.1, 2.6, 4.2, -1000.2, 0.0, 0.8 };
        double result = cinq::from(nums).min();
        return (result == nums[3]); // 4.2
    }));
    
    tests.push_back(test("min() on strings with mapping function", []
    {
        vector<string> authors { "kevin chen", "jonathan barrios", "jonathan wang" };
        auto result = cinq::from(authors).min([](string x) { return x.length(); });
        return (result == authors[0].length()); // chen
    }));
    
    tests.push_back(test("sum() on int", []
    {
        vector<int> nums { -1, 26, 42, -10002, 0, 8 };
        int result = cinq::from(nums).sum();
        return (result == -9927);
    }));
    
    tests.push_back(test("sum() on double", []
    {
        vector<double> nums { -0.1, 2.6, 4.2, -1000.2, 0.0, 0.8 };
        double result = cinq::from(nums).sum();
        return (result == nums[0] + nums[1] + nums[2] + nums[3] + nums[4] + nums[5]);
    }));
    
    tests.push_back(test("sum() on strings with mapping function", []
    {
        vector<string> authors { "kevin chen", "jonathan barrios", "jonathan wang" };
        auto result = cinq::from(authors).sum([](string x) { return x.length(); });
        return (result == authors[0].length() + authors[1].length() + authors[2].length());
    }));
    
    tests.push_back(test("average() on int", []
    {
        vector<int> nums { -1, 26, 42, -10002, 0, 8 };
        auto result = cinq::from(nums).average();
        return is_same<decltype(result), double>::value && (result == -9927 / (double)nums.size());
    }));
    
    tests.push_back(test("average() on float", []
    {
        vector<float> nums { -0.1, 2.6, 4.2, -1000.2, 0.0, 0.8 };
        auto result = cinq::from(nums).average();
        return is_same<decltype(result), float>::value
               && (result == (nums[0] + nums[1] + nums[2] + nums[3] + nums[4] + nums[5]) / nums.size());
    }));
    
    tests.push_back(test("average() on string length with mapping function", []
    {
        vector<string> authors { "kevin chen", "jonathan barrios", "jonathan wang", "hi" };
        auto result = cinq::from(authors).average([](string x) { return x.length(); });
        return is_same<decltype(result), double>::value
               && (result == (double)(authors[0].length() + authors[1].length() + authors[2].length() + authors[3].length()) / authors.size());
    }));
    
    tests.push_back(test("average() on float with mapping function", []
    {
        vector<float> nums { -0.1, 2.6, 4.2, -1000.2, 0.0, 0.8 };
        auto result = cinq::from(nums).average([](float x) { return x * x; });
        return is_same<decltype(result), float>::value
               && (result == (nums[0]*nums[0] + nums[1]*nums[1] + nums[2]*nums[2] + nums[3]*nums[3] + nums[4]*nums[4] + nums[5]*nums[5]) / nums.size());
    }));
    
    return tests;
}
