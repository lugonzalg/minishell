/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extrawc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 16:21:15 by mikgarci          #+#    #+#             */
/*   Updated: 2022/01/11 17:52:58 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

void	ft_verifyd2(t_wc *z)
{
	size_t			i;
	DIR				*dir;

	i = 0;
	while (z->info[i])
	{
		dir = opendir(z->info[i]);
		if (dir == NULL)
			z->info = ft_putoffd2(z->info, i);
		else
		{
			closedir(dir);
			i++;
		}
	}
}

void	ft_initinout(t_wc *z, char *str)
{
	size_t	i;

	i = ft_strlen(str);
	z->inout = ft_calloc(sizeof(int), 2);
	if (str[0] != '*')
		z->inout[0] = 1;
	if (str[i - 1] != '*')
		z->inout[1] = 1;
}

size_t	ft_lastcompare(t_wc *z, char *str, size_t i)
{
	if (z->inout[1])
	{
		if (ft_strlen(z->compwc[i]) > ft_strlen(str))
			return (0);
		while (ft_strlen(z->compwc[i]) < ft_strlen(str))
			str++;
		if (ft_strncmp(str, z->compwc[i], ft_strlen(z->compwc[i])))
			return (0);
	}
	if (!str)
		return (0);
	return (1);
}

int	ft_lastwc(t_wc *z)
{
	ft_splitwc(z, NULL, false);
	free(z->temp);
	free(z->pwd);
	return (0);
}

int	ft_lastdir(t_wc *z)
{
	char	*str;

	str = ft_strchr(z->temp, '/');
	if (str)
	{
		while (str)
		{
			str++;
			ft_increasepath(z, str, true);
			str = z->temp;
			str = ft_strchr(z->temp, '/');
		}
	}
	ft_increasepath(z, NULL, false);
	free(z->temp);
	free(z->pwd);
	return (0);
}
