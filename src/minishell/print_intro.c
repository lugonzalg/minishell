/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_intro.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:04 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/17 20:34:27 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/inc/libft.h"
#include "../libft/inc/get_next_line.h"
#include "../ft_printf/inc/ft_printf.h"
#include "../../inc/minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <term.h>

int	got_error(int n)
{
	g_glob.error = n;
	return (g_glob.error);
}

void   sig_handler(int signo)
{
	char buf[1024];
	char *str;
	if (signo == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signo == SIGQUIT && g_glob.killid)
	{
		kill(g_glob.killid, SIGQUIT);
		printf("Quit: 3\n");
		g_glob.error = 131;
	}
	else
	{
		tgetent(buf, getenv("TERM"));
		str = tgetstr("do", NULL);
		fputs(str, stdout);
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
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, sig_handler);
		str = get_next_line(fd);
		if (!str)
			break ;
		printf("%s", str);
		free(str);
	}
	close(fd);
}
