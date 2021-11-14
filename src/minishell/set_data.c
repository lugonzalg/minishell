/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:38:35 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/13 23:37:16 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "inc/minishell.h"
#include "inc/libft.h"
#include "inc/get_next_line.h"
#include <fcntl.h>

void	lstadd_back(t_builtin **blt, t_builtin *new)
{
	t_builtin	*node;

	node = *blt;
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new;
		new->next = NULL;
	}
	else
		(*blt) = new;
}

static void	set_function(t_builtin *blt)
{
	if (!ft_strncmp(blt->key, "echo", sizeof("echo")))
			blt->ptr = ft_echo;
	else if (!ft_strncmp(blt->key, "exit", sizeof("exit")))
			blt->ptr = ft_exit;
	else if (!ft_strncmp(blt->key, "cd", sizeof("cd")))
			blt->ptr = ft_cd;
	else if (!ft_strncmp(blt->key, "pwd", sizeof("pwd")))
			blt->ptr = ft_pwd;
	else if (!ft_strncmp(blt->key, "export", sizeof("export")))
			blt->ptr = ft_export;
	else if (!ft_strncmp(blt->key, "unset", sizeof("unset")))
			blt->ptr = ft_unset;
	else if (!ft_strncmp(blt->key, "env", sizeof("env")))
			blt->ptr = ft_env;
}

static void create_list(t_prompt *p)
{
	char		*line;
	ssize_t		fd;
	t_builtin	*tmp;

	fd = open("doc/builtin_cmd", O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		tmp = (t_builtin *)malloc(sizeof(t_builtin));
		line[ft_strlen(line) - 1] = 0;
		tmp->key = line;
		tmp->next = NULL;
		lstadd_back(&p->head, tmp);
		set_function(tmp);
	}
}

extern void	set_prompt(t_prompt *p)
{
	int	i;

	ft_memset(p, 0, sizeof(t_prompt));
	p->user = ft_strjoin(getenv("USER"), " \e[1;37mminishell \e[0;m% ");
	p->path = ft_split(getenv("PATH"), ':');
	i = -1;
	while (p->path[++i])
	{
		p->tmp = ft_strjoin(p->path[i], "/");
		free(p->path[i]);
		p->path[i] = p->tmp;
	}
	create_list(p);
}

extern void	set_child(t_prompt *p, t_child *child)
{
	size_t	i;

	i = 1;
	while (p->d2_prompt[i - 1])
		i++;
	ft_memset(child, 0, sizeof(t_child));
	child->fdpipe = (int **)malloc(sizeof(int *) * i);
	child->size[0] = i;
	while (i--)
	{
		child->fdpipe[i] = (int *)malloc(sizeof(int) * 2);
		pipe(child->fdpipe[i]);
	}
	child->fdpipe[child->size[0] - 1][1] = 2;
}

extern void	free_child(t_child *child)
{
	size_t	i;

	i = -1;
	i = -1;
	while (++i < child->size[0])
	{
		close(child->fdpipe[i][0]);
		close(child->fdpipe[i][1]);
		free(child->fdpipe[i]);
	}
	free(child->fdpipe);
	free(child->ttypath);
}

extern void	free_p(t_prompt *p)
{
	free_d2(p->path);
	free(p->user);
	free(p->envpath);
}
