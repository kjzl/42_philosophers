/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strsl_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:00:54 by kwurster          #+#    #+#             */
/*   Updated: 2024/07/29 13:44:10 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t  usizemin(size_t a, size_t b)
{
	if (a < b)
		return (a);
	return (b);
}

int32_t	ft_isspace(int32_t c)
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v'
		|| c == '\t' || c == '\f');
}

int32_t	ft_isdigit(int32_t c)
{
	return (c >= '0' && c <= '9');
}

/// @brief Moves a stringview.
/// @param s The stringview to move.
/// @param n The amount of characters to move by.
/// @return The moved stringview.
t_str_slice	strsl_move(t_str_slice s, size_t n)
{
	n = usizemin(n, s.len);
	s.str += n;
	s.len -= n;
	return (s);
}

/// @brief Moves a stringview.
/// @param s The stringview to move.
/// @param n The amount of characters to move by.
/// @return FALSE if the stringview's len or n are 0.
t_bool	strsl_move_inplace(t_str_slice *s, size_t n)
{
	n = usizemin(n, s->len);
	s->str += n;
	s->len -= n;
	return (n != 0);
}
