/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_intro.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:04 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/19 20:18:01 by mikgarci         ###   ########.fr       */
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

int	ft_putchar1(int c)
{
	write(1, &c, 1);
	return (0);
}

int	got_error(int n)
{
	g_glob.error = n;
	return (g_glob.error);
}

void   sig_handler(int signo)
{
	char buf[1024];
	char *str;
	(void) signo;
	if (signo == SIGINT)
	{
		tgetent(buf, getenv("TERM"));
		str = tgetstr("kL", NULL);
		//printf("\n\n\n");
	//	str1 = tgetstr("rc", NULL);
		tputs(str, 1, &ft_putchar1);
//		tputs(str, 1, &ft_putchar1);
	//	int a = tgetnum("li");
	//	int b = tgetnum("co");
		//ft_putstr_fd(tgoto(str, b, 1), 1);
	//	printf("  ");
	//	ft_putstr_fd(tgoto(str, b, a), 1);
	//	ft_putstr_fd(tgoto(gotostr, x, y), 1);
//		printf("\n");
	//	printf("%d, %d\n", a, b);
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
