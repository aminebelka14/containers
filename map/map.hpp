/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelkac <ambelkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:11:41 by amine             #+#    #+#             */
/*   Updated: 2022/08/12 23:10:19 by ambelkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "rb_tree.hpp"
#include <memory.h>
#include "map_iterator.hpp"
#include "../vector/vector_iterator.hpp"
#include "../utils/lexicographical_compare.hpp"
#include "pair.hpp"
#include <cstddef>

	#if __has_include(<sys/cdefs.h>)
	# include <sys/cdefs.h>
	#endif /* __has_include(<sys/cdefs.h>) */

	#if !defined(__unused)
	# define __unused __attribute__((unused))
	#endif /* !defined(__unused) */

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator< ft::pair<const Key, T> > >
	class map
	{
		public:
			typedef Key															key_type;
			typedef T															mapped_type;
			typedef ft::pair<const key_type, mapped_type>						value_type;
			typedef Compare														key_compare;
			typedef Allocator													allocator_type;
			typedef typename allocator_type::size_type							size_type;
			typedef typename allocator_type::difference_type					difference_type;
			typedef typename allocator_type::reference							reference;
			typedef typename allocator_type::const_reference					const_reference;
			typedef typename allocator_type::pointer							pointer;
			typedef typename allocator_type::const_pointer						const_pointer;
			typedef ft::tree_iterator<value_type, ft::Node<value_type> >		iterator;
			typedef ft::tree_iterator<const value_type, ft::Node<value_type> >	const_iterator;
			typedef ft::reverse_iterator<iterator>								reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>						const_reverse_iterator;

			class value_compare : public ft::binary_function<value_type, value_type, bool>
			{
				friend class map;

				protected:
					key_compare	comp;
					value_compare(key_compare c) : comp(c) {}

				public:
					bool	operator()(const value_type& x, const value_type& y) const
					{ return comp(x.first, y.first); }
			};

			//						CONSTRUCTOR
			// Default
			explicit				map(const key_compare& comp = key_compare(), __unused const allocator_type& alloc = allocator_type()) : _tree(value_compare(comp)) {}
			// Range
			template <class InputIte>
			map(InputIte first, InputIte last, const key_compare& comp = key_compare(), __unused const allocator_type& alloc = allocator_type()) : _tree(value_compare(comp))
			{ insert(first, last); }
			// Copy
			map(const map& m) : _tree(value_compare(key_compare())) { insert(m.begin(), m.end()); }
			
			~map()
			{
				clear();
				_tree.destroy_last();
			}

			map&					operator=(const map& m)
			{
				if (this == &m)
					return *this;
				
				this->~map();
				_tree = ft::RB_Tree<value_type, value_compare>(value_compare(key_compare()));
				insert(m.begin(), m.end());
				return *this;
			}
			//						ITERATOR
			iterator				begin() { return iterator(_tree.get_base(), _tree.get_end(), _tree.min()); }
			const_iterator			begin() const { return const_iterator(_tree.get_base(), _tree.get_end(), _tree.min()); }

			iterator				end() { return iterator(_tree.get_base(), _tree.get_end(), _tree.get_end()); }
			const_iterator			end() const { return const_iterator(_tree.get_base(), _tree.get_end(), _tree.get_end()); }

			reverse_iterator		rbegin() { return reverse_iterator(end()); }
			const_reverse_iterator	rbegin() const { return const_reverse_iterator(end()); }

			reverse_iterator		rend() { return reverse_iterator(begin()); }
			const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }

			//						CAPACITY
			bool					empty() const { return size() == 0; }
			size_type				size() const { return _tree.size(); }
			size_type				max_size() const { return _tree.max_size(); }

			//						ELEMENT ACCESS
			mapped_type&			operator[](const key_type& k)
			{
				insert(ft::make_pair(k, mapped_type()));
				return find(k)->second;
			}

			//						MODIFIER
			ft::pair<iterator, bool>insert(const value_type& v)
			{
				const bool	r = _tree.insert(v);
				return ft::make_pair(find(v.first), r);
			}

			iterator 				insert(__unused const_iterator position, const value_type& v)
			{
				insert(v);
				return find(v.first);
			}

			template <class Ite>
			void					insert(Ite first, Ite last)
			{
				for ( ; first != last; ++first)
					insert(*first);
			}

			void					erase(iterator position)
			{ erase(position->first); }
			
			size_type				erase(const key_type& k)
			{ return _tree.delete_node(ft::make_pair(k, mapped_type())); }

			void					erase(iterator first, iterator last)
			{
				while (first != last)
				{
					first = find(first->first);
					erase(first++);
				}
			}
			
			void					swap(map& m) { _tree.swap(m._tree); }

			void					clear() { return _tree.destroy(); }

			//						OBESERVER
			key_compare				key_comp() const { return key_compare(); }
			value_compare			value_comp() const { return value_compare(key_comp()); }

			//						OPERATOR
			iterator				find(const key_type& k)
			{
				return iterator(_tree.get_base(), _tree.get_end(), _tree.search(ft::make_pair(k, mapped_type())));
			}
			
			const_iterator			find(const key_type& k) const
			{
				return const_iterator(_tree.get_base(), _tree.get_end(), _tree.search(ft::make_pair(k, mapped_type())) );
			}
			
			size_type				count(const key_type& k) const { return find(k) != end(); }

			iterator				lower_bound(const key_type& k)
			{
				return iterator(_tree.get_base(), _tree.get_end(), _tree.lower_bound(ft::make_pair(k, mapped_type())));
			}
			
			const_iterator			lower_bound(const key_type& k) const
			{
				return const_iterator(_tree.get_base(), _tree.get_end(), _tree.lower_bound(ft::make_pair(k, mapped_type())));
			}
			
			iterator				upper_bound(const key_type& k)
			{
				return iterator(_tree.get_base(), _tree.get_end(), _tree.upper_bound(ft::make_pair(k, mapped_type())));
			}
			
			const_iterator			upper_bound(const key_type& k) const
			{
				return const_iterator(_tree.get_base(), _tree.get_end(), _tree.upper_bound(ft::make_pair(k, mapped_type())));
			}

			pair<iterator, iterator>equal_range(const key_type& k)
			{ return ft::make_pair(lower_bound(k), upper_bound(k)); }
			
			pair<const_iterator, const_iterator>equal_range(const key_type& k) const
			{ return ft::make_pair(lower_bound(k), upper_bound(k)); }
			
			//						ALLOCATOR
			allocator_type			get_allocator() const { return allocator_type(); }

			private:
				RB_Tree<value_type, value_compare>		_tree;
			
	};

	template <class Key, class T, class Compare, class Allocator>
	bool	operator==(const map<Key,T,Compare,Allocator>& x,
					const map<Key,T,Compare,Allocator>& y)
	{ return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin()); }

	template <class Key, class T, class Compare, class Allocator>
	bool	operator!=(const map<Key,T,Compare,Allocator>& x,
					const map<Key,T,Compare,Allocator>& y)
	{ return !(x == y); }

	template <class Key, class T, class Compare, class Allocator>
	bool	operator< (const map<Key,T,Compare,Allocator>& x,
					const map<Key,T,Compare,Allocator>& y)
	{ return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

	template <class Key, class T, class Compare, class Allocator>
	bool	operator<=(const map<Key,T,Compare,Allocator>& x,
					const map<Key,T,Compare,Allocator>& y)
	{ return !(y < x); }

	template <class Key, class T, class Compare, class Allocator>
	bool	operator> (const map<Key,T,Compare,Allocator>& x,
					const map<Key,T,Compare,Allocator>& y)
	{ return y < x; }

	template <class Key, class T, class Compare, class Allocator>
	bool	operator>=(const map<Key,T,Compare,Allocator>& x,
					const map<Key,T,Compare,Allocator>& y)
	{ return !(x < y); }

	template <class Key, class T, class Compare, class Allocator>
	void	swap(map<Key,T,Compare,Allocator>& x,
				map<Key,T,Compare,Allocator>& y)
	{ x.swap(y); }
}