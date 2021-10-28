/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 22:28:39 by lugonzal          #+#    #+#             */
/*   Updated: 2021/10/28 20:12:53 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "inc/libft.h"
#include "inc/get_next_line.h"
#include "inc/ft_printf.h"
#include "inc/minishell.h"

static void free_d2_data(char **dat)
{
	int	i;

	i = -1;
	while (dat[++i])
		free(dat[i]);
	free(dat);
}

static char	*find_fd(t_child *child, char *cmd, short int fd, short int pipe)
{
	char		**d2_cmd;
	int			i;
	short int	local_fd;
	char		*file;

	i = -1;
	d2_cmd = ft_split(cmd, ' ');
	while (d2_cmd[++i])
	{
		if (fd)
			local_fd = open(d2_cmd[i], O_RDWR |O_TRUNC |O_CREAT, 0755);
		else
			local_fd = open(d2_cmd[i], O_RDWR);
		if (local_fd > 2)
		{
			child->fd[pipe][fd] = local_fd;
			close(local_fd);
			file = ft_strdup(d2_cmd[i]);
			free_d2_data(d2_cmd);
			return (file);
		}
	}
	free_d2_data(d2_cmd);
	return (NULL);
}

static char	*trim_prompt (t_string *str, int i)
{
	char *tmp;

	str->tmp = ft_strtrim(str->d2_prompt[i], str->trim_file);
	free(str->trim_file);
	str->trim_file = NULL;
	free(str->d2_prompt[i]);
	str->d2_prompt[i] = NULL;
	tmp = ft_strtrim(str->tmp, " <>");
	free(str->tmp);
	return (tmp);
}

static void	set_child(t_child *child, t_string *str)
{
	int	i;

	i = 1;
	ft_memset(child, 0, sizeof(t_child));
	while (str->d2_prompt[i - 1] != NULL)
		i++;
	child->fd = ft_calloc(sizeof(int *), i);
	while ((i--))
	{
		child->fd[i] = ft_calloc(sizeof(int), 2);
		pipe(child->fd[i]);
	}
	child->fd[0][0] = 0;
	i = 0;
	while (str->d2_prompt[i] != NULL)
	{
		if (ft_strchr(str->d2_prompt[i], '>'))
			str->trim_file = find_fd(child, str->d2_prompt[i], 1, i);
		else if (ft_strchr(str->d2_prompt[i], '<'))
			str->trim_file = find_fd(child, str->d2_prompt[i], 0, i);
		if (str->trim_file)
		{
			str->d2_prompt[i] = trim_prompt(str, i);
			free(str->trim_file);
			str->trim_file = NULL;
		}
		i++;
	}
}

static void	process_io(t_string *str)
{
	int	i;
	t_child	child;

	ft_memset(str, 0, sizeof(t_string));
	str->d2_prompt = ft_split(str->prompt, '|');
	set_child(&child, str);
	i = 0;
	while (0 && str->path[i])
	{
		//str->tmp = ft_strjoin(str->d2_prompt[0], str->path[i]);
		//if (!access(str->tmp, X_OK))
			//create_process(str);
		//else
		//	free(str->tmp);
		//i++;
	}
}

static void	prompt_io(t_string *str)
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

int	main(int argc, char *argv[], char *env[])
{
	t_string	str;
	(void)argc;
	(void)argv;
	(void)env;

	print_intro();
	prompt_io(&str);
	//free_str(&str);
	return (0);
}

//rl_replace_line("OLE", 2147483647);
//rl_redisplay();
//execve("/bin/ls", cmd, NULL); 
//int	fd;
//rl_on_new_line();
//printf("->%d\n", rl_on_new_line());
