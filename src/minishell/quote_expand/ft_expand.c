/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 23:24:33 by lugonzal          #+#    #+#             */
/*   Updated: 2022/01/13 19:00:38 by lugonzal         ###   ########.fr       */
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

extern char	*ft_joinfr(t_prompt *p, char *str, size_t *j)
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

static void	ft_no_dollar_case(t_child *c, t_prompt *p, size_t i)
{
	c->info[i] = ft_quote_case(p, c->info[i]);
	if (!c->expand && i && ft_strlen(c->info[i]) == 0
		&& (ft_strchr(c->info[i - 1], '<')
			|| ft_strchr(c->info[i - 1], '>')))
		c->expand = ft_strdup(p->expand);
	else
	{
		free(p->expand);
		p->expand = NULL;
	}
}

extern void	ft_expand(t_prompt *p, t_child *c)
{
	size_t	i;	

	i = -1;
	while (c->info[++i])
	{
		if (ft_strnstr(c->info[i], "$?", 3))
		{
			free(c->info[i]);
			c->info[i] = ft_itoa(g_glob.error);
			if (ft_strnstr(c->info[0], "echo", 5))
				ft_go_exit(0);
		}
		else
			ft_no_dollar_case(c, p, i);
	}
	i = -1;
	while (c->info[++i])
			c->info[i] = ft_quote_clean(c->info[i]);
}
