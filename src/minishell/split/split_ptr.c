/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:13:15 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/28 02:09:18 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "inc/minishell.h"
#include "inc/libft.h"

static size_t	ft_query_len(char *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (0);
}

extern char	*ft_cut(char *s, char **s_ptr, char c, size_t on)
{
	char	*n_str;
	char	*quote;
	size_t	i;
	size_t	j;

	n_str = ft_calloc(sizeof(char), ft_strlen(s));
	i = 0;
	j = 0;
	while (s[i] && s[i] != c)
	{
		if (s[i] == '\"' || s[i] == '\"')
		{
			quote = s + i + 1 - on;
			ft_memcpy(n_str + i + j, quote, ft_query_len(quote + on, *(quote - 1 + on)) + (on * 2));
			s += ft_query_len(quote + on, *(quote - 1 + on)) + 2;
			j += ft_query_len(quote + on, *(quote - 1 + on)) + (on * 2);
			continue ;
		}
		n_str[i + j] = s[i];
		i++;
	}
	(*s_ptr) = s + i;
	return (n_str);
}

extern size_t	ft_lenp(char *s, char c)
{
	size_t	row;
	char	*quote;

	row = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
			row++;
		while (*s && *s != c)
		{
			if (*s == '\"' || *s == '\"')
			{
				quote = s + 1;
				s += ft_query_len(quote, *(quote - 1)) + 1;
			}
			s++;
		}
	}
	return (row);
}

static char	*ft_delimit(char *s, size_t *row)
{
	char	*quote;

	if (*s == '\"' || *s == '\"')
	{
		quote = s + 1;
		s += ft_query_len(quote, *(quote - 1)) + 1;
	}
	if (*s == '<' || *s == '>')
	{
		quote = s + 1;
		s += ft_query_len(quote, *(quote - 1)) + 1;
		if (*s + 1)
			row++;
	}
	return (s);
}

extern size_t	ft_len_redir(char *s, char c)
{
	size_t	row;

	row = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
			row++;
		while (*s && *s != c)
		{
			s = ft_delimit(s, &row);
			s++;
		}
	}
	return (row);
}

static char	**ft_handle_tab(const char *str, char c, char **tab, size_t on)
{
	size_t	j;

	j = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str != c)
		{
			tab[j] = ft_cut((char *)str, (char **)&str, c, on);
			if (tab[j] == NULL)
			{
				free_d2(tab);
				return (NULL);
			}
			j++;
		}
		while (*str && *str != c)
			str++;
	}
	return (tab);
}

char	**ft_split_ptr(const char *s, char c, t_len ft_len, size_t on)
{
	size_t	j;
	char	**tab;

	if (!s)
		return (NULL);
	j = ft_len((char *)s, c);
	tab = (char **)ft_calloc(sizeof(char *), j + 1);
	if (!tab)
		return (NULL);
	return (ft_handle_tab(s, c, tab, on));
}
