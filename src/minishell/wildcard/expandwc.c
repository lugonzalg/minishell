/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandwc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 16:12:27 by mikgarci          #+#    #+#             */
/*   Updated: 2022/01/11 17:53:09 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

static int	ft_shortpwd(char *dir, char *wc, t_wc *z)
{
	if (dir && wc)
	{
		if (dir - z->temp <= wc - z->temp)
			ft_increasepath(z, dir, true);
		else
			ft_splitwc(z, dir, true);
		return (1);
	}
	if (wc)
		return (ft_lastwc(z));
	return (ft_lastdir(z));
}

static void	ft_searchpwd(t_wc *z)
{
	char	*dir;
	char	*wc;
	size_t	i;

	i = -1;
	while (z->pwd[++i])
	{
		dir = ft_strchr(z->temp, '/');
		if (dir && *dir == '/')
			dir++;
		wc = ft_strchr(z->temp, '*');
		if (!ft_shortpwd(dir, wc, z))
		{
			return ;
		}
		i += ft_strlen(&z->pwd[i]) - ft_strlen(z->temp) - 1;
	}
	if (z->pwd[ft_strlen(z->pwd) - 1] == '/')
	{
		free(z->pwd);
		free(z->temp);
	}
	return ;
}

static void	ft_initinfowc(t_child *child, size_t index, t_wc *z)
{
	bool	ver;

	ver = false;
	ft_memset(z, 0, sizeof(t_wc));
	z->pwd = ft_strdup(child->info[index]);
	z->i = index;
	z->info = ft_calloc(sizeof(char *), 2);
	if (child->info[index][0] != '/')
	{
		ver = true;
		z->info[0] = ft_strdup("./");
	}
	else
		z->info[0] = ft_strdup("");
	z->temp = ft_strdup(z->pwd);
	ft_searchpwd(z);
	if (z->info && z->info[0] && z->info[0][0])
	{
		if (ver)
			ft_take_2in(z);
		child->info = ft_add_info(z->info, child->info, &index);
	}
	else
		free(z->info);
}

void	ft_handlewc(t_child *child)
{
	size_t	index;
	t_wc	z;	

	index = -1;
	while (child->info[++index])
	{
		if (ft_strchr(child->info[index], '*'))
		{
			ft_initinfowc(child, index, &z);
		}
	}
	return ;
}

char	*ft_comparedata_util(t_wc *z, char *str, size_t *i)
{
	char	*temp;

	while ((z->compwc[*i] && !z->inout[1])
		|| *i < (ft_strlen_d2(z->compwc) - 1))
	{
		temp = ft_strnstr(str, z->compwc[*i], ft_strlen(str));
		if (!temp)
			return (0);
		str = temp;
		(*i)++;
	}
	return (str);
}
