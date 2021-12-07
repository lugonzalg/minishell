/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_intro.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:04 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/06 17:22:10 by mikgarci         ###   ########.fr       */
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

void	sig_handler(int signo)
{
	(void) signo;
/*	if (signo == SIGINT && !g_glob.killid)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signo == SIGINT && g_glob.killid)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		printf("\n");
	}
	if (signo == SIGQUIT && g_glob.killid)
	{
		kill(g_glob.killid, SIGQUIT);
		printf("Quit: 3\n");
		g_glob.error = 131;
	}*/
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
