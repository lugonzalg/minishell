/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:38:35 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/13 17:15:55 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "inc/minishell.h"
#include "inc/libft.h"
#include <fcntl.h>

extern void	set_str(t_prompt *p)
{
	int	i;

	ft_memset(p, 0, sizeof(t_prompt));
	p->user = ft_strjoin(getenv("USER"), " \e[1;37mminishell \e[0;m% ");
	p->path = ft_split(getenv("PATH"), ':');
	i = -1;
	while (p->path[++i])
	{
		p->tmp = ft_strjoin(p->path[i], "/");
		free(p->path[i]);
		p->path[i] = p->tmp;
	}
}

extern void	set_child(t_prompt *p, t_child *child)
{
	size_t	i;

	i = 1;
	while (p->d2_prompt[i - 1])
		i++;
	ft_memset(child, 0, sizeof(t_child));
	child->fdpipe = (int **)malloc(sizeof(int *) * i);
	child->size[0] = i;
	while (i--)
	{
		child->fdpipe[i] = (int *)malloc(sizeof(int) * 2);
		pipe(child->fdpipe[i]);
	}
	child->fdpipe[child->size[0] - 1][1] = 2;
}

extern void	free_child(t_child *child)
{
	size_t	i;

	i = -1;
	i = -1;
	while (++i < child->size[0])
	{
		close(child->fdpipe[i][0]);
		close(child->fdpipe[i][1]);
		free(child->fdpipe[i]);
	}
	free(child->fdpipe);
	free(child->ttypath);
}

extern void	free_p(t_prompt *p)
{
	free_d2(p->path);
	free(p->user);
	free(p->envpath);
}
