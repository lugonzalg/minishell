/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:38:35 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/05 19:15:14 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "inc/minishell.h"
#include "inc/libft.h"
#include <fcntl.h>

extern void	set_str(t_string *str)
{
	int	i;

	ft_memset(str, 0, sizeof(t_string));
	str->user = ft_strjoin(getenv("USER"), " \e[1;37mminishell \e[0;m% ");
	str->path = ft_split(getenv("PATH="), ':');
	i = -1;
	while (str->path[++i])
	{
		str->tmp = ft_strjoin(str->path[i], "/");
		free(str->path[i]);
		str->path[i] = str->tmp;
	}
	str->tmp = NULL;
}

extern void	set_child(t_string *str, t_child *child)
{
	int	i;

	i = 1;
	while (str->d2_prompt[i - 1])
		i++;
	ft_memset(child, 0, sizeof(t_child));
	child->fdpipe = (int **)malloc(sizeof(int *) * i);
	//BREAK CHECK I
	child->size[0] = i;
	while (i--)
	{
		child->fdpipe[i] = (int *)malloc(sizeof(int) * 2);
		pipe(child->fdpipe[i]);
	}
	child->tty = ttyslot();
	//BREAK CHECK CHILD->TTY
	child->ttypath = ttyname(child->tty);
	child->tty = open(child->ttypath, O_RDWR);
	//BREAK CHECK CHILD->TTY FD?
	close(child->tty);
	//CLOSE ??
//	close(child->fdpipe[0][0]);
//	printf("%d\n", child->tty);
//	child->fdpipe[0][0] = open("file1", O_RDONLY);
//	close(child->fdpipe[child->size[0] - 1][1]);
//	close(child->fdpipe[0][0]);
	//child->fdpipe[child->size[0] - 1][1] = child->tty;
//	child->fdpipe[1][1] = child->tty;
//	close(child->fdpipe[1][1]);
}
