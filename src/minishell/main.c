/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 22:28:39 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/18 18:30:37 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "inc/libft.h"
#include "inc/ft_printf.h"
#include "inc/minishell.h"

extern void	free_d2(char **dat)
{
	int	i;

	i = -1;
	while (dat[++i])
		free(dat[i]);
	free(dat);
	dat = NULL;
}

int	trim_path(t_child *child, int j)
{
	char	**tmp;
	int		i;

	tmp = ft_split(child->info[j], '/');
	i = 0;
	while (tmp[i])
		i++;
	free(child->info[j]);
	child->info[j] = ft_strdup(tmp[i - 1]);
	i = -1;
	free_d2(tmp);
	return (j);
}

extern int	command_pos(t_prompt *p, t_child *child)
{
	int	i;
	int	j;

	i = -1;
	while (p->path[++i])
	{
		j = -1;
		while (child->info[++j])
		{
			if (!access(child->info[j], X_OK))
			{
				child->path = ft_strdup(child->info[j]);
				return (trim_path(child, j));
			}
			child->path = ft_strjoin(p->path[i], child->info[j]);
			if (!access(child->path, X_OK))
				return (j);
			free(child->path);
		}
	}
	return (-1);
}

int	main(int argc, char *argv[], char *env[])
{
	t_prompt	p;

	(void)argv;
	if (argc != 1)
		return (1);
	set_str(&p);
	ft_putenv(env, &p);
	print_intro();
	prompt_io(&p);
	free_p(&p);
	return (0);
}
