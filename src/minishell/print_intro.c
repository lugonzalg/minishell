/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_intro.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:04 by lugonzal          #+#    #+#             */
/*   Updated: 2021/10/28 20:44:06 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"
#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern void	set_str(t_string *str)
{
	int	i;

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

extern void	print_intro(void)
{
	int		fd;
	char	*str;

	fd = open("./doc/intro.txt", O_RDONLY);
	printf("\n");
	while (1)
	{
		str = get_next_line(fd);
		if (!str)
			break ;
		printf("%s", str);
		free(str);
	}
	close(fd);
}
