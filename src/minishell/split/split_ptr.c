/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:13:15 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/27 22:28:09 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "inc/minishell.h"
#include "inc/libft.h"

extern void	ft_quote(const char *s, const char **n_str)
{
	char quote;

	if (!s)
		return ;
	quote = 0;
	if (*s == '\'')
		quote = '\'';
	else if (*s == '\"')
		quote = '\"';
	if (quote == '\'' && ft_strchr(s, '\'') < ft_strchr(s, '<'))
		quote = '<';
	else if (quote == '\"' && ft_strchr(s, '\"') < ft_strchr(s, '<'))
		quote = '>';
	while (quote)
	{
		(*n_str) = ft_strchr(s + 1, quote);
		if (!(*n_str))
		{
			(*n_str) = s;
			break;
		}
		else
			s = (*n_str);
	}
}

extern size_t	ft_lenp(const char *s, char c)
{
	size_t	row;

	row = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s != c && *s)
			row++;
		while (*s != c && *s)
		{
			ft_quote(s, &s);
			s++;
		}
	}
	return (row);
}

extern size_t	ft_len_redir(const char *s, char c)
{
	size_t	row;

	row = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s != c && *s)
			row++;
		while (*s != c && *s)
		{
			if (*s == '<' || *s == '>')
			{
				while (*s && (*s == '<' || *s == '>'))
					s++;
				if (*s && *s != 32)
					row++;
				break ;
			}
			if (*s && (*s == '\'' || *s == '\"'))
				ft_quote(s, &s);
			s++;
		}
	}
	return (row);
}

static char	**ft_handle_tab(const char *str, char c, char **tab, t_cut ft_cut)
{
	size_t	j;

	j = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str != c)
		{
			tab[j] = ft_cut(str, (char **)&str);
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

char	**ft_split_ptr(const char *s, char c, t_len ft_len, t_cut ft_cut)
{
	size_t	j;
	char	**tab;

	if (!s)
		return (NULL);
	j = ft_len(s, c);
	tab = (char **)ft_calloc(sizeof(char *), j + 1);
	if (!tab)
		return (NULL);
	return (ft_handle_tab(s, c, tab, ft_cut));
}
