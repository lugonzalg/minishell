/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 19:58:59 by mikgarci          #+#    #+#             */
/*   Updated: 2021/11/09 20:06:57 by mikgarci         ###   ########.fr       */
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

#define PATH_MAX 4096

int		ft_checkbuiltins(char *str)
{
	if (!ft_strncmp(str, "pwd", sizeof("pwd")))
			return (1);
	if (!ft_strncmp(str, "cd", sizeof("cd")))
			return (1);
	if (!ft_strncmp(str, "env", sizeof("env")))
			return (1);
	if (!ft_strncmp(str, "export", sizeof("export")))
			return (1);
	if (!ft_strncmp(str, "unset", sizeof("unset")))
			return (1);
	return (0);
}

static void		showenv(t_string *str)
{
	char	*line;
	int		fd;

	printf("%s\n", str->envpath);
	fd = open(str->envpath, O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

void	envinclude(t_child	*child, t_string *str)
{
	int	fd;
	int	a;
	
	fd = open(str->envpath, O_WRONLY | O_APPEND);
	a = 0;
	while (child->info[1][a])
	{
		write(fd, &child->info[1][a], 1);
		a++;
	}
	write(fd, "\n", 1);
	close(fd);
}

void	deletenv(t_child	*child, t_string *str)
{
	int		fd1;
	int		fd2;
	char	*line;
	
	fd1 = open(str->envpath, O_RDONLY);
	fd2 = open(".envtemp", O_WRONLY | O_CREAT, 0644);
	line = get_next_line(fd1);
	while (line)
	{
		if (ft_strncmp(child->info[1], line, ft_strlen(child->info[1]) - 1))
			write(fd2, line, ft_strlen(line));
		free(line);
		line = get_next_line(fd1);
	}
	close(fd1);
	close(fd2);
	fd1 = open(str->envpath, O_WRONLY | O_TRUNC);
	fd2 = open(".envtemp", O_RDONLY);
	line = get_next_line(fd2);
	while (line)
	{
		write(fd1, line, ft_strlen(line));
		free(line);
		line = get_next_line(fd2);
	}
	close(fd1);
	close(fd2);
	unlink(".envtemp");
}

void	ft_builtins(t_child *child, t_string *str)
{
	char	pwd[PATH_MAX];

	if (!ft_strncmp(child->info[0], "pwd", sizeof("pwd")))
	{
		if (getcwd(pwd, sizeof(pwd)) != NULL)
			printf("%s\n", pwd);
	}
	if (!ft_strncmp(child->info[0], "cd", sizeof("cd")))
		chdir(child->info[1]);
	if (!ft_strncmp(child->info[0], "env", sizeof("env")))
		showenv(str);
	if (!ft_strncmp(child->info[0], "export", sizeof("export")) && ft_strchr(child->info[1], '='))
		envinclude(child, str);
	if (!ft_strncmp(child->info[0], "unset", sizeof("unset")))
		deletenv(child, str);
}

void	ft_putenv(char **env, t_string *str)
{
	int		a;
	int		b;
	int		fd;
	char	pwd[PATH_MAX];

	getcwd(pwd, sizeof(pwd));
	str->envpath = ft_strjoin(pwd, "/.env");
	fd = open(str->envpath, O_WRONLY |O_TRUNC | O_CREAT, 0644);
	a = 0;
	while(env[a])
	{
		b = 0;
		while(env[a][b])
		{
			write(fd, &env[a][b], sizeof(char));
			b++;
		}
		write(fd, "\n", sizeof(char));
		a++;
	}
	close(fd);
}
