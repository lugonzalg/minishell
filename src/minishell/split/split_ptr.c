/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:13:15 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/02 22:33:40 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "inc/minishell.h"
#include "inc/libft.h"

static size_t	ft_query_len(char *s, char c)
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

extern char	*dquote_expand(char *str, t_prompt *p)
{
	static char	*n_str;
	char		*env;
	size_t		i;

	i = 0;
	n_str = ft_strchr(str, '$');
	while (ft_isalpha(n_str[i + 1]))
		i++;
	p->tmp = ft_substr(n_str + 1, 0, i);
	env = ft_gnl_query(p->envpath, p->tmp);
	free(p->tmp);
	p->tmp = ft_substr(env, i + 1, ft_strlen(env) - 2 - i);
	free(env);
	n_str = ft_calloc(sizeof(char), ft_strlen(p->tmp) + ft_strlen(str) + 100);
	i = ft_strchr(str, '$') - str;
	env = ft_memcpy(n_str, str, i);
	env = ft_memcpy(env + ft_strlen(env), p->tmp, ft_strlen(p->tmp));
	free(p->tmp);
	p->tmp = ft_strchr(str + i, '\'');
	env = ft_memcpy(env + ft_strlen(env), p->tmp, ft_strlen(p->tmp));
	free(str);
	if (ft_strchr(n_str, '$'))
		dquote_expand(n_str, p);
	return (n_str);
}

extern char	*ft_cut(char *s, char **s_ptr, char c, t_prompt *p)
{
	char	*n_str;
	char	*quo;
	size_t	i;
	size_t	j;

	n_str = ft_calloc(sizeof(char), ft_strlen(s) + 1);
	i = 0;
	j = 0;
	while (s[i] && s[i] != c)
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			quo = s + i;
			if (c == '|')
				ft_memcpy(n_str + i + j, quo, ft_query_len(quo, *quo) + 1);
			else
				ft_memcpy(n_str + i + j, quo + 1, ft_query_len(quo, *quo) - 1);
			s += ft_query_len(s + i, *quo) + 1;
			j = ft_strlen(n_str) - i;
			if (c == ' ' && *quo == '\"' && ft_strnstr(n_str, "\'$", 2048))
			{
				n_str = dquote_expand(n_str, p);
				j = ft_strlen(n_str) - i;
			}
			if (c == ' ' && (s[i] == '<' || s[i] == '>'))
				break ;
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
				quote = s;
				s += ft_query_len(quote, *quote);
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
		quote = s;
		s += ft_query_len(quote, *(quote));
		(*row)++;
	}
	if (*s == '<' || *s == '>')
	{
		while (*s == '<' || *s == '>')
			s++;
		if (*(s + 1) != 32)
			(*row)++;
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
			if (*s != 32)
				s++;
		}
	}
	return (row);
}

static void	cut_redir(char *str, char **s_ptr, char **tab, size_t *j)
{
	size_t	i;
	char	redir;

	redir = *str;
	tab[(*j)] = ft_calloc(sizeof(char), ft_strlen(str) + 1);
	i = 0;
	while (str[i] && str[i] == redir)
		i++;
	ft_memcpy(tab[(*j)++], str, i);
	if (str[i] && str[i] != 32)
	{
		str += i;
		tab[(*j)] = ft_calloc(sizeof(char), ft_strlen(str) + 1);
		i += ft_query_len(str, 32);
		ft_memcpy(tab[(*j)++], str, i);
	}
	(*s_ptr) = str + i;
}

static char	**ft_handle_tab(const char *str, char c, char **tab, t_prompt *p)
{
	size_t	j;

	j = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str != c)
		{
			tab[j] = ft_cut((char *)str, (char **)&str, c, p);
			if (tab[j] == NULL)
			{
				free_d2(tab);
				return (NULL);
			}
			j++;
			if (c == ' ' && (*str == '<' || *str == '>'))
				cut_redir((char *)str, (char **)&str, tab, &j);
		}
		while (*str && *str != c)
			str++;
	}
	return (tab);
}

char	**ft_split_ptr(const char *s, char c, t_len ft_len, t_prompt *p)
{
	size_t	j;
	char	**tab;

	if (!s)
		return (NULL);
	j = ft_len((char *)s, c);
	tab = (char **)ft_calloc(sizeof(char *), j + 1);
	if (!tab)
		return (NULL);
	return (ft_handle_tab(s, c, tab, p));
}
