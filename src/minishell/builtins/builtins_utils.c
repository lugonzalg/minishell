/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikgarci <mikgarci@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:34:46 by mikgarci          #+#    #+#             */
/*   Updated: 2022/01/10 19:34:51 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

extern int	ft_nl_checker(char *str, bool *nl)
{
	size_t	i;

	i = 1;
	if (!str)
		return (0);
	if (!ft_strncmp(str, "-n", 2))
	{
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i])
			*nl = false;
		if (!str[i])
			return (1);
	}
	return (0);
}

void	ft_changepwd(char *str, t_prompt *p)
{
	char	*temp;

	temp = ft_strjoin("PWD=", str);
	ft_deletpwd(temp, p);
	free(temp);
}

extern int	ft_changediraux(t_child *child, t_prompt *p)
{
	char	*path;
	char	*temp;

	if ((child->info[1][0] == '~' && child->info[1][1] == '/') ||
			(child->info[1][0] == '~' && ft_strlen(child->info[1]) == 1))
	{
		temp = ft_strdup(child->info[1] + 1);
		path = ft_strjoin(p->home, temp);
		free(temp);
		if (!chdir(path))
		{
			ft_changepwd(path, p);
			free(path);
			return (0);
		}
		else
		{
			printf("minishell: cd: %s: No such file or directory\n", path);
			free(path);
			return (ft_go_exit(1));
		}
	}
	return (-1);
}

extern void	ft_deletenv_2(t_prompt *p)
{
	int		fd[2];
	char	*line;

	fd[0] = open(p->envpath, O_WRONLY | O_TRUNC);
	fd[1] = open(p->temppath, O_RDONLY);
	while (1)
	{
		line = get_next_line(fd[1]);
		if (!line)
			break ;
		write(fd[0], line, ft_strlen(line));
		free(line);
	}
	close(fd[0]);
	close(fd[1]);
	unlink(p->temppath);
}

extern int	ft_check_env(t_child *child, char *line, t_prompt *p)
{
	char		*str;
	int			a;
	static int	is;

	if (!line && !is)
	{
		p->sizenv++;
		return (0);
	}
	if (!line && is)
	{
		is = 0;
		return (1);
	}
	str = ft_strchr(line, '=');
	a = ft_strncmp(child->info[1], line, str - line + 1);
	if (!a)
		is = 1;
	return (a);
}
