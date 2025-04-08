/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amine <amine@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 20:41:16 by amine             #+#    #+#             */
/*   Updated: 2022/08/01 21:15:54 by amine            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft
{
	template <class T, class U>
	struct pair
	{
		typedef T		first_type;
		typedef U		second_type;

		//		Constructor
		pair() : first(), second() {}

		pair(const pair &src) : first(src.first), second(src.second) {}

		pair(const T &first, const U &second) : first(first), second(second) {}

		template<class V, class W>
		pair(const pair<V, W> &src) : first(src.first), second(src.second) {}

		pair	&operator=(const pair &src)
		{
			first = src.first;
			second = src.second;
			return (*this);
		}

		T				first;
		U				second;
	};

	template <class T, class U>
	ft::pair<T, U>		make_pair(T i, U j) { return (pair <T, U>(i, j)); };

		template <class T, class U>
	bool	operator== (const pair<T, U>& lhs, const pair<T,U>& rhs) { return (lhs.first == rhs.first) && (lhs.second == rhs.second); };
	
	template <class T, class U>
	bool	operator!= (const pair<T, U>& lhs, const pair<T,U>& rhs) { return !(lhs == rhs); };

	template <class T, class U>
	bool	operator< (const pair<T, U>& lhs, const pair<T,U>& rhs)
	{ return lhs.first < rhs.first || (rhs.first == lhs.first && lhs.second < rhs.second); };

	template <class T, class U>
	bool	operator> (const pair<T, U>& lhs, const pair<T,U>& rhs)
	{ return lhs.first > rhs.first || (rhs.first == lhs.first && lhs.second > rhs.second); };

	template <class T, class U>
	bool	operator<= (const pair<T, U>& lhs, const pair<T,U>& rhs) { return !(rhs < lhs); };

	template <class T, class U>
	bool	operator>= (const pair<T, U>& lhs, const pair<T,U>& rhs) { return !(lhs < rhs); };
};