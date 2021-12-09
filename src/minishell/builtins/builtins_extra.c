/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:51:36 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/09 17:50:42 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

void	changepwd(char *str, t_prompt *p)
{
	char	*temp;

	temp = ft_strjoin("PWD=", str);
	deletpwd(temp, p);
}

extern void	deletpwd(char *str, t_prompt *p)
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
		if (ft_strncmp(str, line, 4))
			write(fd[1], line, ft_strlen(line));
		else
		{
			write(fd[1], str, ft_strlen(str));
			write(fd[1], "\n", 1);
		}
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	deletenv_2(p);
}

static int	changediraux(t_child *child, t_prompt *p)
{
	char	*path;
	char	*temp;

	if ((child->info[1][0] == '~' && child->info[1][1] == '/') ||
			(child->info[1][0] == '~' && ft_strlen(child->info[1]) == 1))
	{
		temp = ft_strdup(child->info[1] + 1);
		path = ft_strjoin(p->home, temp);
		free(temp);
		if (!chdir(path))
		{
			changepwd(path, p);
			free(path);
			return (0);
		}
		else
		{
			printf("minishell: cd: %s: No such file or directory\n", path);
			free(path);
			return (go_exit(1));
		}
	}
	return (-1);
}

char	*search_pwd(t_prompt *p)
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
		if (!ft_strncmp("PWD=", line, 4))
			return (line);
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	return (NULL);
}

int	ft_changedir(t_child *child, t_prompt *p)
{
	if (child->size[1] == 1)
	{
		chdir(p->home);
		changepwd(p->home, p);
		return (0);
	}
	if (changediraux(child, p) != -1)
		return (0);
	if (chdir(child->info[1]))
	{
		printf("minishell: cd: %s: No such file or directory\n", child->info[1]);
		go_exit(128);
		return (1);
	}
	ft_putpwd(child->info[1], p);
	return (0);
}
