/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:51:36 by lugonzal          #+#    #+#             */
/*   Updated: 2022/01/10 18:59:04 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

extern void	ft_deletpwd(char *str, t_prompt *p)
{
	int		fd[2];
	char	*line;

	fd[0] = open(p->envpath, O_RDONLY);
	fd[1] = open(p->temppath, O_WRONLY | O_CREAT, 0644);
	while (1)
	{
		line = get_next_line(fd[0]);
		if (!line)
			break ;
		if (ft_strncmp(str, line, 4))
			write(fd[1], line, ft_strlen(line));
		else
		{
			write(fd[1], str, ft_strlen(str));
			write(fd[1], "\n", 1);
		}
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	ft_deletenv_2(p);
}

extern char	*ft_search_pwd(t_prompt *p)
{
	int		fd[2];
	char	*line;

	fd[0] = open(p->envpath, O_RDONLY);
	fd[1] = open(p->temppath, O_WRONLY | O_CREAT, 0644);
	while (1)
	{
		line = get_next_line(fd[0]);
		if (!line)
			break ;
		if (!ft_strncmp("PWD=", line, 4))
			return (line);
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	return (NULL);
}

static void	ft_putpwdextra(char **str, char **pwd, char **temp, char **dest)
{
	int	len;

	len = ft_strlen(ft_strchr(*str, '/'));
	*temp = ft_substr(*str, 0, ft_strlen(*str) - len);
	*dest = ft_strjoin("/", *temp);
	free(*temp);
	*temp = ft_strjoin(*pwd, *dest);
	free(*dest);
	free(*pwd);
	*pwd = *temp;
	*str = ft_strchr(*str, '/');
}

static void	ft_putpwdutils(char **str, char **pwd, char **temp, char **dest)
{
	int	len;

	if (**str == '.')
	{
		len = ft_strlen(ft_strrchr(*temp, '/'));
		if (ft_strlen(*temp) - len == 0)
			len--;
		*temp = ft_substr(*pwd, 0, ft_strlen(*pwd) - len);
		free(*pwd);
		*pwd = *temp;
		if (!*pwd[0])
		{
			free(*pwd);
			*pwd = ft_strdup("/");
			return ;
		}
		*str = ft_strchr(*str, '/');
	}
	else
		ft_putpwdextra(str, pwd, temp, dest);
}

void	ft_putpwd(char *str, t_prompt *p)
{
	char	*temp;
	char	*pwd;
	char	*dest;

	pwd = ft_search_pwd(p);
	if (!pwd)
		pwd = getcwd(pwd, 2048);
	temp = ft_substr(pwd, 4, ft_strlen(pwd));
	free(pwd);
	pwd = temp;
	while (str && *str)
	{
		if (*str == '/')
			str++;
		ft_putpwdutils(&str, &pwd, &temp, &dest);
	}
	ft_changepwd(pwd, p);
	free(pwd);
}
