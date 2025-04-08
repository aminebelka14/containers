/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amine <amine@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 18:16:51 by amine             #+#    #+#             */
/*   Updated: 2022/06/15 18:45:18 by amine            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../vector/vector.hpp"

namespace ft
{
	template <class T, class Container = vector<T> >
	class stack
	{
		public:
			typedef		T				value_type;
			typedef		Container		container_type;
			typedef		size_t			size_type;

			explicit	stack(const container_type &container = container_type()) { _container = container; }

			~stack() {};

			bool		empty() const { return _container.empty(); }

			size_type	size() const { return _container.size(); }

			value_type	&top() { return _container.back(); }

			value_type	&top() const { return _container.back(); }

			void		push(const value_type &val) { _container.push_back(val); }

			void		pop() { _container.pop_back(); }

			bool		operator==(const stack &rhs) const { return _container == rhs._container; };

			bool		operator!=(const stack &rhs) const { return _container != rhs._container; };

			bool		operator<(const stack &rhs) const { return _container < rhs._container; };

			bool		operator<=(const stack &rhs) const { return _container <= rhs._container; };

			bool		operator>(const stack &rhs) const { return _container > rhs._container; };

			bool		operator>=(const stack &rhs) const { return _container >= rhs._container; };


		protected:
			container_type		_container;
	};
}