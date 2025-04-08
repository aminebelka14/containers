/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_iterator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelkac <ambelkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 17:46:37 by amine             #+#    #+#             */
/*   Updated: 2022/08/12 13:38:15 by ambelkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <cstddef>
#include "../utils/iterator_traits.hpp"

namespace ft
{
	template <class T, class Node>
	class tree_iterator : public iterator<bidirectional_iterator_tag, T>
	{
	public:
		typedef T													value_type;
		typedef iterator<bidirectional_iterator_tag, value_type>	iterator_type;
		typedef typename iterator_type::iterator_category			iterator_category;
		typedef typename iterator_type::difference_type				difference_type;
		typedef typename iterator_type::pointer						pointer;
		typedef typename iterator_type::reference					reference;
		typedef Node												node_type;
		typedef Node												*node_pointer;

		tree_iterator(void) : _base(NULL), _end(NULL), _cur(NULL) {}
		
		tree_iterator(node_pointer const& begin, node_pointer const& end, node_pointer const& current) : _base(begin), _end(end), _cur(current) {}

		tree_iterator(tree_iterator const& src) { *this = src; }

		tree_iterator&		operator=(const tree_iterator& src)
		{
			if (this == &src)
				return *this;
			
			_base = src._base;
			_end = src._end;
			_cur = src._cur;
			return *this;
		}
		
		~tree_iterator() {}

		node_pointer		base(void) const
		{
			return _cur;
		}

		reference			operator*() const
		{
			return _cur->val;
		}

		pointer				operator->() const
		{
			return &(operator*());
		}

		tree_iterator&		operator++()
		{
			if (_cur == max(_base))
			{
				_cur = _end;
				return *this;
			}
			else if (_cur == _end)
			{
				_cur = NULL;
				return *this;
			}
			_cur = successor(_cur);
			return *this;
		}

		tree_iterator		operator++(int)
		{
			tree_iterator	tmp(*this);
			operator++();
			return tmp;
		}

		tree_iterator&		operator--()
		{
			if (_cur == _end)
			{
				_cur = max(_base);
				return *this;
			}
			_cur = predecessor(_cur);
			return *this;
		}

		tree_iterator		operator--(int)
		{
			tree_iterator	tmp(*this);
			operator--();
			return tmp;
		}

		operator			tree_iterator<const T, Node> (void)
		{
			return tree_iterator<const T, Node>(_base, _end, _cur);
		}

	private:
		node_pointer	min(node_pointer node)
		{
			for ( ; node->left != _end; node = node->left);
			return node;
		}

		node_pointer	max(node_pointer node)
		{
			for ( ; node->right != _end; node = node->right);
			return node;
		}

		node_pointer	successor(node_pointer node)
		{
			if (node->right != _end)
				return min(node->right);

			node_pointer	tmp = node->parent;
			while (tmp != _end && node == tmp->right)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}

		node_pointer	predecessor(node_pointer node)
		{
			if (node->left != _end)
				return max(node->left);

			node_pointer	tmp = node->parent;
			while (tmp != _end && node == tmp->left)
			{
				node = tmp;
				tmp = tmp->parent;
			}
			return tmp;
		}

		node_pointer	_base;
		node_pointer	_end;
		node_pointer	_cur;

	};

template <class IteratorL, class IteratorR, class _Node>
bool	operator==(tree_iterator<IteratorL, _Node> const& x, tree_iterator<IteratorR, _Node> const& y)
{ return x.base() == y.base(); }

template <class IteratorL, class IteratorR, class _Node>
bool	operator!=(tree_iterator<IteratorL, _Node> const& x, tree_iterator<IteratorR, _Node> const& y)
{ return x.base() != y.base(); }
}