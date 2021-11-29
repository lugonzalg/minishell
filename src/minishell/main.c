/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 22:28:39 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/29 18:24:43 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/ft_printf.h"
#include "inc/libft.h"
#include <signal.h>
#include <stdlib.h>

int	ft_errorcheck(char *str)
{
	int	doub[2];
	int	simp[2];

	ft_memset(doub, 0, 2 * sizeof(int));
	ft_memset(simp, 0, 2 * sizeof(int));
	while (*str)
	{
		if (*str == '\'')
		{
			simp[0]++;
			if (simp[0] % 2 == 1 && !doub[1])
				simp[1] = 1;
			if (simp[0] % 2 == 0 && simp[1])
			{
				if (doub[0] % 2 == 1)
					return (0);
				simp[1] = 0;
			}
		}
		if (*str == '\"')
		{
			doub[0]++;
			if (doub[0] % 2 == 1 && !simp[1])
				doub[1] = 1;
			if (doub[0] % 2 == 0 && doub[1])
			{
				if (simp[0] % 2 == 1)
					return (0);
				doub[1] = 0;
			}
		}
		str++;
	}
	if (doub[0] % 2 == 1 || simp[0] % 2 == 1)
		return (0);
	return (1);
}

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
	
	g_glob = ft_calloc(sizeof(t_global), 1);
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
