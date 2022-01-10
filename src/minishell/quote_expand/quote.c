/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 18:48:29 by lugonzal          #+#    #+#             */
/*   Updated: 2022/01/03 20:34:26 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "inc/minishell.h"
#include "inc/libft.h"

extern char	*ft_quote_case(t_prompt *p, char *str)
{
	char	squote;
	char	dquote;
	size_t	j;

	j = -1;
	squote = 0;
	dquote = 0;
	while (str[++j])
	{
		if (!squote && str[j] == '\'')
			squote = dquote + 1;
		else if (!dquote && str[j] == '\"')
			dquote = squote + 1;
		else if (((dquote == 1 && squote == 2) || !squote) && str[j] == '$'
			&& ft_isalpha(str[j + 1]))
			str = ft_joinfr(p, str, &j);
		else if (squote && str[j] == '\'')
			squote = 0;
		else if (dquote && str[j] == '\"')
			dquote = 0;
	}
	return (str);
}

extern char	*ft_quote_clean(char *str)
{
	char	quote[2];
	size_t	len;
	size_t	j;

	j = -1;
	ft_memset(quote, 0, sizeof(char) * 2);
	while (str[++j])
	{
		if (!quote[0] && str[j] == '\'')
			quote[0] = quote[1] + 1;
		else if (!quote[1] && str[j] == '\"')
			quote[1] = quote[0] + 1;
		if (quote[1] || quote[0])
		{
			len = ft_query_len(&str[j], str[j]);
			ft_memcpy(&str[j], &str[j + 1], len + 1);
			j += len;
			ft_memcpy(&str[j - 1], &str[j + 1], ft_strlen(str + j) + 1);
			j -= 2;
			ft_memset(quote, 0, sizeof(char) * 2);
		}
	}
	return (str);
}
