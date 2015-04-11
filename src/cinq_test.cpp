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

    tests.push_back(test("first() std::vector; ", []
    {
        std::vector<string> my_vector {"cat","dog","rabbit","turtle"};

        auto result = cinq::from(my_vector)
                            .first();


        return result=="cat";
    }));

    tests.push_back(test("first() ensure_data std::vector; ", [] {

        std::vector<string> my_vector {"cat","dog","rabbit","turtle"};
        auto temp = cinq::from(my_vector);
        temp.ensure_data();
        auto result = temp.first();

        return result=="cat";

    }));



    tests.push_back(test("first(predicate) ensure_data std::vector str; ",[] {

        std::vector<string> my_vector{"cat","dog","rabbit","turtle"};
        auto temp = cinq::from(my_vector);
        temp.ensure_data();
        auto result = temp.first([](string x){return x=="rabbit";});

        return result == "rabbit";



    }));
    tests.push_back(test("first(predicate) std::vector str; ",[] {

        std::vector<string> my_vector{"cat","dog","rabbit","turtle"};
        auto result = cinq::from(my_vector).first([](string x){return x=="rabbit";});
        return result == "rabbit";
    }));
    tests.push_back(test("first(predicate) std::list ints; ",[] {

        std::list<int> my_list{1,2,4,5};
        auto result = cinq::from(my_list).first([](int x){return x==5;});
        return result == 5;

    }));
    tests.push_back(test("first(predicate) ensure_data std::list doubles; ",[] {
        std::list<double> my_list{1.435,2.4345,4.454,5.5555};
        auto temp = cinq::from(my_list);
        temp.ensure_data();
        auto result = temp.first([](double x){return x>4;});
        return result == 4.454;

    }));
    tests.push_back(test("last(predicate) ensure_data std::vector strings; ",[] {
        std::vector<string> my_vector{"C++","python","ada", "Ocaml","C#"};
        auto temp =cinq::from(my_vector);
        temp.ensure_data();

        auto result = temp.last([](string x){return x.length()>3;});
        
        return result=="Ocaml";


    }));

    tests.push_back(test("last()  std::list int; ",[] {

        std::list<int> my_list{1,2,4,5,7};
        auto result =cinq::from(my_list).last();

        return result==7;


    }));
    tests.push_back(test("last() ensure_data std::list int; ",[] {

        std::list<int> my_list{1,2,4,5,7};
        auto temp =cinq::from(my_list);
        temp.ensure_data();
        auto result = temp.last();

        return result==7;

        }));

     tests.push_back(test("single() ensure_data std::list string",[]{

        std::list<string> my_list{"cat" /*,"apple","face","snake"*/};
        auto temp=cinq::from(my_list);
        temp.ensure_data();
        auto result = temp.single();
        return result=="cat";

     }));

     tests.push_back(test("single()  std::list string",[]{

        std::list<string> my_list{"cat" /*,"apple","face","snake"*/};
        auto temp=cinq::from(my_list);
      //  temp.ensure_data();
        auto result = temp.single();
        return result=="cat";

     }));

     tests.push_back(test("single()  std::vector string",[]{

        std::vector<string> my_vector{"cat"};
        auto temp=cinq::from(my_vector);
        temp.ensure_data();
        auto result = temp.single();
        return result=="cat";


    }));

     tests.push_back(test("single(predicate) ensure_data std::vector string",[]{

            std::vector<string> my_vector{"cat","dog","goat","pig"};
            auto temp = cinq::from(my_vector);
            temp.ensure_data();
            auto result = temp.single([](string x){return x.length()==4;});
            return result=="goat";


         }));

     tests.push_back(test("single(predicate) std::vector string",[]{

            std::vector<string> my_vector{"cat","dog","goat","pig"};
            auto temp = cinq::from(my_vector);
            //temp.ensure_data();
            auto result = temp.single([](string x){return x.length()==4;});
            return result=="goat";


         }));

     tests.push_back(test("skip(string) std::vector string",[]{

             std::vector<string> my_vector{"cat","dog","goat","pig"};
             auto temp = cinq::from(my_vector);
            // temp.ensure_data();
            auto result= temp.skip(2).to_vector();
           // for(string a: result) cout<<a<<endl;
             std::vector<string> answer{"goat","pig"};
             return result == answer;


    }));

     tests.push_back(test("skip(string) ensure_data std::vector string",[]{

                std::vector<string> my_vector{"cat","dog","goat","pig"};
             auto temp = cinq::from(my_vector);
             temp.ensure_data();
            auto result= temp.skip(2).to_vector();
           // for(string a: result) cout<<a<<endl;
             std::vector<string> answer{"goat","pig"};
             return result == answer;

        }));

     tests.push_back(test("skip(string) std::list string",[]{

            std::list<string> my_vector{"cat","dog","goat","pig"};
             auto temp = cinq::from(my_vector);
             //temp.ensure_data();
            auto result= temp.skip(2).to_vector();
           // for(string a: result) cout<<a<<endl;
             std::vector<string> answer{"goat","pig"};
             return result == answer;

        }));




    
    return tests;
}
