/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:34:46 by mikgarci          #+#    #+#             */
/*   Updated: 2021/12/08 20:39:17 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

extern char	**ft_realloc_child(char **temp)
{
	int		size;
	char	**d2;
	int		index;

	size = 0;
	while (temp[size])
		size++;
	size += 1;
	d2 = (char **)ft_calloc(sizeof(char *), size + 1);
	d2[0] = ft_strdup("echo");
	size = 1;
	index = 0;
	while (temp[index])
	{
		d2[size] = ft_strdup(temp[index]);
		free(temp[index]);
		index++;
		size++;
	}
	d2[size] = NULL;
	free(temp);
	return (d2);
}

extern void	ft_echo(t_child *child)
{
	size_t	i;
	bool	nl;
	int		fd;

	fd = 1;
	nl = true;
	i = 0;
	if (child->redir[1] || child->id < child->size[0] - 2)
		fd = child->fdpipe[child->id + 1][1];
	if (child->info[1] && !ft_strncmp(child->info[1], "-n", 3))
	{
		nl = false;
		i++;
	}
	while (child->info[++i])
	{
		write(fd, child->info[i], ft_strlen(child->info[i]));
		if (i < child->size[1] - 1)
			write(fd, " ", 1);
	}
	if (nl)
		write(fd, "\n", 1);
}

static void	putpwdextra(char **str, char **pwd, char **temp, char **dest)
{
	int	len;

	len = ft_strlen(ft_strchr(*str, '/'));
	*pwd = ft_substr(*str, 0, ft_strlen(*str) - len);
	*dest = ft_strjoin("/", *pwd);
	free(*pwd);
	*pwd = ft_strjoin(*temp, *dest);
	free(*dest);
	free(*temp);
	*temp = *pwd;
	*str = ft_strchr(*str, '/');
}

static void	putpwdutils(char **str, char **pwd, char **temp, char **dest)
{
	int	len;

	if (**str == '.')
	{
		len = ft_strlen(ft_strrchr(*temp, '/'));
		*pwd = ft_substr(*temp, 0, ft_strlen(*temp) - len);
		//free(*temp);
		*temp = *pwd;
		if (!*temp[0])
		{
			free(*temp);
			*pwd = ft_strdup("/");
			return ;
		}
		*str = ft_strchr(*str, '/');
	}
	else
		putpwdextra(str, pwd, temp, dest);
}

void	ft_putpwd(char *str, t_prompt *p)
{
	char	*temp;
	char	*pwd;
	char	*dest;

	pwd = search_pwd(p);
	temp = ft_substr(pwd, 4, ft_strlen(pwd) - 5);
	free(pwd);
	while (str && *str)
	{
		if (*str == '/')
			str++;
		putpwdutils(&str, &pwd, &temp, &dest);
	}
	changepwd(pwd, p);
	free(pwd);
}
