#ifndef __test_code_hpp__
#define __test_code_hpp__

//when the other file gets to big just write the code here and it will be pound included at 
//the end of the test function
//#define START_TEST(MSG) tests.push_back(test(MSG, []{
//#define END_TEST() }));
//

START_TEST("take(string).where() std::list")

	std::string a,b,c;
	a=b=c="str";

  std::list<std::string> my_list { a , b , c };
  
        auto result = cinq::from(my_list)
                      .where([](string x){ return x=="str"; })
                      .to_vector();
        std::vector<std::string> answer { a , b , c };
        return (result == answer);

END_TEST

START_TEST("take(string /*literals*/).where() std::vector ;")

	std::vector<string> my_vector {"cat","dog","rabbit","turtle"};

	auto result = cinq::from(my_vector)
						.where([](string x){return (x=="cat"||x=="dog");})
						.take(1)
						.to_vector()[0];

	return result=="cat";

END_TEST















#endif