/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sig_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:04 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/11 20:23:39 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void	ft_sig_handler(int signo)
{
	if (signo == SIGINT && !g_glob.killid)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		ft_go_exit(1);
	}
	if (signo == SIGINT && g_glob.killid)
	{
		printf("\r");
		write(1, "\n", 1);
		ft_go_exit(130);
	}
	if (signo == SIGQUIT && g_glob.killid)
	{
		kill(g_glob.killid, SIGQUIT);
		printf("Quit: 3\n");
		g_glob.error = 131;
	}
}

extern int	ft_go_exit(int n)
{
	g_glob.error = n;
	return (n);
}

static int	ft_fd_name(t_child *child, size_t *i, char redir, char **fd_err)
{
	if (ft_strchr(child->info[*i], redir)
		&& ft_strchr(child->info[*i + 1], redir)
		&& (ft_strncmp(child->info[*i], "<", 2)
		|| ft_strncmp(child->info[*i], "<<", 3)
		|| ft_strncmp(child->info[*i], ">", 2)
		|| ft_strncmp(child->info[*i], ">>", 3)))
	{
		(*fd_err) = child->info[*i + 1];
		return (-1);
	}
	else if (!child->info[*i + 1] && ft_strlen(child->info[*i]) < 3)
		return (-1);
	return (1);
}

extern int	ft_fdcheck(t_child *child, char redir)
{
	size_t	i;
	int		fd;
	char	*fd_err;

	i = -1;
	while (child->info[++i])
	{
		fd_err = "newline";
		if (!ft_strchr(child->info[i], redir))
			continue ;
		fd = ft_fd_name(child, &i, redir, &fd_err);
		if (fd == -1)
		{
			printf("minishell: syntax error near unexpected token `%s'\n",
				fd_err);
			return (ft_go_exit(258));
		}
	}
	return (0);
}

extern bool	ft_check_prompt(t_prompt *p)
{
	size_t	i;

	p->d2_prompt = ft_split_ptr(p->prompt, '|', ft_lenp);
	i = -1;
	if (p->prompt && p->prompt[0])
		add_history(p->prompt);
	while (p->d2_prompt[++i])
	{
		if (!ft_quote_error(p->d2_prompt[i]))
		{
			ft_free_d2(p->d2_prompt);
			free(p->prompt);
			return (true);
		}
	}
	ft_process_io(p);
	free(p->id);
	free(p->prompt);
	return (false);
}
