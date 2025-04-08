/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_iterator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelkac <ambelkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 15:45:00 by amine             #+#    #+#             */
/*   Updated: 2022/08/11 22:12:52 by ambelkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../utils/iterator_traits.hpp"
#include "../utils/enable_if.hpp"
#include <cstddef>

namespace ft
{
	template <class Ite>
	class random_access_iterator : public iterator<random_access_iterator_tag, Ite>
	{
	public:
		typedef typename ft::iterator<random_access_iterator_tag, Ite>::value_type value_type;
		typedef typename ft::iterator<random_access_iterator_tag, Ite>::difference_type difference_type;
		typedef typename ft::iterator<random_access_iterator_tag, Ite>::pointer pointer;
		typedef typename ft::iterator<random_access_iterator_tag, Ite>::reference reference;
		typedef typename ft::iterator<random_access_iterator_tag, Ite>::iterator_category iterator_category;

		random_access_iterator() : _cur(0) {}

		random_access_iterator(pointer x) : _cur(x) {}

		random_access_iterator(const random_access_iterator &u) : _cur(u.base()) {}

		virtual ~random_access_iterator() {}

		random_access_iterator &operator=(const random_access_iterator &u)
		{
			if (this == &u)
				return *this;

			_cur = u.base();
			return *this;
		}

		pointer base() const
		{
			return _cur;
		}

		reference operator*() const
		{
			return *_cur;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		random_access_iterator &operator++()
		{
			++_cur;
			return *this;
		}

		random_access_iterator operator++(int)
		{
			random_access_iterator tmp(*this);
			++_cur;
			return tmp;
		}

		random_access_iterator &operator--()
		{
			--_cur;
			return *this;
		}

		random_access_iterator operator--(int)
		{
			random_access_iterator tmp(*this);
			--_cur;
			return tmp;
		}

		random_access_iterator operator+(difference_type n)
		{
			return random_access_iterator(_cur + n);
		}

		random_access_iterator &operator+=(difference_type n)
		{
			_cur += n;
			return *this;
		}

		random_access_iterator operator-(difference_type n)
		{
			return random_access_iterator(_cur - n);
		}

		random_access_iterator &operator-=(difference_type n)
		{
			_cur -= n;
			return *this;
		}

		reference operator[](difference_type n) const
		{
			return *(_cur + n);
		}

		// needed for conversion to a const_iterator
		operator random_access_iterator<const Ite>(void)
		{
			return random_access_iterator<const Ite>(_cur);
		}

	private:
		pointer _cur;
	};

	template <class Iterator>
	random_access_iterator<Iterator> operator+(int n, const random_access_iterator<Iterator> &x)
	{
		return random_access_iterator<Iterator>(x.base() + n);
	}

	template <class IteratorL, class IteratorR>
	int operator-(const random_access_iterator<IteratorL> &lhs, const random_access_iterator<IteratorR> &rhs)
	{
		return lhs.base() - rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator==(const random_access_iterator<IteratorL> &lhs,
					const random_access_iterator<IteratorR> &rhs)
	{
		return lhs.base() == rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator!=(const random_access_iterator<IteratorL> &lhs,
					const random_access_iterator<IteratorR> &rhs)
	{
		return lhs.base() != rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator<(const random_access_iterator<IteratorL> &lhs,
				   const random_access_iterator<IteratorR> &rhs)
	{
		return lhs.base() < rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator<=(const random_access_iterator<IteratorL> &lhs,
					const random_access_iterator<IteratorR> &rhs)
	{
		return lhs.base() <= rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator>(const random_access_iterator<IteratorL> &lhs,
				   const random_access_iterator<IteratorR> &rhs)
	{
		return lhs.base() > rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator>=(const random_access_iterator<IteratorL> &lhs,
					const random_access_iterator<IteratorR> &rhs)
	{
		return lhs.base() >= rhs.base();
	}

	/* Reverse Iterator */
	template <class Ite>
	class reverse_iterator : public iterator<typename iterator_traits<Ite>::iterator_category,
											 typename iterator_traits<Ite>::value_type,
											 typename iterator_traits<Ite>::difference_type,
											 typename iterator_traits<Ite>::pointer,
											 typename iterator_traits<Ite>::reference>
	{
	public:
		typedef Ite												iterator_type;
		typedef typename iterator_traits<Ite>::difference_type	difference_type;
		typedef typename iterator_traits<Ite>::reference		reference;
		typedef typename iterator_traits<Ite>::pointer			pointer;

		reverse_iterator() : _cur() {}

		explicit reverse_iterator(Ite x) : _cur(x) {}

		template <class U>
		reverse_iterator(const reverse_iterator<U> &u) : _cur(u.base()) {}

		template <class U>
		reverse_iterator &operator=(const reverse_iterator<U> &u)
		{
			if (*this == u)
				return *this;

			_cur = u.base();
			return *this;
		}

		Ite base() const
		{
			return _cur;
		}

		reference operator*() const
		{
			iterator_type cpy = _cur;
			return *--cpy;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		reverse_iterator &operator++()
		{
			--_cur;
			return *this;
		}

		reverse_iterator operator++(int)
		{
			reverse_iterator tmp(*this);
			--_cur;
			return tmp;
		}

		reverse_iterator &operator--()
		{
			++_cur;
			return *this;
		}

		reverse_iterator operator--(int)
		{
			reverse_iterator tmp(*this);
			++_cur;
			return tmp;
		}

		reverse_iterator operator+(difference_type n)
		{
			return reverse_iterator(_cur - n);
		}

		reverse_iterator &operator+=(difference_type n)
		{
			_cur -= n;
			return *this;
		}

		reverse_iterator operator-(difference_type n)
		{
			return reverse_iterator(_cur + n);
		}

		reverse_iterator &operator-=(difference_type n)
		{
			_cur += n;
			return *this;
		}

		reference operator[](difference_type n)
		{
			return (*(*this + n));
		}

		operator reverse_iterator<const Ite>(void)
		{
			return reverse_iterator<const Ite>(_cur);
		}

	protected:
		Ite _cur;
	};

	template <class _Ite>
	reverse_iterator<_Ite> operator+(long n, const reverse_iterator<_Ite> &x)
	{
		return reverse_iterator<_Ite>(x.base() - n);
	}

	template <class IteratorL, class IteratorR>
	long operator-(const reverse_iterator<IteratorL> &lhs, const reverse_iterator<IteratorR> &rhs)
	{
		return rhs.base() - lhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator==(const reverse_iterator<IteratorL> &lhs,
					const reverse_iterator<IteratorR> &rhs)
	{
		return lhs.base() == rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator!=(const reverse_iterator<IteratorL> &lhs,
					const reverse_iterator<IteratorR> &rhs)
	{
		return lhs.base() != rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator<(const reverse_iterator<IteratorL> &lhs,
				   const reverse_iterator<IteratorR> &rhs)
	{
		return lhs.base() > rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator<=(const reverse_iterator<IteratorL> &lhs,
					const reverse_iterator<IteratorR> &rhs)
	{
		return lhs.base() >= rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator>(const reverse_iterator<IteratorL> &lhs,
				   const reverse_iterator<IteratorR> &rhs)
	{
		return lhs.base() < rhs.base();
	}

	template <class IteratorL, class IteratorR>
	bool operator>=(const reverse_iterator<IteratorL> &lhs,
					const reverse_iterator<IteratorR> &rhs)
	{
		return lhs.base() <= rhs.base();
	}
}
