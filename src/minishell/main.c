/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 22:28:39 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/07 19:43:30 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/ft_printf.h"
#include "inc/libft.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

void	check_redir(t_prompt *p, t_child *child)
{
	if (ft_strchr(p->d2_prompt[child->id], INPUT))
		child->redir[0] = true;
	if (ft_strchr(p->d2_prompt[child->id], OUTPUT))
		child->redir[1] = true;
	child->info = ft_split_ptr(p->d2_prompt[child->id],
			' ', ft_len_redir);
	ft_expand(p, child);
	while (child->info[child->size[1]])
		child->size[1]++;
	if (child->redir[0] || child->redir[1])
		unify_fdio(child);
	unify_cmd(p, child);
	if (!child->builtin || !ft_strncmp(child->info[0], "expr", 4))
		command_pos(p, child);
}

int	go_exit(int n)
{
	g_glob.error = n;
	return (n);
}

int	ft_putpath(t_child *child)
{
	if (child->info[0] && !access(child->info[0], X_OK))
		child->path = ft_strdup(child->info[0]);
	return (1);
}

extern void	command_pos(t_prompt *p, t_child *child)
{
	int	i;

	i = -1;
	p->path = ft_setpath(p);
	if (!p->path && ft_putpath(child))
		return ;
	while (p->path[++i] && !child->path)
	{
		if (!access(child->info[0], X_OK))
		{
			child->path = ft_strdup(child->info[0]);
			break ;
		}
		child->path = ft_strjoin(p->path[i], child->info[0]);
		if (!access(child->path, X_OK))
			break ;
		free(child->path);
		child->path = NULL;
	}
	free_d2(p->path);
}

int	main(int argc, char *argv[], char *env[])
{
	t_prompt	p;

	(void)argv;
	rl_catch_signals = 0;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	if (argc != 1)
		return (1);
	ft_memset(&p, 0, sizeof(t_prompt));
	p.home = ft_strdup(getenv("HOME"));
	ft_putenv(env, &p);
	print_intro();
	prompt_io(&p);
	free_p(&p);
	return (0);
}
