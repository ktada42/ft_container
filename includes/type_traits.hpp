/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:31:18 by ktada             #+#    #+#             */
/*   Updated: 2022/11/26 22:03:18 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <vector>

#define enable_func_if(bool_val) ft2::enable_if<bool_val>* = NULL

//継承　修飾し　public にしないといけない
//const volatile はconst Tで受け取れるのか
namespace ft
{
	template <class T, T v>
	struct integral_constant 
	{
		static const T value 			= v;
    	typedef							T value_type;
		typedef integral_constant<T,v>	type;
	};

	typedef integral_constant<bool, true>  true_type;
	typedef integral_constant<bool, false> false_type;

	template <class T> 
	struct type_class {typedef T type;};

	//ok enable_if 
	template <bool, class T = void>
	struct enable_if {};

	template <class T>
	struct enable_if<true, T> : public type_class<T> {};

	
	//ok is_same
	template <class T, class U>
	struct is_same: public false_type{};

	template <class T>
	struct is_same<T, T>:public true_type{};


	//remove_cv
	template <class T> 
	struct remove_const : public type_class<T>{};

	template <class T> 
	struct remove_const<const T>:public type_class<T>{};
		
	template <class T> 
	struct remove_volatile:public type_class<T>{};
		
	template <class T> 
	struct remove_volatile<volatile T>:public type_class<T>{};

	template <class T> 
	struct remove_cv:public type_class<typename remove_volatile<typename remove_const<T>::type>::type>{};

	//ok is_integral
	template <class T> 
	struct _is_integral: public false_type{};
 
	template <>
	struct _is_integral<unsigned char> : public true_type{};
	template <>
	struct _is_integral<unsigned short> : public true_type{};
	template <>
	struct _is_integral<unsigned int> : public true_type{};
	template <>
	struct _is_integral<unsigned long> : public true_type{};
	template <>
	struct _is_integral<unsigned long long> : public true_type{};
	template <>
	struct _is_integral<char> : public true_type{};
	template <>
	struct _is_integral<short> : public true_type{};
	template <>
	struct _is_integral<int> : public true_type{};
	template <>
	struct _is_integral<long> : public true_type{};
	template <>
	struct _is_integral<long long> : public true_type{};
	template <>
	struct _is_integral<wchar_t> : public true_type{};

	template <class T> 
	struct is_integral :public _is_integral<typename remove_cv<T>::type>{};
}
#endif

