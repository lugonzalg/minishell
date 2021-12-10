/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sig_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:04 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/10 22:26:04 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>

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
