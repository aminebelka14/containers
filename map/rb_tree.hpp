/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rb_tree.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelkac <ambelkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 20:36:09 by amine             #+#    #+#             */
/*   Updated: 2022/08/12 15:23:19 by ambelkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include "pair.hpp"

#define		BLACK_NODE 0
#define		RED_NODE 1

namespace ft
{
	template <class T>
	struct Node
	{
		public:
			typedef T			value_type;
			typedef size_t		size_type;

			Node(void) : parent(NULL), left(NULL), right(NULL), color(BLACK_NODE), val() {}

			Node(value_type const &val, size_type color = BLACK_NODE, Node *parent = NULL, Node *left = NULL, Node *right = NULL)
			: parent(parent), left(left), right(right), color(color), val(val) {}

			Node(Node const &src) : parent(src.parent), left(src.left), right(src.right), color(src.color), val(src.val) {}

			~Node() {}

			bool operator==(Node const &rhs) { return val == rhs.val; }

			Node &operator=(Node const &rhs)
			{
				if (this == &rhs)
					return *this;

				parent = rhs.parent;
				left = rhs.left;
				right = rhs.right;
				val = rhs.val;
				color = rhs.color;
				return *this;
			}

			Node				*parent;
			Node				*left;
			Node				*right;
			size_type			color;
			value_type			val;
	};

	template <class T, class Compare, class Node = Node<T>, class Allocator = std::allocator<Node> >
	class RB_Tree
	{
	
		public:
			typedef T											value_type;
			typedef Node										node_type;
			typedef Compare										value_compare;
			typedef Allocator									allocator_type;
			typedef typename allocator_type::difference_type	difference_type;
			typedef typename allocator_type::size_type			size_type;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;


		public:
			RB_Tree(value_compare const& cmp = value_compare()) :
				_comp(cmp)
			{
				_end = allocator_type().allocate(1);
				allocator_type().construct(_end, node_type());
				_base = _end;
			}

			~RB_Tree() {}

			size_type	size(pointer node) const
			{
				if (node == _end)
					return 0;
				return size(node->right) + size(node->left) + 1;
			}

			pointer		max(pointer node) const
			{
				if (node == NULL || node == _end)
					return _end;
				for ( ; node->right != _end; node = node->right);
				return node;
			}

			pointer		max() const { return max(_base); }

			pointer		min(pointer node) const
			{
				if (node == NULL || node == _end)
					return _end;
				for (; node->left != _end; node = node->left);
				return node;
			}

			pointer		min() const { return min(_base); }

			size_type	size() const { return size(_base); }
			size_type	max_size() const { return allocator_type().max_size(); }
			pointer		search(const value_type& key) const { return search_node(_base, key); }

			pointer		successor(pointer node) const
			{
				if (node->right != _end)
					return min(node->right);
				if (node->parent == NULL || node == max())
					return _end;
				
				pointer	tmp = node->parent;
				while (tmp != _end && node == tmp->right)
				{
					node = tmp;
					tmp = tmp->parent;
				}
				return tmp;
			}

			pointer		predecessor(pointer node) const
			{
				if (node->left != _end)
					return max(node->left);
				if (node->parent == NULL || node == min())
					return _end;

				pointer	tmp = node->parent;
				while (tmp != _end && node == tmp->left)
				{
					node = tmp;
					tmp = tmp->parent;
				}
				return tmp;
			}
		
			bool	insert(const value_type& key)
			{
				pointer	y = NULL;
				pointer	x = _base;
		
				pointer	s = allocator_type().allocate(1);
				allocator_type().construct(s, node_type(key, RED_NODE, NULL, _end, _end));

				while (x != _end)
				{
					y = x;
					if (_comp(s->val, x->val))
						x = x->left;
					else if (_comp(x->val, s->val))
						x = x->right;
					else
					{
						allocator_type().destroy(s);
						allocator_type().deallocate(s, 1);
						return false;
					}
				}

				s->parent = y;
				if (y == NULL)
					_base = s;
				else if (_comp(s->val, y->val))
					y->left = s;
				else
					y->right = s;

				if (s->parent == NULL)
				{
					s->color = BLACK_NODE;
					return true;
				}

				if (s->parent->parent == NULL)
					return true;
				balance_insertion(s);
				return true;
			}

			pointer		get_base() const { return _base; }
			pointer		get_end() const { return _end; }

			bool	delete_node(const value_type& key) { return __delete_node_wrapper(_base, key); }

			void		destroy()
			{
				_destroy(_base);
				_base = _end;
			}

			void		destroy_last()
			{
				allocator_type().destroy(_end);
				allocator_type().deallocate(_end, 1);
			}

			pointer		lower_bound(const value_type& v) const
			{
				pointer	p = min();

				for ( ; p != _end; p = successor(p))
				{
					if (!_comp(p->val, v))
						break ;
				}
				return p;
			}

			pointer		upper_bound(const value_type& v) const
			{
				pointer	p = min();

				for ( ; p != _end; p = successor(p))
				{
					if (_comp(v, p->val))
						break ;
				}
				return p;
			}

			void		swap(RB_Tree& ref)
			{
				pointer			tmp_root = _base;
				pointer			tmp_last = _end;

				_base = ref.get_base();
				_end = ref.get_end();
				ref._base = tmp_root;
				ref._end = tmp_last;
			}

		private:
			pointer		search_node(pointer node, const value_type& key) const
			{
				if (node == _end)
					return node;
				else if (_comp(key, node->val))
					return search_node(node->left, key);
				else if (_comp(node->val, key))
					return search_node(node->right, key);
				else
					return node;
			}

			void		rotate_left(pointer node)
			{
				pointer	tmp = node->right;
				
				node->right = tmp->left;
				if (tmp->left != _end)
					tmp->left->parent = node;
				tmp->parent = node->parent;
				if (node->parent == NULL)
					_base = tmp;
				else if (node == node->parent->left)
					node->parent->left = tmp;
				else
					node->parent->right = tmp;
				tmp->left = node;
				node->parent = tmp;
			}

			void		rotate_right(pointer node)
			{
				pointer	y = node->left;
				
				node->left = y->right;
				if (y->right != _end)
					y->right->parent = node;
				y->parent = node->parent;
				if (node->parent == NULL)
					_base = y;
				else if (node == node->parent->right)
					node->parent->right = y;
				else
					node->parent->left = y;
				y->right = node;
				node->parent = y;
			}

			void		balance_insertion(pointer node)
			{
				pointer	u;
				
				while (node->parent->color == RED_NODE)
				{
					if (node->parent == node->parent->parent->right)
					{
						u = node->parent->parent->left;
						if (u->color == RED_NODE)
						{
							u->color = BLACK_NODE;
							node->parent->color = BLACK_NODE;
							node->parent->parent->color = RED_NODE;
							node = node->parent->parent;
						}
						else
						{
							if (node == node->parent->left)
							{
								node = node->parent;
								rotate_right(node);
							}
							node->parent->color = BLACK_NODE;
							node->parent->parent->color = RED_NODE;
							rotate_left(node->parent->parent);
						}
					}
					else
					{
						u = node->parent->parent->right;
						if (u->color == RED_NODE)
						{
							u->color = BLACK_NODE;
							node->parent->color = BLACK_NODE;
							node->parent->parent->color = RED_NODE;
							node = node->parent->parent;	
						}
						else
						{
							if (node == node->parent->right)
							{
								node = node->parent;
								rotate_left(node);
							}
							node->parent->color = BLACK_NODE;
							node->parent->parent->color = RED_NODE;
							rotate_right(node->parent->parent);
						}
					}
					if (node == _base)
						break ;
				}
				_base->color = BLACK_NODE;
			}

			void		balance_erase(pointer node)
			{
				pointer	s;
				
				while (node != _base && node->color == BLACK_NODE)
				{
					if (node == node->parent->left)
					{
						s = node->parent->right;
						if (s->color == RED_NODE)
						{
							s->color = BLACK_NODE;
							node->parent->color = RED_NODE;
							rotate_left(node->parent);
							s = node->parent->right;
						}

						if (s->left->color == BLACK_NODE && s->right->color == BLACK_NODE)
						{
							s->color = RED_NODE;
							node = node->parent;
						}
						else
						{
							if (s->right->color == BLACK_NODE)
							{
								s->left->color = BLACK_NODE;
								s->color = RED_NODE;
								rotate_right(s);
								s = node->parent->right;
							} 
							s->color = node->parent->color;
							node->parent->color = BLACK_NODE;
							s->right->color = BLACK_NODE;
							rotate_left(node->parent);
							node = _base;
						}
					}
					else
					{
						s = node->parent->left;
						if (s->color == RED_NODE)
						{
							s->color = BLACK_NODE;
							node->parent->color = RED_NODE;
							rotate_right(node->parent);
							s = node->parent->left;
						}

						if (s->left->color == BLACK_NODE && s->right->color == BLACK_NODE)
						{
							s->color = RED_NODE;
							node = node->parent;
						}
						else
						{
							if (s->left->color == BLACK_NODE)
							{
								s->right->color = BLACK_NODE;
								s->color = RED_NODE;
								rotate_left(s);
								s = node->parent->left;
							}
							s->color = node->parent->color;
							node->parent->color = BLACK_NODE;
							s->left->color = BLACK_NODE;
							rotate_right(node->parent);
							node = _base;
						}
					} 
				}
				node->color = BLACK_NODE;
			}

			void		_rb_transplant(pointer i, pointer j)
			{
				if (i->parent == NULL)
					_base = j;
				else if (i == i->parent->left)
					i->parent->left = j;
				else
					i->parent->right = j;
				j->parent = i->parent;
			}

			bool	__delete_node_wrapper(pointer node, const value_type& key)
			{
				pointer	z = _end;
				pointer	x;
				pointer	y;

				while (node != _end)
				{
					if (_comp(node->val, key))
						node = node->right;
					else if (_comp(key, node->val))
						node = node->left;
					else
					{
						z = node;
						node = node->right;
					}
				}

				if (z == _end)
					return false;

				y = z;
				int	y_original_color = y->color;
				if (z->left == _end)
				{
					x = z->right;
					_rb_transplant(z, z->right);
				}
				else if (z->right == _end)
				{
					x = z->left;
					_rb_transplant(z, z->left);
				}
				else
				{
					y = min(z->right);
					y_original_color = y->color;
					x = y->right;
					
					if (y->parent == z)
						x->parent = y;
					else
					{
						_rb_transplant(y, y->right);
						y->right = z->right;
						y->right->parent = y;
					}
					_rb_transplant(z, y);
					y->left = z->left;
					y->left->parent = y;
					y->color = z->color;
				}

				allocator_type().destroy(z);
				allocator_type().deallocate(z, 1);
				
				if (y_original_color == BLACK_NODE)
					balance_erase(x);
				return true;
			}

			void		_destroy(pointer root)
			{
				if (root == _end)
					return ;
				_destroy(root->left);
				_destroy(root->right);
				allocator_type().destroy(root);
				allocator_type().deallocate(root, 1);
			}


		private:
			value_compare	_comp;
			pointer			_base;
			pointer			_end;
		
	};

	template <class Arg1, class Arg2, class Result>
	struct binary_function
	{
		typedef Arg1			first_argument_type;
		typedef Arg2			second_argument_type;
		typedef Result			result_type;
	};

	template <class T>
	struct less : binary_function <T, T, bool>
	{
		bool	operator()(const T& x, const T& y) const { return x < y; }
	};

	template <class T>
	struct equal_to : binary_function <T, T, bool>
	{
		bool	operator()(const T& x, const T& y) const { return x == y; }
	};

	template <class T>
	struct not_equal_to : binary_function <T, T, bool>
	{
		bool	operator()(const T& x, const T& y) const { return x != y; }
	};

	template <class T>
	struct greater : binary_function <T, T, bool>
	{
		bool	operator()(const T& x, const T& y) const { return x > y; }
	};

	template <class T>
	struct greater_equal : binary_function <T, T, bool>
	{
		bool	operator()(const T& x, const T& y) const { return x >= y; }
	};

	template <class T>
	struct less_equal : binary_function <T, T, bool>
	{
		bool	operator()(const T& x, const T& y) const { return x <= y; }
	};
}
