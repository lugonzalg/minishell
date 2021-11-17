/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unify.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 21:18:51 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/16 21:39:29 by lugonzal         ###   ########.fr       */
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
			&& ft_strlen(child->info[pos - 1]) < 3)
		{
			*child->info[pos] = 0;
			*child->info[pos - 1] = 0;
			child->size[1] -= 2;
		}
		pos++;
	}
}

static void	here_doc(t_child *child, char *key)
{
	char	*line;
	size_t	size;

	while (1)
	{
		line = get_next_line(0);
		if (!line)
			break ;
		size = ft_strlen(line);
		line[size - 1] = 0;	
		if (!ft_strncmp(key, line, ft_strlen(line)))
		{
			free(line);
			break;
		}
		write(child->fdpipe[child->id + 1][1], line, ft_strlen(line));
		free(line);
	}
}

extern void	unify_fdio(t_child *child)
{
	int		fd;
	size_t	i;

	i = -1;
	while (child->info[++i])
	{
		if (ft_strchr(child->info[i], OUTPUT))
		{
			if (ft_strlen(child->info[i]) == 1)
				fd = open(child->info[++i], O_RDWR | O_TRUNC | O_CREAT, 0644);
			else if (ft_strlen(child->info[i]) == 2)
				fd = open(child->info[++i], O_RDWR | O_APPEND | O_CREAT, 0644);
			else
				exit(0); //SET FAILURE
			close(child->fdpipe[child->id + 1][1]);
			child->fdpipe[child->id + 1][1] = fd;
		}
		if (ft_strchr(child->info[i], INPUT))
		{
			if (ft_strlen(child->info[i]) == 1)
				fd = open(child->info[++i], O_RDONLY);
			else if (ft_strlen(child->info[i]) == 2)
			{
				here_doc(child, child->info[i + 1]);
				continue ;
			}
			else
				exit(0); //SET FAILURE
			close(child->fdpipe[child->id][0]);
			child->fdpipe[child->id][0] = fd;
		}
	}
}

static char	*expand_var(t_prompt *p, t_child *child, size_t i)
{
	char	*line;
	int		fd;
	char	*var;

	fd = open(p->envpath, O_RDONLY);
	line = ft_strtrim(child->info[i], "$\"");
	free(child->info[i]);
	child->info[i] = ft_strjoin(line, "=");;
	free(line);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (!ft_strncmp(line, child->info[i], ft_strlen(child->info[i])))
		{
			line[ft_strlen(line) - 1] = 0;
			close(fd);
			var = ft_strtrim(line, child->info[i]);
			free(line);
			return (var);
		}
		free(line);
	}
	close(fd);
	return (NULL);
}

void	unify_cmd(t_prompt *p, t_child *child)
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
		if (ft_strchr(child->info[index], '\''))
			temp[i++] = ft_strtrim(child->info[index], "\'");
		else if (ft_strchr(child->info[index], '$'))
				temp[i++] = expand_var(p, child, index);
		else if (*child->info[index])
			temp[i++] = ft_strdup(child->info[index]);
		index++;
	}
	free_d2(child->info);
	child->info = temp;
}
