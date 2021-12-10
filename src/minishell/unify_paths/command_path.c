/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 17:08:41 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/10 22:06:24 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

extern int	ft_putpath(t_child *child)
{
	if (child->info[0] && !access(child->info[0], X_OK))
		child->path = ft_strdup(child->info[0]);
	return (1);
}

static int	ft_check_even(char *str, char **s_ptr, char c)
{
	while (*str)
	{
		if (*str == c)
		{
			(*s_ptr) = str + 1;
			return (1);
		}
		str++;
	}
	return (0);
}

extern int	ft_quote_error(char *str)
{
	char	quote;

	while (*str)
	{
		if (*str == '\'' || *str == '\"')
		{
			quote = *str;
			if (!ft_check_even(str + 1, &str, *str))
			{
				printf("unclosed quotes %c\n", quote);
				return (0);
			}
			continue ;
		}
		str++;
	}
	return (1);
}

extern char	*ft_gnl_query(char *path, char *query)
{
	char	*line;
	char	*info;
	char	*pos;
	int		fd;

	fd = open(path, O_RDONLY);
	info = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line && !info)
			return (NULL);
		else if (info && !line)
			break ;
		pos = ft_strchr(line, '=');
		line[pos - line] = 0;
		if (!info && !ft_strncmp(line, query, ft_strlen(line))
			&& ft_strlen(line) == ft_strlen(query))
		{
			line[pos - line] = '=';
			info = ft_strdup(line);
		}
		free(line);
	}
	return (info);
}

extern char	**ft_setpath(t_prompt *p)
{
	char	**tmp;
	char	*line;
	char	*nl;
	int		i;

	line = ft_gnl_query(p->envpath, "PATH");
	if (!line)
		return (NULL);
	nl = ft_strchr(line, 10);
	if (nl)
		*nl = 0;
	tmp = ft_split(line + 5, ':');
	free(line);
	i = -1;
	while (tmp[++i])
	{
		line = ft_strjoin(tmp[i], "/");
		free(tmp[i]);
		tmp[i] = line;
	}
	return (tmp);
}
