/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 19:58:59 by mikgarci          #+#    #+#             */
/*   Updated: 2021/11/17 18:33:29 by mikgarci         ###   ########.fr       */
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

#define PATH_MAX 4096

int	ft_checkbuiltins(char *str, t_prompt *p)
{
	int		fd;
	char	*line;
	size_t	size;	

	fd = open(p->builtpath, O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		size = ft_strlen(line);
		line[size - 1] = 0;
		if (!ft_strncmp(str, line, size))
		{
			free(line);
			close(fd);
			return (1);
		}
		free(line);
	}
	close(fd);
	return (0);
}

static void	showenv(t_prompt *p)
{
	char	*line;
	int		fd;

	printf("%s\n", p->envpath);
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

void	envinclude(t_child	*child, t_prompt *p)
{
	int	fd;
	int	i;

	fd = open(p->envpath, O_WRONLY | O_APPEND);
	i = -1;
	while (child->info[1][++i])
		write(fd, &child->info[1][i], 1);
	write(fd, "\n", 1);
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

void	deletenv(t_child	*child, t_prompt *p)
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
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	deletenv_2(p);
}

static void	ft_echo(t_child *child)
{
	size_t	i;
	bool	nl;

	nl = true;
	i = 0;
	if (child->info[1] && !ft_strncmp(child->info[1], "-n", 3))
	{
		nl = false;
		i++;
	}
	while (child->info[++i])
	{
		write(0, child->info[i], ft_strlen(child->info[i]));
		if (i < child->size[1] - 1)
			write(0, " ", 1);
	}
	if (nl)
		write(child->fdpipe[child->id + 1][1], "\n", 1);
}

void	ft_builtins(t_child *child, t_prompt *p)
{
	char	pwd[PATH_MAX];

	if (!ft_strncmp(child->info[0], "pwd", sizeof("pwd")))
	{
		if (getcwd(pwd, sizeof(pwd)) != NULL)
			printf("%s\n", pwd);
	}
	else if (!ft_strncmp(child->info[0], "cd", sizeof("cd")))
		chdir(child->info[1]);
	else if (!ft_strncmp(child->info[0], "env", sizeof("env")))
		showenv(p);
	else if (!ft_strncmp(child->info[0], "export", sizeof("export"))
			&& ft_strchr(child->info[1], '='))
		envinclude(child, p);
	else if (!ft_strncmp(child->info[0], "unset", sizeof("unset")))
		deletenv(child, p);
	else if (!ft_strncmp(child->info[0], "echo", sizeof("echo")))
		ft_echo(child);
	printf("\n");
}

void	ft_putenv(char **env, t_prompt *p)
{
	size_t	i;
	size_t	j;
	int		fd;
	char	pwd[PATH_MAX];

	getcwd(pwd, sizeof(pwd));
	p->builtpath = ft_strjoin(pwd, "/doc/.builtin_cmd");
	p->envpath = ft_strjoin(pwd, "/.env");
	fd = open(p->envpath, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	i = -1;
	while (env[++i])
	{
		j = -1;
		while (env[i][++j])
			write(fd, &env[i][j], sizeof(char));
		write(fd, "\n", sizeof(char));
	}
	close(fd);
}
