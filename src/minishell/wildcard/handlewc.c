/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlewc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:40:11 by mikgarci          #+#    #+#             */
/*   Updated: 2022/01/11 17:52:48 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

int	ft_increasepath_util(t_wc *z, char *dir, bool ver)
{
	free(dir);
	if (ver)
		ft_verifyd2(z);
	return (1);
}

int	ft_increasepath(t_wc *z, char *dir, bool ver)
{
	char	*dest;
	size_t	i;

	dir = ft_substr(z->temp, 0, ft_strlen(z->temp) - ft_strlen(dir));
	dest = ft_substr(z->temp, ft_strlen(dir),
			ft_strlen(z->temp) - ft_strlen(dir));
	free(z->temp);
	z->temp = dest;
	i = -1;
	while (z->info[++i])
	{
		dest = ft_strjoin(z->info[i], dir);
		if (ft_verify_info(z->info[i], dest) || !ft_strncmp(dest, "/", 2))
		{
			free(z->info[i]);
			z->info[i] = dest;
		}
		else
		{
			z->info = ft_putoffd2(z->info, i);
			i -= 1;
			free(dest);
		}
	}
	return (ft_increasepath_util(z, dir, ver));
}

size_t	ft_comparedata(t_wc *z, char *str)
{
	size_t	i;

	if (str[0] == '.' && (!z->compwc[0]
			|| (z->compwc[0] && z->compwc[0][0] != '.')))
		return (0);
	if (!z->compwc[0])
		return (1);
	i = 0;
	if (z->inout[0])
	{
		if (ft_strncmp(str, z->compwc[0], ft_strlen(z->compwc[0])))
			return (0);
		str += ft_strlen(z->compwc[0]);
		i++;
	}
	str = ft_comparedata_util(z, str, &i);
	if (!ft_lastcompare(z, str, i))
		return (0);
	return (1);
}

static	void	ft_splitwc_util(t_wc *z, char *dir, bool ver)
{
	char	*str;

	if (ver)
	{
		str = ft_substr(z->temp, 0, ft_strlen(z->temp) - ft_strlen(dir) - 1);
		z->compwc = ft_split(str, '*');
		ft_initinout(z, str);
		free(str);
		str = ft_strdup(dir);
		free(z->temp);
		z->temp = str;
	}
	else
	{
		str = ft_strdup(z->temp);
		z->compwc = ft_split(str, '*');
		ft_initinout(z, str);
		free(str);
		str = ft_strdup("");
		free(z->temp);
		z->temp = str;
	}
}

int	ft_splitwc(t_wc *z, char *dir, bool ver)
{
	size_t	i;

	ft_splitwc_util(z, dir, ver);
	i = -1;
	while (z->info[++i])
		ft_takedirinfo(&i, z);
	if (ver)
	{
		ft_verifyd2(z);
		ft_add_file(z);
	}
	ft_free_d2(z->compwc);
	free(z->inout);
	return (1);
}
