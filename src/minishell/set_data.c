/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:38:35 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/07 19:31:13 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "inc/minishell.h"
#include "inc/libft.h"
#include <fcntl.h>

extern void	restart_data(t_child *child)
{
	ft_memset(&child->size[1], 0, sizeof(size_t) * 3);
	ft_memset(child->redir, false, sizeof(bool) * 2);
	free_d2(child->info);
	free(child->path);
}

extern void	set_child(t_prompt *p, t_child *child)
{
	size_t	i;
	char	*tmp;

	i = 1;
	while (p->d2_prompt[i - 1])
	{
		tmp = ft_strtrim(p->d2_prompt[i - 1], " ");
		free(p->d2_prompt[i - 1]);
		p->d2_prompt[i - 1] = tmp;
		i++;
	}
	ft_memset(child, 0, sizeof(t_child));
	child->fdpipe = (int **)malloc(sizeof(int *) * i);
	child->size[0] = i;
	p->id = (pid_t *)malloc(sizeof(pid_t) * child->size[0]);
	i = -1;
	while (++i < child->size[0])
	{
		child->fdpipe[i] = (int *)malloc(sizeof(int) * 2);
		pipe(child->fdpipe[i]);
	}
}

extern void	free_child(t_child *child)
{
	size_t	i;

	i = -1;
	i = -1;
	while (++i < child->size[0])
	{
		if (child->fdpipe[i][0] != 0)
			close(child->fdpipe[i][0]);
		if (child->fdpipe[i][1] != 1)
			close(child->fdpipe[i][1]);
		free(child->fdpipe[i]);
	}
	free(child->fdpipe);
	free(child->ttypath);
}

extern void	free_p(t_prompt *p)
{
	free(p->user);
	unlink(p->envpath);
	free(p->envpath);
	free(p->builtpath);
	free(p->home);
	free(p->prompt);
}

extern void	free_d2(char **dat)
{
	int	i;

	i = -1;
	if (!dat)
		return ;
	while (dat[++i])
	{
		free(dat[i]);
		dat[i] = NULL;
	}
	free(dat);
}
