/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 19:01:38 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/28 00:46:09 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "inc/libft.h"
#include "inc/minishell.h"

extern char	*ft_find_quote(const char *s)
{
	char	*s_quote;
	char	*d_quote;

	s_quote = ft_strchr(s, '\'');
	d_quote = ft_strchr(s, '\"');
	if (!s_quote && d_quote)
		return (d_quote);
	else if (s_quote && !d_quote)
		return (s_quote);
	else if (!s_quote && !d_quote)
		return (NULL);
	else if (s_quote < d_quote)
		return (s_quote);
	else if (d_quote < s_quote)
		return (d_quote);
	return (NULL);
}
