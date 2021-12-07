/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 22:28:39 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/06 17:27:19 by mikgarci         ###   ########.fr       */
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

static int ft_check_even(char *str, char **s_ptr, char c)
{
	while (*str)
	{
		if (*str == c)
		{
			(*s_ptr) = str + 1;
			return (1);
		}
		str++;
	}
	return (0);
}

extern int ft_quote_error(char *str)
{
	char	quote;

	while (*str)
	{
		if (*str == '\'' || *str == '\"')
		{
			quote = *str;
			if (!ft_check_even(str + 1, &str, *str))
			{
				printf("unclosed quotes %c\n", quote);
				return (0);
			}
			continue ;
		}
		str++;
	}
	return (1);
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
	set_str(&p);
	ft_putenv(env, &p);
	print_intro();
	prompt_io(&p);
	free_p(&p);
	return (0);
}
