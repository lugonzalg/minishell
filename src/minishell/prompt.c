/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:37:46 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/12 20:15:02 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "inc/minishell.h"
#include "inc/libft.h"
#include "inc/ft_printf.h"
#include "inc/get_next_line.h"
#include <readline/readline.h>
#include <readline/history.h>


void	check_redir(t_prompt *p, t_child *child)
{
	p->tmp = p->d2_prompt[child->id];
	//USEFULL??
	if (ft_strchr(p->tmp, INPUT))
		child->redir[0] = true;
	if (ft_strchr(p->tmp, OUTPUT))
		child->redir[1] = true;
	child->info = ft_split(p->d2_prompt[child->id], ' ');	
	while (child->info[child->size[1]])
		child->size[1]++;
	//USEFULL??
	if (child->redir[0] || child->redir[1])
		unify_fdio(child);
	unify_cmd(child);
	command_pos(p, child); //FIND COMMAND POS
}
//FALTA REDIRIGIR LOS FDs DE LOOS ARCHIVOS TEMPORALES (SI HAY) AL FDPIPES, Y METER EL PATH EXACTO EN LA EXTRUCTURA
void	multipipe(t_child *child)
{
	size_t	i;
	ssize_t	signal;

	i = 0;
	while (i < child->size[0])
	{
		if (i != child->id)
			close(child->fdpipe[i][0]);
		if (i != child->id + 1)
			close(child->fdpipe[i][1]);
		i++;
	}
	dup2(child->fdpipe[child->id][0], 0);
	close(child->fdpipe[child->id][0]);
	dup2(child->fdpipe[child->id + 1][1], 1);
	close(child->fdpipe[child->id + 1][1]);
   	signal = execve(child->path, child->info, NULL);
	exit(0);
}

static void	restart_data(t_child *child)
{
	size_t	i;

	ft_memset(&child->size[1], 0, sizeof(size_t) * 3);
	ft_memset(child->redir, false, sizeof(bool) * 2);
	i = -1;
	while (child->info[++i])
	{
		free(child->info[i]);
		child->info[i] = NULL;
	}
	free(child->info);
	child->info = NULL;
	free(child->path);
	child->path = NULL;
}

static void	process_io(t_prompt *p)
{
	size_t	i;
	pid_t	*id;
	t_child	child;

	p->d2_prompt = ft_split(p->prompt, '|');
	set_child(p, &child);
	id = (pid_t *)malloc(sizeof(pid_t) * child.size[0]);
	i = -1;
	while (p->d2_prompt[++i])
	{
		child.id = i;
		check_redir(p, &child);
		if (ft_checkbuiltins(child.info[0]))
			ft_builtins(&child, p);
		else
		{
			id[i] = fork();
			if (id[i] == 0)
				multipipe(&child);
			else
			{
				close(child.fdpipe[i][1]);
				wait(NULL);
			}
		}
		restart_data(&child);
	}
	i = -1;
	while (0 && p->d2_prompt[++i])
	{
		close(child.fdpipe[i][0]);
		close(child.fdpipe[i][1]);
	}
}

extern void	prompt_io(t_prompt *p)
{
	while (1)
	{
		p->prompt = readline(p->user);
		add_history(p->prompt);
		process_io(p);
		free(p->prompt);
	}
	rl_clear_history();
}
