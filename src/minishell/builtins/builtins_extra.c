/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:51:36 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/29 21:47:56 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
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

extern char	*expand_var(t_prompt *p, t_child *child, size_t i)
{
	char	*line;
	int		fd;
	char	*var;

	child->builtin = true;
	fd = open(p->envpath, O_RDONLY);
	if (child->info[0][0] == '$')
		child->echo = true;
	line = ft_strtrim(child->info[i], "$\"");
	free(child->info[i]);
	child->info[i] = ft_strjoin(line, "=");
	free(line);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (!ft_strncmp(line, child->info[i], ft_strlen(child->info[i])))
		{
			line[ft_strlen(line) - 1] = 0;
			close(fd);
			var = ft_strtrim(line, child->info[i]);
			free(line);
			return (var);
		}
		free(line);
	}
	close(fd);
	return (NULL);
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
