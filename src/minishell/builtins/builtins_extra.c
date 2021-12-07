/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:51:36 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/07 19:53:54 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

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

int	ft_changedir(t_child *child, t_prompt *p)
{
	if (child->size[1] == 1)
	{
		chdir(p->home);
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
	return (0);
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

extern char	**ft_realloc_child(char **temp)
{
	int		size;
	char	**d2;
	int		index;

	size = 0;
	while (temp[size])
		size++;
	size += 1;
	d2 = (char **)ft_calloc(sizeof(char *), size + 1);
	d2[0] = ft_strdup("echo");
	size = 1;
	index = 0;
	while (temp[index])
	{
		d2[size] = ft_strdup(temp[index]);
		free(temp[index]);
		index++;
		size++;
	}
	d2[size] = NULL;
	free(temp);
	return (d2);
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
			write(fd, " ", 1);
	}
	if (nl)
		write(fd, "\n", 1);
}
