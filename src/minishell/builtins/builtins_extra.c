/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:51:36 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/02 22:33:40 by lugonzal         ###   ########.fr       */
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

static char *ft_joinfr(char *s1, char *s2, size_t j, size_t k)
{
	char	*n_str;
	size_t	i;
	char	*pos;

	i = ft_strlen(s2);
	pos = ft_strchr(s2, '=');
	n_str = ft_calloc(sizeof(char), ft_strlen(s1) + i + 1);
	ft_memcpy(n_str, s1, j);
	ft_memcpy(n_str + j, pos + 1, ft_strlen(pos) - 2);
	ft_memcpy(n_str + ft_strlen(n_str), s1 + j + k, ft_strlen(s1) - j);
	free(s1);
	free(s2);
	return (n_str);
}

extern char	*expand_var(t_prompt *p, t_child *child, size_t i)
{
	char	*n_str;
	char	*env;
	size_t	j;
	size_t	k;

	j = 0;
	//var = ft_puterror(child, i);
	//if (var)
	//	return (var);
	while (child->info[i][j])
	{
		n_str = ft_strchr(&child->info[i][j], '$');
		if (!n_str)
			break ;
		k = 0;
		while (n_str && ft_isalnum(n_str[k + 1]))
			k++;
		p->tmp = ft_substr(n_str + 1, 0, k);
		env = ft_gnl_query(p->envpath, p->tmp);
		if (!env)
		{
			free(p->tmp);
			j += k + (n_str - &child->info[i][j]);
			continue ;
		}
		free(p->tmp);
		child->info[i] = ft_joinfr(child->info[i], env, j + 1, k + 1);
	}
	return (ft_strdup(child->info[i]));
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
