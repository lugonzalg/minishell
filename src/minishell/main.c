/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 22:28:39 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/26 21:17:25 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "inc/libft.h"
#include "inc/ft_printf.h"
#include "inc/minishell.h"
#include <signal.h>
#include <fcntl.h>
#include <term.h>

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

char	**ft_setpath(t_prompt *p)
{
	char	**path;
	char	*tmp;
	int		fd;
	int		i;

	fd = open(p->envpath, O_RDONLY); 
	path = ft_calloc(sizeof(char *), p->sizenv + 1);
	i = -1;
	while (path[++i])
	{
		tmp = ft_strjoin(path[i], "/");
		free(p->path[i]);
		path[i] = tmp;
	}
	path[i] = NULL;
	return (path);
}

extern void	command_pos(t_prompt *p, t_child *child)
{
	int	i;
	int	j;

	i = -1;
	p->path = ft_setpath(p);
	while (p->path[++i])
	{
		j = -1;
		while (child->info[++j])
		{
			if (!access(child->info[j], X_OK))
			{
				child->path = ft_strdup(child->info[j]);
				trim_path(child, j);
				break ;
			}
			child->path = ft_strjoin(p->path[i], child->info[j]);
			if (!access(child->path, X_OK))
				break ;
			free(child->path);
		}
	}
	free_d2(p->path);
}

int	main(int argc, char *argv[], char *env[])
{
	t_prompt	p;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
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
