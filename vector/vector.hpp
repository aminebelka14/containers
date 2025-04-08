/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelkac <ambelkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 15:29:31 by amine             #+#    #+#             */
/*   Updated: 2022/08/13 14:31:17 by ambelkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <memory>
#include <exception>
#include <stdexcept>
#include "vector_iterator.hpp"
#include "../utils/enable_if.hpp"
#include "../utils/lexicographical_compare.hpp"

namespace ft
{
	template < class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef				T												value_type;
			typedef				Alloc											allocator_type;
			typedef	typename	allocator_type::reference						reference;
			typedef	typename	allocator_type::const_reference					const_reference;
			typedef	typename	allocator_type::pointer							pointer;
			typedef	typename	allocator_type::const_pointer					const_pointer;
			typedef				ft::random_access_iterator<value_type>			iterator;
			typedef				ft::random_access_iterator<const value_type>	const_iterator;
			typedef				ft::reverse_iterator<iterator>					reverse_iterator;
			typedef				ft::reverse_iterator<const_iterator>			const_reverse_iterator;
			typedef				ptrdiff_t										difference_type;
			typedef				size_t											size_type;

			//				CONSTRUCTOR
			// Default
			explicit	vector (const allocator_type &alloc = allocator_type()) : _size(0), _capacity(0), _array(NULL), _alloc(alloc) {}
			// Fill
			explicit	vector (size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type()) : _size(n), _capacity(n), _alloc(alloc)
			{
				_array = _alloc.allocate(n);
				for (size_type i = 0; i < n; ++i)
					_alloc.construct(&_array[i], val);
			}
			// Range
			template <class InputIterator>
			vector (InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type(),
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0) : _alloc(alloc)
			{
				_size = 0;
				_capacity = 0;
				for (; first != last ; ++first)
					push_back(*first);
				_capacity = _size;
			}
			// Copy
			vector (const vector &x) : _size(x._size), _capacity(x._size), _array(NULL), _alloc(x._alloc)
			{
				_array = _alloc.allocate(x._size);
				for (size_type i = 0; i < _size; ++i)
					_alloc.construct(&_array[i], x._array[i]);
			}

			//				DESTRUCTOR
			~vector(void)
			{
				for (size_type i = 0; i < _size; ++i)
					_alloc.destroy(&_array[i]);
				_alloc.deallocate(_array, _capacity);
			}

			//				OPERATOR OVERLOAD
			vector		&operator=(vector const & src)
			{
				reserve(src._size);
				for (size_type i = 0; i < src._size; ++i)
					_alloc.construct(&_array[i], src._array[i]);
				_size = src._size;
				return *this;
			}

			//				ITERATOR
			iterator			begin()				{ return iterator(_array); };
			iterator			end()				{ return (iterator(_array) + size()); };

			const_iterator		begin() const		{ return const_iterator(_array); };
			const_iterator		end() const			{ return (const_iterator(_array) + size()); };

			reverse_iterator rbegin()				{ return reverse_iterator(end()); }
			reverse_iterator rend()					{ return reverse_iterator(begin()); }

			const_reverse_iterator rbegin() const	{ return const_reverse_iterator(end())  ; }
			const_reverse_iterator rend() const		{ return const_reverse_iterator(begin()) ; }

			//				CAPACITY
			size_type			size(void) const		{ return _size; };
			
			size_type			max_size(void) const	{ return _alloc.max_size(); };
			
			void				resize (size_type n, value_type val = value_type())
			{
				if (n > _alloc.max_size())
					throw	std::length_error("Lenght Error.");
				while (_size > n)
					pop_back();
				if (_size + n > _capacity * 2)
					reserve(n);
				else if (_size + n > _capacity)
					reserve(_capacity * 2);
				while (_size < n)
					push_back(val);
			}

			size_type			capacity(void) const	{ return _capacity; };

			bool				empty(void) const		{ return !_size ? true : false; };

			void				reserve(size_type n)
			{
				pointer		n_arr;

				if (!n)
					return;
				if (n > _alloc.max_size())
					throw	std::length_error("Lenght Error.");
				if (n > _capacity)
				{
					n_arr = _alloc.allocate(n);
					for (size_type i = 0; i < _size; ++i)
					{
						_alloc.construct(&n_arr[i], _array[i]);
						_alloc.destroy(&_array[i]);
					}
					_alloc.deallocate(_array, _capacity);
					_array = n_arr;
					_capacity = n;
				}
			}

			//				ELEMENT ACCES
			reference			operator[] (size_type n)		{ return _array[n]; };
			const_reference		operator[] (size_type n) const	{ return _array[n]; };
			
			reference			at (size_type n)
			{
				if (n >= _size)
					throw std::out_of_range("Out of Range");
				return _array[n];
			};
			const_reference		at (size_type n) const
			{
				if (n >= _size)
					throw std::out_of_range("Out of Range.");
				return _array[n];
			};
			
			reference			front(void)			{ return _array[0]; };
			const_reference		front(void) const	{ return _array[0]; };

			reference			back(void)			{ return _array[_size - 1]; };
			const_reference		back(void) const	{ return _array[_size - 1]; };

			//				MODIFIER
			//	range
			template <class InputIterator>
			void	assign (InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
			{
				resize(0);
				for (; first != last; ++first)
					push_back(*first);
			}
			//	fill
			void	assign (size_type n, const value_type& val)
			{
				resize(0);
				_array = _alloc.allocate(n);
				_capacity = n;
				for (size_type i = 0; i < n; ++i)
					_alloc.construct(&_array[i], val);
				_size = n;
			};

			void	push_back (const value_type& val)
			{
				pointer		n_arr;
				if (_capacity && _capacity < _size + 1)
				{
					n_arr = _alloc.allocate(_capacity * 2);
					for (size_type i = 0; i < _size; ++i)
					{
						_alloc.construct(&n_arr[i], _array[i]);
						_alloc.destroy(&_array[i]);
					}
					_alloc.deallocate(_array, _capacity);
					_array = n_arr;
					_capacity = _capacity * 2;
				}
				if (_capacity == 0)
				{
					_capacity = 1;
					n_arr = _alloc.allocate(1);
					_array = n_arr;
				}
				_size += 1;
				_alloc.construct(&_array[_size - 1], val);
			}

			void	pop_back (void)
			{
				_size -= 1;
				_alloc.destroy(&_array[_size - 1]);
			}


			//single element
			iterator		insert (iterator position, const value_type& val)
			{
				if (!_size)
				{
					push_back(val);
					return (begin());
				}
				difference_type n = distance(begin(), position);

				insert(position, 1, val);
				return begin() + n;
			}
			// fill

			void insert (iterator position, size_type n, const value_type& val)
			{
				if (!n)
					return ;
				difference_type pos = distance(begin(), position);

				if (n + _size > _capacity)
				{
					if (_size + n > _size * 2)
						reserve(_size + n);
					else
						reserve(_size * 2);
				}
				for (difference_type i = _size; i > pos; i--)
				{
					_alloc.construct(&_array[i + n - 1], _array[i - 1]);
					_alloc.destroy(&_array[i - 1]);
				}
				for (size_type i = 0; i < n; i++)
				{
					_alloc.construct(&_array[pos + i], val);
				}
				_size += n;
				
			} 
			
			template<class InputIterator>
			void	insert(iterator position, InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = 0)
			{
				difference_type pos = distance(begin(), position);
				size_type n = distance(first, last);

				if (n + _size > _capacity)
				{
					if (_size + n > _size * 2)
						reserve(_size + n);
					else
						reserve(_size * 2);
				}
				for (difference_type i = _size; i > pos; i--)
				{
					_alloc.construct(&_array[i + n - 1], _array[i - 1]);
					_alloc.destroy(&_array[i - 1]);
				}
				for (size_type i = 0; first != last; first++, i++)
				{
					_alloc.construct(&_array[pos + i], *first);
				}
				_size += n;
			}

			iterator erase(iterator position)
			{
				size_t		idx = distance(begin(), position);
				size_t		idx_cpy = idx;

				_alloc.destroy(&_array[idx]);
				for (; idx < _size - 1; ++idx)
					_array[idx] = _array[idx + 1];
				resize(_size - 1);
				return iterator(&_array[idx_cpy]);
			}

			iterator erase(iterator first, iterator last)
			{
				iterator ret = first;

				for (; first != last; --last)
					erase(first);
				return ret;
			}

			void			swap(vector &src)
			{
				if (this == &src)
					return ;
				allocator_type	tmp_alloc = _alloc;
				pointer			tmp_array = _array;
				size_type		tmp_cap = _capacity;
				size_type		tmp_size = _size;

				_alloc = src._alloc;
				_array = src._array;
				_capacity = src._capacity;
				_size = src._size;
				src._capacity = tmp_cap;
				src._size = tmp_size;
				src._alloc = tmp_alloc;
				src._array = tmp_array;
			}

			void	clear (void)
			{
				for (size_t i = 0; i < _size; ++i)
					_alloc.destroy(&_array[i]);
				_size = 0;
			}

			template <class InputIt>
			difference_type		distance(InputIt it1, InputIt it2)
			{
				difference_type		count = 0;

				for (;it1 != it2; ++it1)
					++count;
				return count;
			}

		private :
			size_type						_size;
			size_type						_capacity; //Always double capacity on array size max reached and need to realloc
			pointer							_array;
			allocator_type					_alloc;
	};

	//	OPERATOR
	template <class T, class Alloc>
	bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return false;
		for (size_t i = 0; i < lhs.size(); ++i)
			if (lhs[i] != rhs[i])
				return false;
		return true;
	}

	template <class T, class Alloc>
	bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) { return !(lhs == rhs); }
	template <class T, class Alloc>
	bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) { return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }
	template <class T, class Alloc>
	bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) { return rhs < lhs; }
	template <class T, class Alloc>
	bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) { return !(lhs > rhs); }
	template <class T, class Alloc>
	bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) { return !(lhs < rhs); }
}