/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 23:24:33 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/13 20:55:35 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "inc/minishell.h"
#include "inc/libft.h"

static char	*ft_env_query(t_prompt *p, char *query, size_t *lq, size_t *lenv)
{
	char	*raw_env;
	char	*env;
	size_t	j;

	j = 0;
	while (query[j] == '_' || ft_isalnum(query[j]))
		j++;
	(*lenv) = j;
	env = ft_substr(query, 0, j);
	raw_env = ft_gnl_query(p->envpath, env);
	free(env);
	if (!raw_env)
		return (NULL);
	j = 0;
	while (raw_env[j] && raw_env[j] != '=')
		j++;
	(*lq) = ft_strlen(raw_env) - j - 2;
	env = ft_substr(raw_env, j + 1, (*lq));
	free(raw_env);
	return (env);
}

static char	*ft_joinfr(t_prompt *p, char *str, size_t *j)
{
	char	*n_str;
	char	*query;
	size_t	len[2];

	len[0] = 0;
	query = ft_env_query(p, str + (*j) + 1, &len[0], &len[1]);
	n_str = ft_calloc(sizeof(char), ft_strlen(query) + ft_strlen(str) + 1);
	ft_memcpy(n_str, str, (*j));
	if (query)
	{
		ft_memcpy(n_str + (*j), query, len[0]);
		free(query);
	}
	else
	{
		ft_memcpy(n_str + (*j), str + (*j) + len[1] + 1,
			ft_strlen(str + *j));
		free(str);
		(*j)--;
		return (n_str);
	}
	ft_memcpy(n_str + (*j) + len[0], str + (*j) + len[1] + 1,
		ft_strlen(str) - (*j));
	free(str);
	return (n_str);
}

static char	*ft_quote_case(t_prompt *p, char *str)
{
	char	squote;
	char	dquote;
	size_t	j;

	j = -1;
	squote = 0;
	dquote = 0;
	while (str[++j])
	{
		if (!squote && str[j] == '\'')
			squote = dquote + 1;
		else if (!dquote && str[j] == '\"')
			dquote = squote + 1;
		else if (((dquote == 1 && squote == 2) || !squote) && str[j] == '$'
			&& ft_isalpha(str[j + 1]))
			str = ft_joinfr(p, str, &j);
		else if (squote && str[j] == '\'')
			squote = 0;
		else if (dquote && str[j] == '\"')
			dquote = 0;
	}
	return (str);
}

static char	*ft_quote_clean(char *str)
{
	char	quote[2];
	size_t	len;
	size_t	j;

	j = -1;
	ft_memset(quote, 0, sizeof(char) * 2);
	while (str[++j])
	{
		if (!quote[0] && str[j] == '\'')
			quote[0] = quote[1] + 1;
		else if (!quote[1] && str[j] == '\"')
			quote[1] = quote[0] + 1;
		if (quote[1] || quote[0])
		{
			len = ft_query_len(&str[j], str[j]);
			ft_memcpy(&str[j], &str[j + 1], len + 1);
			j += len;
			ft_memcpy(&str[j - 1], &str[j + 1], ft_strlen(str + j) + 1);
			j -= 2;
			ft_memset(quote, 0, sizeof(char) * 2);
		}
	}
	return (str);
}

extern void	ft_expand(t_prompt *p, t_child *child)
{
	size_t	i;	

	i = -1;
	while (child->info[++i])
	{
		if (ft_strnstr(child->info[i], "$?", 3))
		{
			free(child->info[i]);
			child->info[i] = ft_itoa(g_glob.error);
			if (ft_strnstr(child->info[0], "echo", 5))
				ft_go_exit(0);
		}
		else
			child->info[i] = ft_quote_case(p, child->info[i]);
	}
	i = -1;
	while (child->info[++i])
			child->info[i] = ft_quote_clean(child->info[i]);
}
