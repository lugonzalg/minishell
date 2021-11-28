/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 22:28:39 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/28 21:17:04 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/ft_printf.h"
#include "inc/libft.h"
#include <signal.h>
#include <stdlib.h>

int	ft_putpath(t_child *child)
{
	if (!access(child->info[0], X_OK))
		child->path = ft_strdup(child->info[0]);
	return (1);
}

extern void	command_pos(t_prompt *p, t_child *child)
{
	int	i;
	int	j;

	i = -1;
	p->path = ft_setpath(p);
	if (!p->path && ft_putpath(child))
		return ;
	while (p->path[++i] && !child->path)
	{
		j = -1;
		while (child->info[++j])
		{
			if (!access(child->info[j], X_OK))
			{
				child->path = ft_strdup(child->info[j]);
				break ;
			}
			child->path = ft_strjoin(p->path[i], child->info[j]);
			if (!access(child->path, X_OK))
				break ;
			free(child->path);
			child->path = NULL;
		}
	}
	free_d2(p->path);
}

int	main(int argc, char *argv[], char *env[])
{
	t_prompt	p;

	(void)argv;
	//signal(SIGINT, sig_handler);
	//signal(SIGQUIT, sig_handler);
	if (argc != 1)
		return (1);
	set_str(&p);
	ft_putenv(env, &p);
	print_intro();
	prompt_io(&p);
	free_p(&p);
	return (0);
}
