/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilswc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 16:15:02 by mikgarci          #+#    #+#             */
/*   Updated: 2022/01/11 17:52:36 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

size_t	ft_strlen_d2(char **str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	**ft_putoffd2(char **z, size_t i)
{
	size_t	len;
	size_t	index;
	char	**temp;
	size_t	a;

	len = ft_strlen_d2(z);
	temp = ft_calloc(sizeof(char *), len);
	index = -1;
	a = 0;
	while (z[++index])
	{
		if (index != i)
		{
			temp[a] = ft_strdup(z[index]);
			a++;
		}
	}
	ft_free_d2(z);
	z = temp;
	return (temp);
}

int	ft_verify_info(char *pwd, char *comp)
{
	DIR				*dir;
	struct dirent	*info;
	char			*temp;

	dir = opendir(pwd);
	if (dir == NULL)
		return (0);
	info = readdir(dir);
	while (info != NULL)
	{
		temp = ft_strjoin(pwd, info->d_name);
		if (!ft_strncmp(temp, comp, ft_strlen(temp)))
		{
			free(temp);
			closedir(dir);
			return (1);
		}
		free(temp);
		info = readdir(dir);
	}
	closedir(dir);
	return (0);
}

char	**ft_add_info(char **dest, char **z, size_t *i)
{
	size_t	len;
	size_t	j;
	char	**temp;
	size_t	k;

	j = ft_strlen_d2(z);
	temp = ft_calloc(sizeof(char *), ft_strlen_d2(dest) + j);
	j = -1;
	while (++j < *i)
		temp[j] = ft_strdup(z[j]);
	k = j;
	len = -1;
	while (dest[++len])
		temp[j++] = ft_strdup(dest[len]);
	while (z[++k])
		temp[j++] = ft_strdup(z[k]);
	(*i) += ft_strlen_d2(dest) - 1;
	ft_free_d2(z);
	ft_free_d2(dest);
	return (temp);
}

void	ft_takedirinfo(size_t *i, t_wc *z)
{
	char	*pwd;
	char	**data;
	size_t	j;

	pwd = ft_strdup(z->info[*i]);
	data = ft_searchdata(pwd);
	j = 2;
	while (data[j])
	{
		if (!ft_comparedata(z, data[j]))
			data = ft_putoffd2(data, j);
		else
			j++;
	}
	if (data[2])
		ft_join_info(i, z, data);
	else
	{
		z->info = ft_putoffd2(z->info, *i);
		*i -= 1;
		ft_free_d2(data);
	}
	free(pwd);
}
