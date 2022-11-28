/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 20:04:49 by ktada             #+#    #+#             */
/*   Updated: 2022/11/28 15:16:11 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <algorithm>
#include <iostream>
#include <memory>
#include "type_traits.hpp"

/*
Member type	Definition
value_type	T
allocator_type	Allocator
size_type	Unsigned integer type (usually std::size_t)
difference_type	Signed integer type (usually std::ptrdiff_t)
reference	value_type&
const_reference	const value_type&
pointer	 
Allocator::pointer	(until C++11)
std::allocator_traits<Allocator>::pointer	(since C++11)
 
const_pointer	 
Allocator::const_pointer	(until C++11)
std::allocator_traits<Allocator>::const_pointer	(since C++11)
 
iterator	 
LegacyRandomAccessIterator and LegacyContiguousIterator to value_type

(until C++20)
LegacyRandomAccessIterator, contiguous_iterator, and ConstexprIterator to value_type

(since C++20)
 
const_iterator	 
LegacyRandomAccessIterator and LegacyContiguousIterator to const value_type

(until C++20)
LegacyRandomAccessIterator, contiguous_iterator, and ConstexprIterator to const value_type

(since C++20)
 
reverse_iterator	std::reverse_iterator<iterator>
const_reverse_iterator	std::reverse_iterator<const_iterator>
*/
namespace ft
{
	template <class T, class Allocator = std::allocator<T>>	
	class vector
	{
	//member type
	public:
		typedef T                                      value_type;
		typedef Allocator                              allocator_type;
		typedef std::size_t                            size_type;
		typedef std::ptrdiff_t                         difference_type;
		typedef value_type&                            reference;
		typedef const value_type&                      const_reference;
		typedef typename allocator_type::pointer       pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef random_access_iterator<pointer>        iterator;
		typedef random_access_iterator<const_pointer>  const_iterator;
		typedef ft::reverse_iterator<iterator>         reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>   const_reverse_iterator;
	//constructors
		vector() : alloc(allocator_type())
		{
			set_init_ptr();
		}
		
		explicit vector( const allocator_type& alloc ) : alloc(alloc)
		{
			set_init_ptr();	
		}

		explicit vector(size_type count,
                 		const T& value = T(),
                 		const Allocator& alloc = Allocator()) : alloc(alloc)
		{
			set_init_ptr();
			
		}
		
	private:
		void set_init_ptr()
		{
			begin_ptr = NULL;
			end_ptr = NULL;
			reserved_end = NULL;
		}

	protected:
		pointer			begin_ptr;
		pointer			end_ptr;
		pointer			reserved_end;
		allocator_type	alloc;
			
	};
} // namespace ft

#endif