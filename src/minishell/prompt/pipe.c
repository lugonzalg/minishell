/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 20:26:59 by lugonzal          #+#    #+#             */
/*   Updated: 2022/01/03 20:33:40 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include <stdlib.h>

static void	ft_handle_pipe(t_child *child)
{
	size_t	i;

	i = -1;
	while (++i < child->size[0])
	{
		if (i != child->id)
			close(child->fdpipe[i][0]);
		if (i != child->id + 1)
			close(child->fdpipe[i][1]);
	}
	if (child->id || child->redir[0])
	{
		dup2(child->fdpipe[child->id][0], 0);
		close(child->fdpipe[child->id][0]);
	}
	if (child->id < child->size[0] - 2 || child->redir[1])
	{
		dup2(child->fdpipe[child->id + 1][1], 1);
		close(child->fdpipe[child->id + 1][1]);
	}
}

extern void	ft_multipipe(t_child *child)
{
	if (child->redir[2])
		exit(129);
	ft_handle_pipe(child);
	if (child->path)
		execve(child->path, child->info, NULL);
	if (!child->path || access(child->path, X_OK))
		exit(127);
	exit(126);
}
