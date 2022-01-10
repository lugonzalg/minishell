/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 19:58:59 by mikgarci          #+#    #+#             */
/*   Updated: 2022/01/10 19:37:08 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

extern void	ft_showenv(t_prompt *p)
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

extern void	ft_envinclude(t_child	*c, t_prompt *p)
{
	int		fd[2];
	char	*line;

	if (ft_isdigit(c->info[1][0]) || ft_bad_indent(c))
		return (ft_env_fail(c));
	fd[0] = open(p->temppath, O_WRONLY | O_CREAT, 0644);
	fd[1] = open(p->envpath, O_RDONLY);
	while (1)
	{
		line = get_next_line(fd[1]);
		if (!ft_check_env(c, line, p))
		{
			write(fd[0], c->info[1], ft_strlen(c->info[1]));
			write(fd[0], "\n", 1);
		}
		else
			write(fd[0], line, ft_strlen(line));
		if (!line)
			break ;
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	ft_deletenv_2(p);
}

extern void	ft_deletenv(t_child *c, t_prompt *p)
{
	int		fd[2];
	char	*line;

	fd[0] = open(p->envpath, O_RDONLY);
	fd[1] = open(p->temppath, O_WRONLY | O_CREAT, 0644);
	while (1)
	{
		line = get_next_line(fd[0]);
		if (!line)
			break ;
		if (ft_strncmp(c->info[1], line, ft_strlen(c->info[1])))
			write(fd[1], line, ft_strlen(line));
		else
			p->sizenv--;
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	ft_deletenv_2(p);
}

extern void	ft_echo(t_child *c)
{
	size_t	i;
	bool	nl;
	int		fd;

	fd = 1;
	nl = true;
	i = 1;
	if (c->redir[1] || c->id < c->size[0] - 2)
		fd = c->fdpipe[c->id + 1][1];
	while (ft_nl_checker(c->info[i], &nl))
		i++;
	while (c->info[i])
	{
		write(fd, c->info[i], ft_strlen(c->info[i]));
		if (i < c->size[1] - 1)
			write(fd, " ", 1);
		i++;
	}
	if (nl)
		write(fd, "\n", 1);
}

extern int	ft_changedir(t_child *c, t_prompt *p)
{
	if (c->size[1] == 1)
	{
		chdir(p->home);
		ft_changepwd(p->home, p);
		return (0);
	}
	if (ft_changediraux(c, p) != -1)
		return (0);
	if (chdir(c->info[1]))
	{
		printf("minishell: cd: %s: No such file or directory\n", c->info[1]);
		ft_go_exit(1);
		return (1);
	}
	ft_putpwd(c->info[1], p);
	return (0);
}
