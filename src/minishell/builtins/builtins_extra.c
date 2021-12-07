/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:51:36 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/07 21:53:29 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

static void	changepwd(char *str, t_prompt *p)
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
	fd[1] = open(".envtemp", O_WRONLY | O_CREAT, 0644);
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
	fd[1] = open(".envtemp", O_WRONLY | O_CREAT, 0644);
	while (1)
	{
		line = get_next_line(fd[0]);
		if (!line)
			break ;
		if (!ft_strncmp("PWD=", line, 4))
			return(line);
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	return(NULL);
}

void	ft_putpwd(char *str, t_prompt *p)
{
	char *temp;
	char *pwd;
	char *dest;

	pwd = search_pwd(p);
	temp = ft_substr(pwd, 4, ft_strlen(pwd) - 5);
	free(pwd);
	while (str && *str)
	{
		if (*str == '/')
			str++;
		if (*str == '.')
		{
			pwd = ft_substr(temp, 0, ft_strlen(temp) - ft_strlen(ft_strrchr(temp, '/')));
			free(temp);
			temp = pwd;
			if (!temp[0])
			{
				free(temp);
				pwd = ft_strdup("/");
				break ;
			}
			str = ft_strchr(str, '/');
		}
		else
		{
			pwd = ft_substr(str, 0, ft_strlen(str) - ft_strlen(ft_strchr(str, '/')));
			dest = ft_strjoin("/", pwd); 
			free(pwd);
			pwd = ft_strjoin(temp, dest);
			free(dest);
			free(temp);
			temp = pwd;
			str = ft_strchr(str, '/');
		}
	}
	changepwd(pwd, p);
	free(pwd);
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
