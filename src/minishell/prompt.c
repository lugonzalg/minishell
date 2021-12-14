/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:37:46 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/13 20:55:39 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	ft_handle_pipe(t_child *child)
{
	size_t	i;

	i = -1;
	while (++i < child->size[0])
	{
		if (i != child->id)
			close(child->fdpipe[i][0]);
		if (i != child->id + 1)
			close(child->fdpipe[i][1]);
	}
	if (child->id || child->redir[0])
	{
		dup2(child->fdpipe[child->id][0], 0);
		close(child->fdpipe[child->id][0]);
	}
	if (child->id < child->size[0] - 2 || child->redir[1])
	{
		dup2(child->fdpipe[child->id + 1][1], 1);
		close(child->fdpipe[child->id + 1][1]);
	}
}

void	ft_multipipe(t_child *child)
{
	if (child->redir[2])
		exit(129);
	ft_handle_pipe(child);
	if (child->path)
		execve(child->path, child->info, NULL);
	if (!child->path || access(child->path, X_OK))
		exit(127);
	exit(126);
}

static void	ft_process_command(t_prompt *p, t_child *child, size_t i)
{
	child->id = i;
	ft_check_redir(p, child);
	if (ft_checkbuiltins(child->info[0], p))
		ft_builtins(child, p);
	else if (child->info[0])
	{
		p->id[i] = fork();
		g_glob.killid = p->id[0];
		if (p->id[i] == 0)
			ft_multipipe(child);
		else
		{
			if (!ft_strncmp(child->info[0], getenv("PWD")
					, ft_strlen(child->info[0]))
				|| !ft_strncmp(child->info[0], getenv("HOME")
					, ft_strlen(child->info[0])))
			{
				printf("minishell: %s: is a directory\n", child->info[0]);
				ft_go_exit(126);
			}
			if (!child->path || access(child->path, X_OK))
				printf("minishell: %s: command not found\n", child->info[0]);
		}
	}
	ft_restart_data(child);
}

extern void	ft_process_io(t_prompt *p)
{
	size_t	i;
	t_child	child;
	int		status;

	status = 0;
	ft_set_child(p, &child);
	i = -1;
	while (p->d2_prompt[++i])
		ft_process_command(p, &child, i);
	ft_free_child(&child);
	while (1)
	{
		if (wait(&status) <= 0)
			break ;
		if (status == 256)
			ft_go_exit(1);
		else if (status == 32256)
			ft_go_exit(126);
		else if (status == 32512)
			ft_go_exit(127);
		else
			ft_go_exit(0);
	}
}

extern int	ft_prompt_error(char *prompt)
{
	size_t	n;

	n = 0;
	while (*prompt)
	{
		if (*prompt == PIPE && !n)
		{
			ft_go_exit(258);
			return (printf("minishell: syntax error near unexpected token `|'\n"));
		}
		else if (*prompt == PIPE && n)
			n = 0;
		else if (*prompt == AND && !n)
		{
			ft_go_exit(258);
			return (printf("minishell: syntax error near unexpected token `&'\n"));
		}
		else if (*prompt == AND && n)
			n = 0;
		if (*prompt != SPACE && *prompt != PIPE && *prompt != AND)
			n++;
		prompt++;
	}
	return (0);
}

extern void	ft_prompt_io(t_prompt *p)
{
	while (1)
	{
		g_glob.killid = 0;
		p->prompt = readline("minishell > ");
		if (!p->prompt || !ft_strncmp(p->prompt, "exit", 5)
			|| !ft_strncmp(p->prompt, "exit ", 5))
		{
			write(1, "exit\n", 5);
			break ;
		}
		rl_on_new_line();
		if (ft_prompt_error(p->prompt))
		{
			add_history(p->prompt);
			free(p->prompt);
			continue ;
		}
		if (ft_check_prompt(p))
			continue ;
		ft_free_d2(p->d2_prompt);
	}
	rl_clear_history();
}
