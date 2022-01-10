/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unify.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 21:18:51 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/21 20:10:45 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>

static void	ft_command_pos(t_prompt *p, t_child *child)
{
	int	i;

	i = -1;
	p->path = ft_setpath(p);
	if (!p->path && ft_putpath(child))
		return ;
	while (p->path[++i] && !child->path)
	{
		if (child->info[0] && !access(child->info[0], X_OK))
		{
			child->path = ft_strdup(child->info[0]);
			break ;
		}
		child->path = ft_strjoin(p->path[i], child->info[0]);
		if (child->path && !access(child->path, X_OK))
			break ;
		free(child->path);
		child->path = NULL;
	}
	ft_free_d2(p->path);
}

extern void	ft_check_redir(t_prompt *p, t_child *child)
{
	size_t	i;

	child->info = ft_split_ptr(p->d2_prompt[child->id],
			' ', ft_len_redir);
	i = -1;
	while (child->info[++i])
	{
		if (ft_strchr(child->info[i], '<') && !ft_strchr(child->info[i], '\'')
			&& !ft_strchr(child->info[i], '\"'))
			child->redir[0] = true;
		else if (ft_strchr(child->info[i], '>')
			&& !ft_strchr(child->info[i], '\'')
			&& !ft_strchr(child->info[i], '\"'))
			child->redir[1] = true;
	}
	ft_expand(p, child);
	child->size[1] = i;
	i = 0;
	if (child->redir[0] || child->redir[1])
		i = ft_unify_fdio(child);
	if (i)
		return ;
	ft_unify_cmd(p, child);
	if (!child->builtin || !ft_strncmp(child->info[0], "expr", 4))
		ft_command_pos(p, child);
}

static void	ft_cmd_size(t_child *child)
{
	int	pos;

	pos = 1;
	child->size[2] = child->size[1];
	while (child->info[0] && child->info[pos])
	{
		if ((ft_strchr(child->info[pos - 1], INPUT)
				|| ft_strchr(child->info[pos - 1], OUTPUT))
			&& ft_strlen(child->info[pos - 1]) < 3)
		{
			*child->info[pos] = 0;
			*child->info[pos - 1] = 0;
			child->size[1] -= 2;
		}
		pos++;
	}
}

static char	**ft_realloc(char **temp)
{
	int		size;
	char	**d2;
	int		index;

	size = 0;
	while (temp[size])
		size++;
	size += 1;
	d2 = (char **)ft_calloc(sizeof(char *), size + 1);
	d2[0] = ft_strdup("echo");
	size = 1;
	index = 0;
	while (temp[index])
	{
		d2[size] = ft_strdup(temp[index]);
		free(temp[index]);
		index++;
		size++;
	}
	d2[size] = NULL;
	free(temp);
	return (d2);
}

extern void	ft_unify_cmd(t_prompt *p, t_child *child)
{
	char	**temp;
	size_t	index;
	size_t	i;

	(void)p;
	index = 0;
	i = 0;
	ft_cmd_size(child);
	temp = (char **)ft_calloc(sizeof(char *), child->size[1] + 1);
	while (index < child->size[2] && child->info[index])
	{
		if (child->info[index] && *child->info[index])
			temp[i++] = ft_strdup(child->info[index]);
		index++;
	}
	if (child->echo)
		temp = ft_realloc(temp);
	ft_free_d2(child->info);
	child->info = temp;
}
