#include "cinq_test.hpp"

int main(int argc, char **argv)
{
    // TODO: Automate the verification
    std::vector<int> my_vector { 1, 4, 6, 3, -6, 0, -3, 2 };
    auto result1 = cinq::from(my_vector)
                   .where([](int x) { return x > 0; })
                   .to_vector();
    
    std::cout << "[ ";
    for (auto x : result1)
    {
        std::cout << x << " ";
    }
    std::cout << "]\n";
    
    std::list<int> my_list(my_vector.cbegin(), my_vector.cend());
    auto result2 = cinq::from(my_list)
                   .where([](int x) { return x > 0; })
                   .to_vector();
    
    std::cout << "[ ";
    for (auto x : result2)
    {
        std::cout << x << " ";
    }
    std::cout << "]\n";

	std::array<int,4> my_array ={1,2,3,-4};

	auto result3 =cinq::from(my_array)
						.where([](int x){ return x > 0;})
						.to_vector();

	std::cout << "[ ";
	for( auto x : result3)
	{
		std::cout<< x <<" ";
	}

	std::cout <<"]\n";
	
	int my_carray[4] = {1,2,3,-4};

	auto result4 = cinq::from(my_carray)
							.where([](int x){return x > 0;})
							.to_vector();
	std::cout << "[ ";
	for(auto x : result4){
		std::cout<< x <<" ";
	}
	std::cout<<"] ";

    
    return 0;
}
