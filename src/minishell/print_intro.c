/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_intro.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:04 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/27 16:32:05 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"
#include "inc/get_next_line.h"
#include "inc/ft_printf.h"
#include "inc/minishell.h"
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

int	go_exit(int n)
{
	g_glob.error = n;
	return (g_glob.error);
}

void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		//rl_catch_signals = 0;
		printf("\n");
		//rl_catch_signals = 1;
		//rl_on_new_line();
		//rl_replace_line("", 0);
		//rl_redisplay();
	}
	if (signo == SIGQUIT && g_glob.killid)
	{
		kill(g_glob.killid, SIGQUIT);
		printf("Quit: 3\n");
		g_glob.error = 131;
	}
	else
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

extern void	print_intro(void)
{
	int		fd;
	char	*str;

	fd = open("./doc/intro.txt", O_RDONLY);
	printf("\n");
	while (1)
	{
		str = get_next_line(fd);
		if (!str)
			break ;
		printf("%s", str);
		free(str);
	}
	close(fd);
}
