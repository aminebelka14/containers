/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexicographical_compare.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelkac <ambelkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 20:41:00 by amine             #+#    #+#             */
/*   Updated: 2022/08/12 15:11:03 by ambelkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft
{
	template <class InputIte1, class InputIte2>
	bool lexicographical_compare(InputIte1 first1, InputIte1 last1, InputIte2 first2, InputIte2 last2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (first2 == last2 || *first2 < *first1)
				return false;
			else if (*first1 < *first2)
				return true;
		}
		return (first2 != last2);
	}

	template <class InputIte1, class InputIte2>
	bool	equal(InputIte1 first1, InputIte1 last1, InputIte2 first2)
	{
		for ( ; first1 != last1; ++first1, ++first2)
		{
			if (!(*first1 == *first2))
				return false;
		}
		return true;
	}
}