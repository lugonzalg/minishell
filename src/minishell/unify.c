/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unify.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 21:18:51 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/13 03:20:40 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "inc/get_next_line.h"
#include "inc/libft.h"
#include "inc/minishell.h"

extern void	cmd_size(t_child *child)
{
	int	pos;

	pos = 1;
	child->size[2] = child->size[1];
	while (child->info[pos])
	{
		if ((ft_strchr(child->info[pos - 1], INPUT)
				|| ft_strchr(child->info[pos - 1], OUTPUT))
			&& ft_strlen(child->info[pos - 1]) == 1)
		{
			*child->info[pos] = 0;
			*child->info[pos - 1] = 0;
			child->size[1] -= 2;
		}
		pos++;
	}
}

extern void	unify_fdio(t_child *child)
{
	int		fd;
	size_t	i;

	i = -1;
	while (child->info[++i])
	{
		if (ft_strlen(child->info[i]) == 1 && ft_strchr(child->info[i], OUTPUT))
		{
			fd = open(child->info[++i], O_RDWR | O_TRUNC | O_CREAT, 0644);
			close(child->fdpipe[child->id + 1][1]);
			child->fdpipe[child->id + 1][1] = fd;
		}
		if (ft_strlen(child->info[i]) == 1 && ft_strchr(child->info[i], INPUT))
		{
			fd = open(child->info[++i], O_RDONLY);
			close(child->fdpipe[child->id][0]);
			child->fdpipe[child->id][0] = fd;
		}
	}
}

void	unify_cmd(t_child *child)
{
	char	**temp;
	size_t	index;
	size_t	i;

	index = 0;
	i = 0;
	cmd_size(child);
	temp = (char **)ft_calloc(sizeof(char *), child->size[1] + 1);
	while (index < child->size[2])
	{
		if (*child->info[index])
			temp[i++] = ft_strdup(child->info[index]);
		index++;
	}
	index = -1;
	free_d2(child->info);
	child->info = temp;
}
