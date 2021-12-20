/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:04 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/18 00:51:44 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

extern char	*ft_trim_error(char *info, char redir, int on)
{
	char	*msg;
	size_t	len;

	len = 2;
	if (!info || !redir)
		return (ft_strdup("newline"));
	if (on)
		len = ft_strchr(info, redir) - info;
	if (!on && *info != redir)
	{
		len = 0;
		while (ft_isalnum(info[len]))
			len++;
	}
	msg = ft_calloc(sizeof(char), ft_strlen(info));
	ft_memcpy(msg, info, len);
	return (msg);
}

extern int	ft_go_exit(int n)
{
	g_glob.error = n;
	return (n);
}

static int	ft_fd_name(t_child *child, size_t i, char redir)
{
	if (ft_strchr(child->info[i], redir)
		&& ft_strchr(child->info[i + 1], redir)
		&& (ft_strncmp(child->info[i], "<", 2)
			|| ft_strncmp(child->info[i], "<<", 3)
			|| ft_strncmp(child->info[i], ">", 2)
			|| ft_strncmp(child->info[i], ">>", 3)))
	{
		return (-1);
	}
	else if (!child->info[i + 1] && ft_strlen(child->info[i]) < 3)
		return (-1);
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
		if (!ft_strchr(child->info[i], redir))
			continue ;
		fd = ft_fd_name(child, i, redir);
		if (fd == -1)
		{
			fd_err = ft_trim_error(child->info[i + 1], redir, 0);
			printf("minishell: syntax error near unexpected token `%s'\n",
				fd_err);
			free(fd_err);
			return (ft_go_exit(258));
		}
	}
	return (0);
}

extern bool	ft_check_prompt(t_prompt *p)
{
	size_t	i;

	p->d2_prompt = ft_split_ptr(p->prompt, '|', ft_lenp);
	if (p->prompt && p->prompt[0])
		add_history(p->prompt);
	i = -1;
	while (p->d2_prompt[++i])
	{
		if (!ft_quote_error(p->d2_prompt[i]))
		{
			ft_free_d2(p->d2_prompt);
			free(p->prompt);
			return (true);
		}
	}
	ft_process_io(p);
	free(p->id);
	free(p->prompt);
	return (false);
}
