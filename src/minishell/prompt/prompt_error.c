/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 17:38:48 by lugonzal          #+#    #+#             */
/*   Updated: 2022/01/10 19:08:48 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

extern void	ft_resize_prompt(t_prompt *p, ssize_t *n)
{
	char	*line;

	*n = 0;
	line = readline("> ");
	if (!line)
		return ;
	p->tmp = ft_strjoin(p->prompt, " ");
	free(p->prompt);
	p->prompt = p->tmp;
	p->tmp = ft_strjoin(p->prompt, line);
	free(line);
	free(p->prompt);
	p->prompt = p->tmp;
}

static void	ft_print_error(char str_prompt)
{
	printf("minishell: syntax error near unexpected token `%c\n", str_prompt);
}

extern int	ft_prompt_error(t_prompt *p)
{
	ssize_t	n;
	size_t	i;

	i = -1;
	n = 0;
	while (p->prompt[++i])
	{
		if ((n == 0 && ft_strchr("|&;", p->prompt[i]))
			|| (i != 0 && ft_strchr("|&;", p->prompt[i])
				&& ft_strchr("|&;", p->prompt[i - 1])
				&& p->prompt[i] != p->prompt[i - 1]))
		{
			ft_print_error(p->prompt[i]);
			return (1);
		}
		if (p->prompt[i] == S_QUOTE || p->prompt[i] == S_QUOTE)
			i += ft_query_len(p->prompt + i, p->prompt[i]);
		if (!ft_strchr("|&;", p->prompt[i]) && p->prompt[i] != 32)
			n++;
		if (n != 0 && ft_strchr("|&", p->prompt[i]) && !p->prompt[i + 1])
			ft_resize_prompt(p, &n);
		if (n && ft_strchr("|&", p->prompt[i]))
			n = 0;
	}
	return (0);
}
