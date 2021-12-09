/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:46:07 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/09 19:42:24 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

extern void	ft_builtins(t_child *child, t_prompt *p)
{
	char	pwd[PATH_MAX];

	if (!ft_strncmp(child->info[0], "pwd", sizeof("pwd")))
	{
		if (getcwd(pwd, sizeof(pwd)) != NULL)
			printf("%s\n", pwd);
	}
	else if (!ft_strncmp(child->info[0], "cd", sizeof("cd")))
		ft_changedir(child, p);
	else if (!ft_strncmp(child->info[0], "env", sizeof("env")))
		showenv(p);
	else if (!ft_strncmp(child->info[0], "export", sizeof("export"))
		&& ft_strchr(child->info[1], '='))
		envinclude(child, p);
	else if (!ft_strncmp(child->info[0], "unset", sizeof("unset")))
		deletenv(child, p);
	else if (!ft_strncmp(child->info[0], "echo", sizeof("echo")))
		ft_echo(child);
}

extern void	ft_putenv(char **env, t_prompt *p)
{
	size_t	i;
	size_t	j;
	int		fd;
	char	pwd[PATH_MAX];

	getcwd(pwd, sizeof(pwd));
	p->builtpath = ft_strjoin(pwd, "/doc/.builtin_cmd");
	p->envpath = ft_strjoin(pwd, "/.env");
	p->temppath = ft_strjoin(pwd, "/.tempenv");
	fd = open(p->envpath, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	i = -1;
	while (env[++i])
	{
		j = -1;
		while (env[i][++j])
			write(fd, &env[i][j], sizeof(char));
		write(fd, "\n", sizeof(char));
		p->sizenv++;
	}
	close(fd);
}

extern int	ft_checkbuiltins(char *str, t_prompt *p)
{
	int		fd;
	char	*line;
	size_t	size;	

	if (!str)
		return (0);
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
