/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 22:28:39 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/12 19:29:28 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "inc/libft.h"
#include "inc/ft_printf.h"
#include "inc/minishell.h"

void free_d2_data(char **dat)
{
	int	i;

	i = -1;
	while (dat[++i])
		free(dat[i]);
	free(dat);
}

int		trim_path(t_child *child, int b)
{
	char	**tmp;
	int		i;

	tmp = ft_split(child->info[b], '/');
	i = 0;
	while (tmp[i])
		i++;
	free(child->info[b]);
	child->info[b] = ft_strdup(tmp[i - 1]);
	i = -1;
	while (tmp[++i])
		free(tmp[i]);
	free(tmp);
	return (b);
}

extern int	command_pos(t_prompt *p, t_child *child)
{
	int 	a;
	int 	b;

	a = 0;
	while (p->path[a])
	{
		b = 0;
		while (child->info[b])
		{
			if (!access(child->info[b], X_OK))
			{
				child->path = ft_strdup(child->info[b]);
				return (trim_path(child, b));
			}
			child->path = ft_strjoin(p->path[a], child->info[b]);
			if (!access(child->path, X_OK))
				return (b);
		//	free(child->path);
			b++;
		}
		a++;
	}
	return (-1);
}

int	main(int argc, char *argv[], char *env[])
{
	t_prompt	p;
	(void)argc;
	(void)argv;

	set_str(&p);//LO HE PUESTO AQUI, ANTES ESTABA AL RPINCIPIO DE LA FUNCION PROMPT_IO
	ft_putenv(env, &p);
	print_intro();
	prompt_io(&p);
	//free_p(&p);
	return (0);
}
