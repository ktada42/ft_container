/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 20:04:49 by ktada             #+#    #+#             */
/*   Updated: 2022/11/28 23:11:24 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <algorithm>
#include <iostream>
#include <memory>
#include <limits>
#include <exception>
#include <stdexcept> 
#include "type_traits.hpp"

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
		vector() : alloc(allocator_type()), max_size_(calc_max_size())
		{
			init_ptr();
			assign(0);//todo?
		}
		
		explicit vector( const allocator_type& alloc ) : alloc(alloc), max_size_(calc_max_size())
		{
			init_ptr();
			assign(0);	
		}

		explicit vector(size_type count,
                 		const T& value = T(),
                 		const allocator_type& alloc = allocator_type()) : alloc(alloc), max_size_(calc_max_size())
		{
			init_ptr();
			assign(count, value);
		}
		
		template< class InputIt >
		vector( InputIt first, InputIt last,
				const allocator_type& alloc = allocator_type() ) : alloc(alloc), max_size_(calc_max_size())
		{
			init_ptr();
			assign(first, last);
		}
		
		
		vector( const ft::vector& other ) : alloc(allocator_type()), max_size_(calc_max_size())
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
			max_size_ = calc_max_size();
			assign(other.begin(), other.end());
		}
		
		
		//全ての要素がvalueになる
		void assign( size_type count, const T& value )
		{
			destroy_range(begin(), resize_with_not_fill(count));
			std::fill(begin(), end(), value);
		}
		
		template< class InputIt >
		void assign( InputIt first, InputIt last )
		{
			destroy_range(begin(), resize_with_not_fill(last - first));
			std::uninitialized_copy(first, last, begin());
		}
		
		allocator_type get_allocator() const
		{
			return alloc;
		}

		
		//////////////////
		//Element access
		//////////////////
		reference at( size_type pos )
		{
			chek_out_of_range(pos);
			return operator[](pos);
		}

		const_reference at( size_type pos ) const
		{
			check_out_of_range(pos);
			return operator[](pos);
		}

		reference operator[]( size_type pos )
		{
			return begin()[pos];
		}

		const_reference operator[]( size_type pos ) const
		{
			return begin()[pos];
		}

		reference front()
		{
			return operator[](0);	
		}

		const_reference front() const
		{
			return operator[](0);	
		}

		reference back()
		{
			return operator[](size() - 1);	
		}

		const_reference back() const
		{
			return operator[](size() - 1);	
		}

		value_type* data()
		{
			return begin_ptr;
		}
		
		const value_type* data() const
		{
			return begin_ptr;
		}


		/////////////////
		//iterators
		/////////////////
		iterator begin()
		{
			return begin_ptr;
		}
		
		const_iterator begin() const
		{
			return begin_ptr;
		}

		iterator end()
		{
			return end_ptr;
		}

		const_iterator end() const
		{
			return end_ptr;
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end_ptr);
		}

		const_reverse_iterator rbegin() const
		{
			return reverse_iterator(end_ptr);
		}
		
		reverse_iterator rend()
		{
			return reverse_iterator(begin_ptr);
		}

		const_reverse_iterator rend() const
		{
			return reverse_iterator(begin_ptr);
		}

		/////////////////
		//Capacity
		/////////////////
		bool empty() const
		{
			return size() == 0;
		}
		
		size_type size() const
		{
			if (begin_ptr == NULL)
				return 0;
			return (size_type(end_ptr - begin_ptr));
		}
		
		size_type max_size() const
		{
			return max_size_;
		}
		
		//ちょうどのサイズでメモリを確保する
		void reserve( size_type new_cap )
		{
			check_capacity_size(new_cap);
			if (new_cap <= capacity())
				return;
			pointer new_begin_ptr = alloc.allocate(new_cap);
			pointer new_end_ptr = new_begin_ptr + size();
			pointer new_reserved_end = new_begin_ptr + new_cap;
			if (begin_ptr != NULL)
			{
				//new_cap > capacity()なので、コピーが範囲外に行くことはない
				std::uninitialized_copy(begin_ptr, end_ptr, new_begin_ptr);
				destroy_allocated();
			}
			begin_ptr = new_begin_ptr;
			reserved_end = new_reserved_end;
		}
		
		size_type capacity() const
		{
			if (begin_ptr == NULL)
				return 0;
			return size_type(end_ptr - begin_ptr);
		}
		
		/////////////////////
		//modifiers
		/////////////////////
		void clear()
		{
			resize(0);
		}
		
		iterator insert( const_iterator pos, const T& value )
		{
			insert(pos, 1, value);
		}
		
		iterator insert( const_iterator pos, size_type count, const T& value )
		{
			size_type pos_idx = pos - begin();
			resize(size() + count);
			move(it(pos_idx), end(), it(pos_idx + count) );
			std::fill(it(pos_idx), it(pos_idx + count), value);
			return insert_pos;
		}
		
		template< class InputIt >
		typename ft::enable_if<!is_integral<InputIt>::value, iterator>::value_type
		insert( const_iterator pos, InputIt first, InputIt last )
		{
			size_type pos_idx = idx(pos);
			size_type first_idx = idx(first);
			size_type last_idx = idx(last);
			resize(size() + (last - first));
			move(it(pos_idx), end(), it(pos_idx) + count);
			std::copy(it(first_idx), it(last_idx), it(pos_idx));
			return insert_pos;
		}

		iterator erase( iterator pos )
		{
			return erase(pos, pos + 1);
		}

		iterator erase( iterator first, iterator last )
		{
			size_t	new_size = size() - (last - first);
			iterator insert_pos = move(last, end(), first);
			//moveによりlast以降はdestroyされているので、resizeを呼ぶ必要はない
			end_ptr = begin() + new_size;
			return insert_pos;
		}

		void push_back( const T& value )
		{
			resize(size() + 1);
			operator[](size() - 1) = value;
		}

		//本家が空の時は未定義なのでそのまま返す
		void pop_back()
		{
			if (empty())
				throw std::exception("can't pop_back when empty elements");
			resize(size() - 1);	
		}
		
		//capが足りてるなら何もしない
		//延長された領域にvalueが代入される
		void resize(size_type size,  T value = T())
		{
			std::fill(resize_with_not_fill(size), end(), value);
		}
		
		void swap( vector& other )
		{
			std::swap(begin_ptr, other.begin_ptr);
			std::swap(end_ptr, other.end_ptr);
			std::swap(alloc, other.alloc);
			std::swap(reserved_end, other.reserved_end);
		}

		/////////////////////
		//operator
		/////////////////////
		
	private:
		const size_type max_size_;

		//begin() + sizeを返す
		//std::fill(resize_with_not_fill(), end(), val)で
		//通常のresizeとして使える
		iterator resize_with_not_fill(size_type size_)
		{
			if (size() <= size_)
				reserve_power2(size_);
			else
				destroy_range(it(size_), end_ptr);
			end_ptr = begin_ptr + size_;
			return it(size_);
		}
		
		size_type idx(const iterator &it)
		{
			return it - begin();
		}

		iterator it(size_type idx)
		{
			return begin() + idx;
		}

		void move(iterator to, iteraot from)
		{
			alloc.destroy(to);
			*to = *from;
			alloc.destroy(from);
		}

		void move(iterator from_begin, iterator from_end, iterator to_begin)
		{
			size_type move_cnt = from_end - from_begin;
			if (from_begin < to_begin)
			{
				iterator to_end = to_begin + move_cnt;
				for (size_t i = 0; i < move_cnt; i++)
					move(to_end - i - i, from_end - i - 1);
			}
			else if (from_begin > to_begin)
			{
				for (size_t i = 0; i < move_cnt; i++)
					move(to_begin + i, from_begin + i);
			}
		}
		
		void	calc_max_size()
		{
			return std::min(static_cast<size_type>(
								std::numeric_limits<difference_type>::max() / sizeof(value_type)),
								alloc_.max_size());
		}
		
		void check_out_of_range(size_type pos)
		{
			if (pos >= size())
				std::out_of_range();
		}
		
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

		void check_capacity_size(size_type cap)
		{
			if (cap > max_size)
				throw std::length_error("allocate");
		}
		
		size_type calc_cap_power2(size_type size)
		{
			check_capacity_size(size);
			if (size == 0)
				return 0;
			size_type new_cap = 1;
			while(new_cap < size)
				new_cap <<= 1;
			check_capacity_size(new_cap);
			return new_cap;
		}

		void reserve_power2(size_t new_size)
		{
			reserve(calc_cap_power2(new_size));
		}

		void init_ptr()
		{
			begin_ptr = NULL;
			end_ptr = NULL;
			reserved_end = NULL;
		}
	private:
		size_type		max_size_;
	protected:
		pointer			begin_ptr;
		pointer			end_ptr;
		pointer			reserved_end;
		allocator_type	alloc;
			
	};
} // namespace ft

#endif