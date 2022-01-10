/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:13:15 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/21 20:03:27 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"

extern size_t	ft_query_len(char *s, char c)
{
	size_t	i;
	size_t	n;

	n = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			n++;
		if (n == 2)
			return (i);
		i++;
	}
	return (0);
}

extern char	*ft_cut(char *s, char **s_ptr, char c)
{
	char	*n_str;
	char	*quo;
	size_t	i;
	size_t	j;

	n_str = ft_calloc(sizeof(char), ft_strlen(s) + 10);
	i = 0;
	j = 0;
	while (s[i] && s[i] != c)
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			quo = s + i;
			ft_memcpy(n_str + i + j, quo, ft_query_len(quo, *quo) + 1);
			s += ft_query_len(quo, *quo) + 1;
			j = ft_strlen(n_str) - i;
			if (s[i] && c == ' ' && (s[i] == '<' || s[i] == '>'))
				break ;
			continue ;
		}
		n_str[i + j] = s[i];
		i++;
	}
	(*s_ptr) = s + i;
	return (n_str);
}

static void	cut_redir(char *str, char **s_ptr, char **tab, size_t *j)
{
	size_t	i;

	tab[(*j)] = ft_calloc(sizeof(char), ft_strlen(str) + 1);
	i = 0;
	while (str[i] && (str[i] == '<' || str[i] == '>'))
		i++;
	ft_memcpy(tab[(*j)++], str, i);
	(*s_ptr) = str + i;
	if (str[i] && str[i] != 32)
	{
		str += i;
		tab[(*j)] = ft_calloc(sizeof(char), ft_strlen(str) + 1);
		i += ft_strlen(str);
		ft_memcpy(tab[(*j)++], str, i);
		(*s_ptr) += ft_strlen(tab[*j - 1]);
	}
}

static char	**ft_handle_tab(const char *str, char c, char **tab)
{
	size_t	j;

	j = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str && *str != c)
		{
			if (c == '|' || (c == ' ' && *str != '<' && *str != '>'))
			{
				tab[j] = ft_cut((char *)str, (char **)&str, c);
				if (tab[j++] == NULL)
				{
					ft_free_d2(tab);
					return (NULL);
				}
			}
			if (c == ' ' && (*str == '<' || *str == '>'))
				cut_redir((char *)str, (char **)&str, tab, &j);
		}
		while (*str && *str != c)
			str++;
	}
	return (tab);
}

char	**ft_split_ptr(const char *s, char c, t_len ft_len)
{
	size_t	j;
	char	**tab;

	if (!s)
		return (NULL);
	j = ft_len((char *)s, c);
	tab = (char **)ft_calloc(sizeof(char *), j + 1);
	if (!tab)
		return (NULL);
	return (ft_handle_tab(s, c, tab));
}
