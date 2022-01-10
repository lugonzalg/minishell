/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:04 by lugonzal          #+#    #+#             */
/*   Updated: 2022/01/03 20:33:42 by lugonzal         ###   ########.fr       */
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
		if (!len)
			len = 2;
	}
	msg = ft_calloc(sizeof(char), ft_strlen(info) + 1);
	ft_memcpy(msg, info, len);
	return (msg);
}

extern int	ft_go_exit(int n)
{
	g_glob.error = n;
	return (n);
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
