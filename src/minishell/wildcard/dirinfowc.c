/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirinfowc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 16:18:28 by mikgarci          #+#    #+#             */
/*   Updated: 2022/01/11 17:53:20 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

static size_t	ft_countfile(char *pwd)
{
	DIR				*dir;
	struct dirent	*info;
	size_t			index;

	dir = opendir(pwd);
	if (dir == NULL)
		return (0);
	index = 0;
	info = readdir(dir);
	while (info != NULL)
	{
		index++;
		info = readdir(dir);
	}
	closedir(dir);
	return (index);
}

char	**ft_searchdata(char *pwd)
{
	DIR				*dir;
	struct dirent	*info;
	size_t			index;
	char			**file;

	index = ft_countfile(pwd);
	file = ft_calloc(sizeof(char *), index + 1);
	dir = opendir(pwd);
	if (dir == NULL)
		return (NULL);
	index = 0;
	info = readdir(dir);
	while (info != NULL)
	{
		file[index] = ft_strdup(info->d_name);
		index++;
		info = readdir(dir);
	}
	closedir(dir);
	return (file);
}

void	ft_join_info(size_t *i, t_wc *z, char	**data)
{
	size_t	j;
	char	*temp;
	size_t	len;
	char	**dest;

	len = ft_strlen_d2(data) - 2;
	dest = ft_calloc(sizeof(char *), len + 1);
	temp = ft_strdup(z->info[*i]);
	len = 0;
	j = 1;
	while (data[++j])
	{
		dest[len] = ft_strjoin(temp, data[j]);
		len++;
	}
	free(temp);
	ft_free_d2(data);
	z->info = ft_add_info(dest, z->info, i);
}

void	ft_add_file(t_wc *z)
{
	size_t	i;
	char	*temp;

	i = -1;
	while (z->info[++i])
	{
		temp = ft_strjoin(z->info[i], "/");
		free(z->info[i]);
		z->info[i] = temp;
	}
}

void	ft_take_2in(t_wc *z)
{
	size_t	i;
	char	*temp;

	i = -1;
	while (z->info[++i])
	{
		temp = ft_strdup(z->info[i] + 2);
		free(z->info[i]);
		z->info[i] = temp;
	}
}
