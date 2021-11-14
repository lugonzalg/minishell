/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 19:58:59 by mikgarci          #+#    #+#             */
/*   Updated: 2021/11/14 02:22:32 by lugonzal         ###   ########.fr       */
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
#include <dirent.h>

#define PATH_MAX 4096

int	ft_checkbuiltins(char *str)
{
	int		fd;
	char	*line;
	size_t	size;	

	fd = open("doc/builtin_cmd", O_RDONLY);
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

void	ft_builtins(t_child *child, t_prompt *p)
{
	t_ptr		ft_builtin;
	t_builtin	*tmp;

	tmp = p->head;
	while (tmp)
	{
		if (!ft_strncmp(child->info[child->id], tmp->key, ft_strlen(tmp->key)))
		{
			ft_builtin = tmp->ptr;
			ft_builtin(p, child);
		}
		tmp = tmp->next;
	}
}

void	ft_putenv(char **env, t_prompt *p)
{
	size_t	i;
	size_t	j;
	int		fd;
	char	pwd[PATH_MAX];

	getcwd(pwd, sizeof(pwd));
	p->envpath = ft_strjoin(pwd, "/.env");
	fd = open(p->envpath, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	i = -1;
	while (env[++i])
	{
		j = -1;
		while (env[i][++j])
			write(fd, &env[i][j], sizeof(char));
		write(fd, "\n", sizeof(char));
	}
	close(fd);
}


extern void	ft_echo(t_prompt *p, t_child *child)
{
	(void)p;
	(void)child;
	printf("eeasfaef");
}


extern void	ft_exit(t_prompt *p, t_child *child)
{

	(void)p;
	(void)child;
	printf("eeasfaef");
}

extern void	ft_cd(t_prompt *p, t_child *child)
{
	DIR 			*dir;
	struct dirent	*s_dir;
	char			**d2_dir;

	d2_dir = ft_split(p->d2_prompt[child->id], 32);
	dir = opendir(d2_dir[1]);
	if (!dir)
	{
		free_d2(d2_dir);
		return ;
	}
	s_dir = readdir(dir);
	(void)child;
	printf("eeasfaef");
}

extern void	ft_pwd(t_prompt *p, t_child *child)
{

	(void)p;
	(void)child;
	printf("eeasfaef");
}

extern void	ft_export(t_prompt *p, t_child *child)
{
	int	fd;
	int	i;

	fd = open(p->envpath, O_WRONLY | O_APPEND);
	i = -1;
	while (child->info[1][++i])
		write(fd, &child->info[1][i], 1);
	write(fd, "\n", 1);
	close(fd);
}

static void	ft_unset_2(t_prompt *p, t_child *child)
{
	int		fd[2];
	char	*line;
	(void)child;

	fd[0] = open(p->envpath, O_WRONLY | O_TRUNC);
	fd[1] = open(".envtemp", O_RDONLY);
	while (1)
	{
		line = get_next_line(fd[1]);
		if (!line)
			break ;
		write(fd[0], line, ft_strlen(line));
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	unlink(".envtemp");
}

extern void	ft_unset(t_prompt *p, t_child *child)
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
		if (ft_strncmp(child->info[1], line, ft_strlen(child->info[1]) - 1))
			write(fd[1], line, ft_strlen(line));
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	ft_unset_2(p, child);
}

extern void	ft_env(t_prompt *p, t_child *child)
{
	char	*line;
	int		fd;
	(void)child;

	printf("%s\n", p->envpath);
	fd = open(p->envpath, O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		printf("%s", line);
		free(line);
	}
	close(fd);
}
