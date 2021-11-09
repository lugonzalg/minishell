/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:37:46 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/09 20:07:01 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "inc/minishell.h"
#include "inc/libft.h"
#include "inc/ft_printf.h"
#include "inc/get_next_line.h"
#include <readline/readline.h>
#include <readline/history.h>


void	check_redir(t_string *str, t_child *child)
{
	str->tmp = str->d2_prompt[child->id];
	//USEFULL??
	if (ft_strchr(str->tmp, INPUT))
		child->redir[0] = 1;
	if (ft_strchr(str->tmp, OUTPUT))
		child->redir[1] = 1;
	child->info = ft_split(str->d2_prompt[child->id], ' ');	
	while (child->info[child->size[1]])
		child->size[1]++;
	child->size[2] = command_pos(str, child); //FIND COMMAND POS
	//USEFULL??
	if (child->redir[0] || child->redir[1])
		unify_fdio(child);
	unify_cmd(child);
}
//FALTA REDIRIGIR LOS FDs DE LOOS ARCHIVOS TEMPORALES (SI HAY) AL FDPIPES, Y METER EL PATH EXACTO EN LA EXTRUCTURA
void	multipipe(t_child *child)
{
	int	i;

	i = 0;
	while (i < child->size[0])
	{
		if (i != child->id)
			close(child->fdpipe[i][0]);
		if (i + 1 != child->id)
			close(child->fdpipe[i][1]);
		i++;
	}
	dup2(child->fdpipe[child->id][0], 0);
	close(child->fdpipe[child->id][0]);
	dup2(child->fdpipe[child->id + 1][1], 1);
	close(child->fdpipe[child->id + 1][1]);
   	execve(child->path, child->info, NULL);
}

static void	restart_data(t_child *child)
{
	size_t	i;

	ft_memset(child->size, 0, sizeof(short int) * 4);
	ft_memset(child->redir, 0, sizeof(short int) * 4);
	i = -1;
	while (child->info[++i])
	{
		free(child->info[i]);
		child->info[i] = NULL;
	}
	free(child->info);
	child->info = NULL;
	free(child->path);
	child->path = NULL;
}

static void	process_io(t_string *str)
{
	int	i;
	int	id;
	t_child	child;
	int a;

	str->d2_prompt = ft_split(str->prompt, '|');
	set_child(str, &child);
	a = child.size[0];
	i = -1;
	while (str->d2_prompt[++i])
	{
		child.id = i;
		check_redir(str, &child);
		if (ft_checkbuiltins(child.info[0]))
			ft_builtins(&child, str);
		else
		{
			id = fork();
			if (id == 0)
				multipipe(&child);
			else
				wait(NULL);
		}
		restart_data(&child);
	}
}

extern void	prompt_io(t_string *str)
{
	while (1)
	{
		str->prompt = readline(str->user);
		add_history(str->prompt);
		process_io(str);
		//FREE STRUCTURES
		//FREE STRUCTURES
		free(str->prompt);
	}
	rl_clear_history();
}
