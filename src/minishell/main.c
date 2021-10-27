/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 22:28:39 by lugonzal          #+#    #+#             */
/*   Updated: 2021/10/27 17:57:39 by mikgarci         ###   ########.fr       */
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
#include "../libft/inc/libft.h"
#include "../libft/inc/get_next_line.h"
#include "../ft_printf/inc/ft_printf.h"
#include "../../inc/minishell.h"

static void	print_intro(t_string *str)
{
	int		fd;
	int		i;

	fd = open("./doc/intro.txt", O_RDONLY);
	printf("\n");
	while (1)
	{
		str->tmp = get_next_line(fd);
		if (!str->tmp)
			break ;
		printf("%s", str->tmp);
		free(str->tmp);
	}
	str->tmp = getenv("USER");	
	str->user = ft_strjoin(str->tmp, " \e[1;37mminishell \e[0;m% ");
	str->tmp = ft_strtrim(getenv("PATH"), "PATH=");
	str->path = ft_split(str->tmp, ':');
	free(str->tmp);
	i = -1;
	while (str->path[++i])
	{
		str->tmp = ft_strjoin(str->path[i], "/");
		free(str->path[i]);
		str->path[i] = str->tmp;
	}
	close(fd);
}

static void create_process()
{

}

static void	process_io(t_string *str)
{
	int	i;

	i = 0;
	str->d2_prompt = ft_split(str->prompt, 32);

	while (str->path[i])
	{
		str->tmp = ft_strjoin(str->d2_prompt[0], str->path[i]);
		if (!access(str->tmp, X_OK))
			create_process(str);
		else
			free(str->tmp);
		i++;
	}
}

static void	prompt_io(t_string *str)
{
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
int	main(int argc, char *argv[])
{
	t_string	str;
	(void)argc;
	(void)argv;

	print_intro(&str);
	prompt_io(&str);
	return (0);
}

//rl_replace_line("OLE", 2147483647);
//rl_redisplay();
//execve("/bin/ls", cmd, NULL); 
//int	fd;
//rl_on_new_line();
//printf("->%d\n", rl_on_new_line());
