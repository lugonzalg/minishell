/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 13:37:46 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/03 15:30:45 by lugonzal         ###   ########.fr       */
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

void	unify_fdinput(t_string *str, t_child *child)
{
	int		fd[2];
	int		i;
	char	*line;

	i = -1;
	fd[0] = open("tmp_IN", O_RDWR |O_TRUNC |O_CREAT, 0755);
	while (++i != pos)
	{
		if (child->info[i] == INPUT)
		{
			fd[1] = open(child->info[++i])
			while (1)
			{
				line = get_next_line(fd[1]);
				if (!line)
					break ;
				write(fd[0], line, ft_strlen(line));
				close(fd[1]);
			}
		}
	}

}

void	unify_fdoutput(t_string *str, t_child *child)
{
	int	fd;

	fd = open("tmp_OUT", O_RDWR |O_TRUNC |O_CREAT, 0755);
}

void	check_redir(t_string *str, t_child *child)
{
	while (ft_strchr(str->d2_prompt[child->id], INPUT))
			child->redir[0]++;
	while (ft_strchr(str->d2_prompt[child->id], OUTPUT))
			child->redir[1]++;
	child->info = ft_split(str->d2_prompt[child->id], ' ');	
	child->pos = command_pos(str, child); //FIND COMMAND POS
	if (child->redir[0] > 1)
		unify_fdinput(str, child);
	if (child->redir[1] > 1)
		unify_fdoutput(str, child);
}

static void	sort_cmd(t_string *str, t_child *child)
{
	check_redir(str, child);
}

static void	process_io(t_string *str)
{
	int	i;
	t_child	child;

	ft_memset(&child, 0, sizeof(t_string));
	str->d2_prompt = ft_split(str->prompt, '|');
	set_child(&child, str);
	i = 0;
	while (str->d2_prompt[i] && i < 1)
	{
		child.id = i;
		sort_cmd(str, &child);
		//str->tmp = ft_strjoin(str->d2_prompt[0], str->path[i]);
		//if (!access(str->tmp, X_OK))
			//create_process(str);
		//else
		//	free(str->tmp);
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
