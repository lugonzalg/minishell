/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 19:01:38 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/26 22:42:27 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "inc/libft.h"
#include "inc/minishell.h"

static char *ft_find_quote(const char *s)
{
	char	*s_quote;
	char	*d_quote;

	s_quote = ft_strchr(s, '\'');
	d_quote = ft_strchr(s, '\"');
	if (!s_quote && d_quote)
		return (d_quote);
	else if (s_quote && !d_quote)
		return (s_quote);
	else if (!s_quote && !d_quote)
		return (NULL);
	else if (s_quote < d_quote)
		return (s_quote);
	else if (d_quote < s_quote)
		return (d_quote);
	return (NULL);
}

static char *ft_find_redir(const char *s)
{
	char	*redir_in[2];
	char	*redir_out[2];

	redir_in[0] = ft_strchr(s, '<');
	redir_out[0] = ft_strchr(s, '>');
	redir_in[1] = ft_strchr(s + 1, '<');
	redir_out[1] = ft_strchr(s + 1, '>');
	if (redir_in[1] - redir_in[0] == 1)
		redir_in[0] = redir_in[1];
	if (redir_out[1] - redir_out[0] == 1)
		redir_out[0] = redir_out[1];
	if (!redir_in[0] && redir_out[0])
		return (redir_out[0]);
	else if (redir_in[0] && !redir_out[0])
		return (redir_in[0]);
	else if (!redir_in[0] && !redir_out[0])
		return (NULL);
	else if (redir_in[0] < redir_out[0])
		return (redir_in[0]);
	else if (redir_out[0] < redir_in[0])
		return (redir_out[0]);
	return (NULL);
}

extern char	*ft_cutp(const char *s, char **s_ptr)
{
	char	*quote;
	size_t		len;

	quote = ft_find_quote(s);
	quote = (char *)ft_quote(quote);
	if (quote)
		quote = ft_strchr(quote, '|');
	else
		quote = ft_strchr(s, '|');
	len = 0;
	if (quote)
		len = quote - s;
	else
	{
		quote = ft_strchr(s, 0);
		len = quote - s;
	}
	(*s_ptr) = quote;
	return (ft_substr(s, 0, len));
}

static void	check_limit(char **limit)
{
	if (!limit[0] && !limit[1] && !limit[2])
		return ;
	if (!limit[0])
		limit[0] = (char *)0xffffffffffffffff;
	if (!limit[1])
		limit[1] = (char *)0xffffffffffffffff;
	if (!limit[2])
		limit[2] = (char *)0xffffffffffffffff;
}

extern char	*ft_cut_redir(const char *s, char **s_ptr)
{
	char	*limit[4];
	size_t	len;
	size_t	i;

	limit[0] = ft_find_quote(s);
	limit[1] = ft_find_redir(s);
	limit[2] = ft_strchr(s, ' ');
	check_limit(limit);
	if (limit[2] < limit[1] && limit[2] < limit[0])
	{
		i = 2;
		limit[i]--;
	}
	else if (limit[0] < limit[1])
	{
		i = 0;
		limit[i] = ft_find_quote(s + 1);
	}
	else if (limit[1] < limit[0])
		i = 1;
	else
		return (ft_substr(s, 0, (size_t)(ft_strchr(s, 0) - s)));
	(*s_ptr) = limit[i];
	len = limit[i] - s + 1;
	limit[3] = ft_substr(s, 0, len);
	if (limit[i][1])
		(**s_ptr) = 32;
	return (limit[3]);
}

extern const char	*ft_quote(const char *s)
{
	if (!s)
		return (NULL);
	if (!ft_strncmp(s, S_QUOTE, 1))
		s = ft_strchr(s + 1, '\'');
	else if (!ft_strncmp(s, D_QUOTE, 1))
		s = ft_strchr(s + 1, '\"');
	return (s);
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
			s = ft_quote(s);
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
				s = ft_quote(s);
			s++;
		}
	}
	return (row);
}
