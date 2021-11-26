/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:37:46 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/26 18:20:00 by lugonzal         ###   ########.fr       */
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
#include <term.h>

void	check_redir(t_prompt *p, t_child *child)
{
	if (ft_strchr(p->d2_prompt[child->id], INPUT))
		child->redir[0] = true;
	if (ft_strchr(p->d2_prompt[child->id], OUTPUT))
		child->redir[1] = true;
	child->info = ft_split_ptr(p->d2_prompt[child->id], ' ', ft_len_redir, ft_cut_redir);
	while (child->info[child->size[1]])
		child->size[1]++;
	if (child->redir[0] || child->redir[1])
		unify_fdio(child);
	unify_cmd(p, child);
	if (!child->builtin)
		command_pos(p, child);
}

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
	if (child->id || child->redir[0])
	{
		dup2(child->fdpipe[child->id][0], 0);
		close(child->fdpipe[child->id][0]);
	}
	if (child->id < child->size[0] - 2 || (child->redir[1] && !child->redir[2]))
	{
		dup2(child->fdpipe[child->id + 1][1], 1);
		close(child->fdpipe[child->id + 1][1]);
	}
	signal = execve(child->path, child->info, NULL);
	exit(0);
}

static void	restart_data(t_child *child)
{
	ft_memset(&child->size[1], 0, sizeof(size_t) * 3);
	ft_memset(child->redir, false, sizeof(bool) * 2);
	free_d2(child->info);
	child->path = NULL;
}

static void	process_io(t_prompt *p)
{
	size_t	i;
	t_child	child;
	int		status;
	pid_t	pid;

	status = 0;
	set_child(p, &child);
	p->id = (pid_t *)malloc(sizeof(pid_t) * child.size[0]);
	i = -1;
	while (p->d2_prompt[++i])
	{
		child.id = i;
		check_redir(p, &child);
		if (ft_checkbuiltins(child.info[0], p))
			ft_builtins(&child, p);
		else
		{
			p->id[i] = fork();
			g_glob.killid = p->id[1];
			if (p->id[i] == 0)
				multipipe(&child);
		}
		restart_data(&child);
	}
	free_child(&child);
	while (1)
	{
		pid = wait(&status);
		if (pid < 0)
			break ;
	}
}

extern void	prompt_io(t_prompt *p)
{
	while (1)
	{
		g_glob.killid = 0;
		p->prompt = readline("minishell > ");
		rl_on_new_line();
		//driver_talk();
		p->d2_prompt = ft_split_ptr(p->prompt, '|', ft_lenp, ft_cutp);
		add_history(p->prompt);
		process_io(p);
		free_d2(p->d2_prompt);
		free(p->prompt);
		free(p->id);
	}
	rl_clear_history();
}
