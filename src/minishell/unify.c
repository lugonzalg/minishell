/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unify.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 21:18:51 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/04 23:00:20 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "inc/get_next_line.h"
#include "inc/libft.h"
#include "inc/minishell.h"

void	cmd_size(t_child *child)
{
	int	pos;

	pos = child->size[2];
	while (child->info[pos])
	{
		if ((ft_strchr(child->info[pos + 1], INPUT)
				|| ft_strchr(child->info[pos + 1], OUTPUT))
				&& ft_strlen(child->info[pos + 1]) == 1)
			break ;
		pos++;
	}
	child->size[3] = pos;
}

void	unify_fdinput(t_child *child)
{
	int		fd[2];
	int		i;
	char	*line;

	i = -1;
	fd[0] = open(".tmp_IN", O_RDWR |O_TRUNC |O_CREAT, 0755);
	close(child->fdpipe[child->id][0]);
	child->fdpipe[child->id][0] = fd[0];
	while (child->info[++i])
	{
		if (ft_strlen(child->info[i]) == 1 && ft_strchr(child->info[i], INPUT))
		{
			fd[1] = open(child->info[++i], O_RDONLY);
			while (1)
			{
				line = get_next_line(fd[1]);
				if (!line)
					break ;
				write(fd[0], line, ft_strlen(line));
				if (!ft_strchr(line, 10))
					write(fd[0], NL, 1);
				free(line);
				close(fd[1]);
			}
		}
	}
	close(fd[0]);
}

void	unify_fdoutput(t_child *child)
{
	int	fd[2];
	int	i;
	int	j;

	fd[0] = open(".tmp_OUT", O_RDWR |O_TRUNC |O_CREAT, 0755);
	close(child->fdpipe[child->id + 1][1]);
	child->fdpipe[child->id + 1][1] = fd[0];
	child->fdout = (int *)malloc(sizeof(int) * child->redir[1]);
	i = -1;
	j = 0;
	while (child->info[++i])
	{
		if (ft_strlen(child->info[i]) == 1 && ft_strchr(child->info[i], OUTPUT))
		{
			fd[1] = open(child->info[++i], O_WRONLY |O_TRUNC |O_CREAT, 0755);
			child->fdout[j++] = fd[1];
			close(fd[1]);
		}
	}
	close(fd[0]);
}

void	unify_cmd(t_child *child)
{
	char	**temp;
	int		index;

	index = 0;
	cmd_size(child);
	temp = (char **) malloc(sizeof(char *) * child->size[3] - child->size[2] + 1);
	while (child->size[2] + index < child->size[3])
	{
		temp[index] = ft_strdup(child->info[index + child->size[2]]);
		index++;
	}
	index = -1;
	while (child->info[++index])
		free(child->info[index]);
	free(child->info);
	child->info = temp;
}
