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

void d(std::vector<int> &a)
{
	std::cout << "size : "<<a.size()<<" cap: "<<a.capacity() << std::endl;
}
struct a
{
	int *p;
	a()
	{
		p = new int[7];
	}
	~a()
	{
		delete[] p;
	}
};
//size 0のbeginがどうなってるか

void p(std::vector<int> & a)
{
	for (size_t i = 0; i < a.size(); i++)
	{
		std::cout << "a["<<i<<"] == "<<a[i] << std::endl;
	}
	
}
int main()
{
	std::vector<int> a(5);
	for (size_t i = 0; i < a.size(); i++)
	{
		a[i] = i;
	}
	
	a.resize(3, 10);
	p(a);

	return 0;
}