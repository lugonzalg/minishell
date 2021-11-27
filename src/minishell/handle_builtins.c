/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:46:07 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/27 17:01:47 by lugonzal         ###   ########.fr       */
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
