/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fd_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 20:19:51 by lugonzal          #+#    #+#             */
/*   Updated: 2022/01/03 20:33:38 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	ft_redir_case(char *redir, char redir_in)
{
	*redir = '<';
	if (redir_in == '<')
		*redir = '>';
}

static int	ft_fd_redir_case(char redir, size_t *i, t_child *child)
{
	char	*n_str;

	n_str = (char *)ft_calloc(sizeof(char), 4);
	ft_memset(n_str, redir, 3);
	if (!ft_strncmp(child->info[*i], n_str, 3))
	{
		if (!child->info[*i + 1])
			*i += 1;
		free(n_str);
		return (-1);
	}
	free(n_str);
	return (0);
}

static int	ft_fd_name(t_child *child, size_t *i, char redir)
{
	char	d_redir;

	ft_redir_case(&d_redir, redir);
	if (ft_strchr(child->info[*i], redir) && child->info[*i + 1]
		&& !ft_strncmp(child->info[*i], child->info[*i + 1], 1))
		return (-1);
	else if (ft_strchr(child->info[*i], redir)
		&& ft_strlen(child->info[*i]) > 2)
	{
		if (ft_fd_redir_case(redir, i, child))
			return (-1);
	}
	else if (ft_strchr(child->info[*i], redir) && child->info[*i + 1]
		&& ft_strchr(child->info[*i + 1], d_redir))
	{
		*i += 1;
		return (-1);
	}
	return (1);
}

extern int	ft_fdcheck(t_child *child, char redir)
{
	size_t	i;
	int		fd;
	char	*fd_err;

	i = -1;
	while (child->info[++i])
	{
		fd_err = "newline";
		if (!ft_strchr(child->info[i], redir))
			continue ;
		fd = ft_fd_name(child, &i, redir);
		if (fd == -1)
		{
			fd_err = ft_trim_error(child->info[i], redir, 0);
			printf("minishell: syntax error near unexpected token `%s'\n",
				fd_err);
			free(fd_err);
			ft_free_d2(child->info);
			child->info = NULL;
			return (ft_go_exit(258));
		}
	}
	return (0);
}
