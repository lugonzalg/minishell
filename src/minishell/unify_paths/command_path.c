/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 17:08:41 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/29 17:35:38 by mikgarci         ###   ########.fr       */
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
	int		fd;

	fd = open(path, O_RDONLY);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			return (NULL);
		if (!ft_strncmp(line, query, ft_strlen(query)))
			break ;
		free(line);
	}
	return (line);
}

extern char	**ft_setpath(t_prompt *p)
{
	char	**tmp;
	char	*line;
	int		i;

	line = ft_gnl_query(p->envpath, "PATH=");
	if (!line)
		return (NULL);
	free(line);
	tmp = ft_split(line + 5, ':');
	i = -1;
	while (tmp[++i])
	{
		tmp[i][ft_strlen(tmp[i])] = 0;
		line = ft_strjoin(tmp[i], "/");
		free(tmp[i]);
		tmp[i] = line;
	}
	return (tmp);
}