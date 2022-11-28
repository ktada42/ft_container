/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 20:04:49 by ktada             #+#    #+#             */
/*   Updated: 2022/11/28 18:24:45 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <algorithm>
#include <iostream>
#include <memory>
#include <limits>
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

#define size_t cant_use
	//constructors
		vector() : alloc(allocator_type())
		{
			init_ptr();
			assign(0);//todo?
		}
		
		explicit vector( const allocator_type& alloc ) : alloc(alloc)
		{
			init_ptr();
			assign(0);	
		}

		explicit vector(size_type count,
                 		const T& value = T(),
                 		const allocator_type& alloc = allocator_type()) : alloc(alloc)
		{
			init_ptr();
			assign(count, value);
		}
		
		template< class InputIt >
		vector( InputIt first, InputIt last,
				const allocator_type& alloc = allocator_type() ) : alloc(alloc)
		{
			init_ptr();
			assign(first, last);
		}
		
		
		vector( const ft::vector& other ) : alloc(allocator_type())
		{
			init_ptr();
			assign(other.begin(), other.end());
		}

		~vector()
		{
			destroy_allocated();
		}

		///////////////////////////
		//Member functions
		///////////////////////////
		
		vector& operator=( const vector& other )
		{
			init_ptr();
			assign(other.begin(), other.end());
		}
		
		
		//全ての要素がvalueになる
		void assign( size_type count, const T& value )
		{
			reserve_power2(count);
			std::fill(begin_ptr, end_ptr, value);
		}
		
		template< class InputIt >
		void assign( InputIt first, InputIt last )
		{
			resize(last - first);
			std::uninitialized_copy(first, last, begin_ptr);
		}
		
		allocator_type get_allocator() const
		{
			return alloc;
		}

		void swap( vector& other )
		{
			std::swap(begin_ptr, other.begin_ptr);
			std::swap(end_ptr, other.end_ptr);
			std::swap(alloc, other.alloc);
			std::swap(reserved_end, other.reserved_end);
		}
		
		//capが足りてるなら何もしない
		//延長された領域にvalueが代入される
		void resize(size_type size,  T value = T())
		{
			size_type pre_size = size();
			reserve_power2(size);
			std::fill(begin_ptr + pre_size, end_ptr, value);
		}

		void clear()
		{
			resize(0);
		}

		size_type capacity() const
		{
			return size_type(end_ptr - begin_ptr);
		}
		
		//ちょうどのサイズでメモリを確保する
		void reserve( size_type new_cap )
		{
			if (new_cap > max_size)
				throw_length_error();
			if (new_cap <= capacity())
				return;
			pointer new_begin_ptr = alloc.allocate(new_cap);
			pointer new_end_ptr = begin_ptr + new_cap;
			pointer new_reserved_end = new_end_ptr;
			//new_cap > capacity()なので、コピーが範囲外に行くことはない
			if (begin_ptr != nullptr)
			{
				std::uninitialized_copy(begin_ptr, end_ptr, new_begin_ptr);
				destroy_allocated();
			}
			begin_ptr = new_begin_ptr;
			end_ptr = new_end_ptr;
			reserved_end = new_end_ptr;
		}

		size_type max_size() const
		{
			return std::min(
                	static_cast<size_type>(std::numeric_limits<difference_type>::max()),
					alloc.max_size());
		}
		
		size_type size() const
		{
			return (size_type(end_ptr - begin_ptr));
		}
		
	private:
		const size_type max_size_;
		
		void destroy_allocated()
		{
			destroy_range(begin_ptr, end_ptr);
			alloc.deallocate(begin_ptr, capacity());
		}
		
		void destroy_range(pointer begin, pointer end)
		{
			while (begin != end)
				alloc.destroy(begin++);
		}

		void throw_length_error()
		{
			throw std::length_error("allocate");
		}
		
		size_type calc_cap_power2(size_type size)
		{
			if (size > max_size())
				throw_length_error();
			if (size == 0)
				return 0;
			size_type new_cap = 1;
			while(new_cap < size)
				new_cap <<= 1;
			if (new_cap > max_size())
				throw_length_error();
			return new_cap;
		}

		void reserve_power2(size_t new_size)
		{
			reserve(calc_cap_power2(new_size));
			end_ptr = begin_ptr + new_size;
		}

		void fill(const T &val)
		{
			for (size_t i = 0; i < count; i++)
			{
				/* code */
			}
			
		}
		
		

		void init_ptr()
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