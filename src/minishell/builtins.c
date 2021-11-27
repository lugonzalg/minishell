/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 19:58:59 by mikgarci          #+#    #+#             */
/*   Updated: 2021/11/27 21:51:56 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "inc/minishell.h"
#include "inc/libft.h"
#include "inc/ft_printf.h"
#include "inc/get_next_line.h"
#include <dirent.h>

extern void	showenv(t_prompt *p)
{
	char	*line;
	int		fd;

	fd = open(p->envpath, O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		printf("%s", line);
		free(line);
	}
	close(fd);
}

extern void	envinclude(t_child	*child, t_prompt *p)
{
	int	fd;
	int	i;

	fd = open(p->envpath, O_WRONLY | O_APPEND);
	i = -1;
	while (child->info[1][++i])
		write(fd, &child->info[1][i], 1);
	write(fd, "\n", 1);
	p->sizenv++;
	close(fd);
}

static void	deletenv_2(t_prompt *p)
{
	int		fd[2];
	char	*line;

	fd[0] = open(p->envpath, O_WRONLY | O_TRUNC);
	fd[1] = open(".envtemp", O_RDONLY);
	while (1)
	{
		line = get_next_line(fd[1]);
		if (!line)
			break ;
		write(fd[0], line, ft_strlen(line));
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	unlink(".envtemp");
}

extern void	deletenv(t_child	*child, t_prompt *p)
{
	int		fd[2];
	char	*line;

	fd[0] = open(p->envpath, O_RDONLY);
	fd[1] = open(".envtemp", O_WRONLY | O_CREAT, 0644);
	while (1)
	{
		line = get_next_line(fd[0]);
		if (!line)
			break ;
		if (ft_strncmp(child->info[1], line, ft_strlen(child->info[1]) - 1))
			write(fd[1], line, ft_strlen(line));
		else
			p->sizenv--;
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	deletenv_2(p);
}

extern void	ft_echo(t_child *child)
{
	size_t	i;
	bool	nl;
	int		fd;

	fd = 1;
	nl = true;
	i = 0;
	if (child->redir[1] || child->id < child->size[0] - 2)
		fd = child->fdpipe[child->id + 1][1];
	if (child->info[1] && !ft_strncmp(child->info[1], "-n", 3))
	{
		nl = false;
		i++;
	}
	while (child->info[++i])
	{
		write(fd, child->info[i], ft_strlen(child->info[i]));
		if (i < child->size[1] - 1)
			write(0, " ", 1);
	}
	if (nl)
		write(fd, "\n", 1);
}
