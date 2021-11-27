/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:51:36 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/27 21:51:56 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

static int		changediraux(t_child *child, t_prompt *p)
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

int		ft_changedir(t_child *child, t_prompt *p)
{
	if(child->size[1] == 1)
	{
		chdir(p->home);
		return (0);
	}
	if (changediraux(child, p) != -1)
		return (0);
	if (chdir(child->info[1]))
	{
		printf("minishell: cd: %s: No such file or directory\n", child->info[1]);
		go_exit(12);
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
