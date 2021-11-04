/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:37:46 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/04 20:40:51 by lugonzal         ###   ########.fr       */
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

void	unify_fdinput(t_child *child)
{
	int		fd[2];
	int		i;
	char	*line;

	i = -1;
	fd[0] = open(".tmp_IN", O_RDWR |O_TRUNC |O_CREAT, 0755);
	while (child->info[++i])
	{
		if (ft_strlen(child->info[i]) == 1 && ft_strchr(child->info[i], INPUT))
		{
			fd[1] = open(child->info[++i], O_RDONLY);
			while (1)
			{
				line = get_next_line(fd[1]);
				if (!line)
					break ;
				write(fd[0], line, ft_strlen(line));
				if (!ft_strchr(line, 10))
					write(fd[0], NL, 1);
				free(line);
				close(fd[1]);
			}
		}
	}
	close(fd[0]);
}

void	unify_fdoutput(t_child *child)
{
	int	fd[2];
	int	i;
	int	j;

	fd[0] = open(".tmp_OUT", O_RDWR |O_TRUNC |O_CREAT, 0755);
	child->fdout = (int *)malloc(sizeof(int) * child->redir[1]);
	i = -1;
	j = 0;
	while (child->info[++i])
	{
		if (ft_strlen(child->info[i]) == 1 && ft_strchr(child->info[i], OUTPUT))
		{
			fd[1] = open(child->info[++i], O_WRONLY |O_TRUNC |O_CREAT, 0755);
			child->fdout[j++] = fd[1];
			close(fd[1]);
		}
	}
	close(fd[0]);
}

void	cmd_size(t_child *child)
{
	int	pos;

	pos = child->size[2];
	while (child->info[pos])
	{
		if ((ft_strchr(child->info[pos + 1], INPUT)
				|| ft_strchr(child->info[pos + 1], OUTPUT))
				&& ft_strlen(child->info[pos + 1]) == 1)
			break ;
		pos++;
	}
	child->size[3] = pos;
}

void	unify_cmd(t_child *child)
{
	char	**temp;
	int		index;

	index = 0;
	cmd_size(child);
	temp = (char **) malloc(sizeof(char *) * child->size[3] - child->size[2] + 2);
	while (child->size[2] + index <= child->size[3])
	{
		temp[index] = ft_strdup(child->info[index + child->size[2]]);
		index++;
	}
	index = -1;
	while (child->info[++index])
		free(child->info[index]);
	free(child->info);
	child->info = temp;
}

void	check_redir(t_string *str, t_child *child)
{
	str->tmp = str->d2_prompt[child->id];
	while (1)
	{
			str->tmp = ft_strchr(str->tmp, INPUT);
			if (!str->tmp)
				break ;
			str->tmp++;
			child->redir[0]++;
	}
	str->tmp = str->d2_prompt[child->id];
	while (1)
	{
			str->tmp = ft_strchr(str->tmp, OUTPUT);
			if (!str->tmp)
				break ;
			str->tmp++;
			child->redir[1]++;
	}
	child->info = ft_split(str->d2_prompt[child->id], ' ');	
	child->size[2] = command_pos(str, child); //FIND COMMAND POS
	if (child->redir[0])
		unify_fdinput(child);
	if (child->redir[1])
		unify_fdoutput(child);
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

static void	process_io(t_string *str)
{
	int	i;
	int	id;
	t_child	child;

	ft_memset(&child, 0, sizeof(t_string));
	str->d2_prompt = ft_split(str->prompt, '|');
	set_child(str, &child);
	i = 0;
	while (str->d2_prompt[i] && i < 1)
	{
		child.id = i;
		check_redir(str, &child);
		set_child(str, &child);
		id = fork();
		if (id == 0)
			multipipe(&child);
		else
			wait(NULL);
		i++;
	}
}

extern void	prompt_io(t_string *str)
{
	set_str(str);
	while (1)
	{
		str->prompt = readline(str->user);
		add_history(str->prompt);
		if (!ft_strncmp(str->prompt, "exit", 5))
			break ;
		process_io(str);
		free(str->prompt);
	}
	rl_clear_history();
}
