/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 17:08:41 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/01 12:15:39 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/get_next_line.h"
#include "inc/minishell.h"
#include "inc/libft.h"
#include <stdlib.h>
#include <fcntl.h>

extern char	*ft_gnl_query(char *path, char *query)
{
	char	*line;
	char	*info;
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
		if (!info && !ft_strncmp(line, query, ft_strlen(query)))
			info = ft_strdup(line);
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

	line = ft_gnl_query(p->envpath, "PATH=");
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
