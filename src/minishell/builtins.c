/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 19:58:59 by mikgarci          #+#    #+#             */
/*   Updated: 2021/11/08 21:22:27 by mikgarci         ###   ########.fr       */
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
	if (!ft_strncmp(str, "pwd", sizeof(str)))
			return (1);
	if (!ft_strncmp(str, "cd", sizeof(str)))
			return (1);
	if (!ft_strncmp(str, "env", sizeof(str)))
			return (1);
	if (!ft_strncmp(str, "export", sizeof(str)))
			return (1);
	return (0);
}

static void		showenv(void)
{
	char	*line;
	int		fd;

	fd = open(".env", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

void	envinclude(t_child	*child)
{
	int	fd;
	
	fd = open(".env", O_APPEND);
	write(fd, child->info[1], sizeof(child->info[1]));
	close(fd);
}

void	ft_builtins(t_child *child)
{
	char	pwd[PATH_MAX];

	if (!ft_strncmp(child->info[0], "pwd", sizeof(child->info[0])))
	{
		if (getcwd(pwd, sizeof(pwd)) != NULL)
			printf("%s\n", pwd);
	}
	if (!ft_strncmp(child->info[0], "cd", sizeof(child->info[0])))
		chdir(child->info[1]);
	if (!ft_strncmp(child->info[0], "env", sizeof(child->info[0])))
		showenv();
	if (!ft_strncmp(child->info[0], "export", sizeof(child->info[0])))
		envinclude(child);
}

void	ft_putenv(char **env)
{
	int	a;
	int b;
	int fd;

	fd = open(".env", O_WRONLY |O_TRUNC | O_CREAT, 0644);
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
}
