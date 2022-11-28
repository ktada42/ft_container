#include <iostream>
#include "type_traits.hpp"
//#include "vector.hpp"
#include <vector>

template<class T>
typename ft::enable_if<!ft::is_same<T, int>::value, void>::type print_type(T a)
{
	(void)a;
	std::cout << "not integral" << std::endl;
	
}

template<class T>
typename ft::enable_if<ft::is_same<T, int>::value, void>::type print_type(T a)
{
	(void)a;
	std::cout << "integral" << std::endl;
}

//enable_ifは後で確認できる
void type_traits_test()
{

}

int main()
{
	std::vector<int> a;
	return 0;
}