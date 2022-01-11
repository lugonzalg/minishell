/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:37:46 by lugonzal          #+#    #+#             */
/*   Updated: 2022/01/11 16:31:24 by mikgarci         ###   ########.fr       */
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
#include <dirent.h>

static void	ft_process_command(t_prompt *p, t_child *child)
{
	ft_check_redir(p, child);
	if (child->info && ft_checkbuiltins(child->info[0], p))
		ft_builtins(child, p);
	else if (child->info && child->info[0])
	{
		p->id[child->id] = fork();
		g_glob.killid = p->id[0];
		ft_handlewc(child);
		if (p->id[child->id] == 0)
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
	{
		child.id = i;
		ft_process_command(p, &child);
	}
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
	}
}

static void	ft_exit(t_prompt *p)
{
	char	**tab;
	size_t	i;

	i = -1;
	if (!p->prompt || !ft_strncmp(p->prompt, "exit", 5)
		|| !ft_strncmp(p->prompt, "exit ", 5))
	{
		tab = ft_split(p->prompt, 32);
		free(p->prompt);
		write(1, "exit\n", 5);
		while (tab && tab[1] && tab[1][++i])
		{
			if (!ft_isdigit(tab[1][i]))
			{
				printf("bash: exit: %s: numeric argument required\n", tab[1]);
				ft_free_d2(tab);
				exit(255);
			}
		}
		i = 0;
		if (tab && tab[1])
			i = ft_atoi(tab[1]);
		ft_free_d2(tab);
		exit(i);
	}
}

extern void	ft_prompt_io(t_prompt *p)
{
	while (1)
	{
		g_glob.here_doc = 1;
		g_glob.killid = 0;
		p->prompt = readline("minishell > ");
		p->tmp = ft_strtrim(p->prompt, " ");
		free(p->prompt);
		p->prompt = p->tmp;
		ft_exit(p);
		rl_on_new_line();
		if (ft_prompt_error(p))
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
