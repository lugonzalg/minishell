/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 18:13:53 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/10 20:13:19 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check(char *s1, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == *s1)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		i;

	if (!set || !s1)
		return ((char *)s1);
	while (*s1)
	{
		if (ft_check((char *)s1, (char *)set))
			s1++;
		else
			break ;
	}
	i = ft_strlen(s1) - 1;
	while (*s1 && s1[i])
	{
		if (ft_check((char *)&s1[i], (char *)set))
			i--;
		else
			break ;
	}
	if (i <= 0)
		i = 1;
	str = ft_substr(s1, 0, i + 1);
	return (str);
}
