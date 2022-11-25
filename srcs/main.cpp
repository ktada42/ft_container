#include <iostream>
#include "type_traits.hpp"
//#include "vector.hpp"

template<class T>
typename ft::enable_if<!ft::is_integral<T>::value, void>::type print_type(T a)
{
	(void)a;
	std::cout << "not integral" << std::endl;
	
}

template<class T>
typename ft::enable_if<ft::is_integral<T>::value, void>::type print_type(T a)
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
	type_traits_test();

	print_type(0);
	print_type((double)0);
	return 0;
}