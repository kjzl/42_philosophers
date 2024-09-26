/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strsl_chr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:32:40 by kwurster          #+#    #+#             */
/*   Updated: 2024/06/28 02:22:27 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/// @brief Find the first occurence of a character in a string.
/// @param s The string to search in.
/// @param c The character to search for.
/// @return (Null)Pointer to the first occurence of c.
const char	*strsl_chr(t_str_slice s, char c)
{
	while (s.len > 0)
	{
		if (*s.str == c)
			return (s.str);
		s.str += 1;
		s.len -= 1;
	}
	return (0);
}

/// @brief Find the last occurence of a character in a string.
/// @param s The string to search in.
/// @param c The character to search for.
/// @return (Null)Pointer to the last occurence of c.
const char	*strsl_rchr(t_str_slice s, char c)
{
	while (s.len > 0)
	{
		if (s.str[s.len - 1] == c)
			return (s.str + s.len - 1);
		s.len--;
	}
	return (0);
}

static size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

/// @brief Creates a stringview from a cstr.
/// @param str The string to view.
/// @return The stringview.
t_str_slice	cstr_view(const char *str)
{
	if (str == 0)
		return ((t_str_slice){.len = 0, .str = 0});
	return ((t_str_slice){.len = ft_strlen(str), .str = str});
}

/// @brief Creates a stringview from a cstr.
/// @param str The string to view.
/// @param len The length of the stringview.
/// @return The stringview.
/// @warning The length of the stringview is not checked.
t_str_slice	cstr_slice(const char *str, size_t len)
{
	return ((t_str_slice){.len = len, .str = str});
}
