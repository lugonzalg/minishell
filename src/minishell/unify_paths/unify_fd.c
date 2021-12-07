/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unify_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 03:25:37 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/07 21:47:19 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"

static void	resize_cat(t_child *child)
{
	char	**resize;
	size_t	j;
	size_t	i;

	resize = ft_calloc(sizeof(char *), child->size[1] + 1);
	if (ft_strchr(child->info[0], '<'))
	{
		j = 0;
		i = 1;
	}
	else
	{
		resize[0] = ft_strdup(child->info[0]);
		i = 2;
		j = 1;
	}
	while (child->info[++i])
		resize[j++] = ft_strdup(child->info[i]);
	child->redir[0] = true;
	free_d2(child->info);
	child->info = resize;
}

static int	here_doc(t_child *child, char *key)
{
	size_t	len;
	char	*line;
	int		fd;

	fd = open(".here_doc", O_RDWR | O_TRUNC | O_CREAT, 0644);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
			break ;
		len = ft_strlen(line);
		if (!ft_strncmp(key, line, len - 1 && len != 1))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
	fd = open(".here_doc", O_RDONLY);
	resize_cat(child);
	return (fd);
}

static void	ft_fdout(t_child *child, size_t i)
{
	int		fd;

	if (ft_strlen(child->info[i]) == 1)
		fd = open(child->info[++i], O_RDWR | O_TRUNC | O_CREAT, 0644);
	else if (ft_strlen(child->info[i]) == 2)
		fd = open(child->info[++i], O_RDWR | O_APPEND | O_CREAT, 0644);
	else
	{
		printf("minishell: syntax error near unexpected token `%s'\n", child->info[i]);
		return ;
	}
	close(child->fdpipe[child->id + 1][1]);
	child->fdpipe[child->id + 1][1] = fd;
}

static size_t	ft_fdin(t_child *child, size_t i)
{
	int		fd;

	if (ft_strlen(child->info[i]) == 1)
		fd = open(child->info[++i], O_RDONLY);
	else if (ft_strlen(child->info[i]) == 2)
		fd = here_doc(child, child->info[i-- + 1]);
	else
	{
		printf("minishell: syntax error near unexpected token `%s'\n", child->info[i]);
		return (i);
	}
	close(child->fdpipe[child->id][0]);
	child->fdpipe[child->id][0] = fd;
	return (i);
}

extern void	unify_fdio(t_child *child)
{
	size_t	i;

	i = -1;
	while (child->info[++i])
	{
		if (ft_strchr(child->info[i], OUTPUT))
			ft_fdout(child, i);
		if (ft_strchr(child->info[i], INPUT))
			i = ft_fdin(child, i);
	}
}
