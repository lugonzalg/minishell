/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unify.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 21:18:51 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/26 21:20:12 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "inc/get_next_line.h"
#include "inc/libft.h"
#include "inc/minishell.h"

extern void	cmd_size(t_child *child)
{
	int	pos;

	pos = 1;
	child->size[2] = child->size[1];
	while (child->info[pos])
	{
		if ((ft_strchr(child->info[pos - 1], INPUT)
				|| ft_strchr(child->info[pos - 1], OUTPUT))
			&& ft_strlen(child->info[pos - 1]) < 3)
		{
			*child->info[pos] = 0;
			*child->info[pos - 1] = 0;
			child->size[1] -= 2;
		}
		pos++;
	}
}

static void	resize_cat(t_child *child)
{
	char	**resize;
	size_t	i;

	i = 1;
	resize = ft_calloc(sizeof(char *), child->size[1] + 1);
	resize[0] = ft_strdup(child->info[0]);
	resize[1] = ft_strdup(".here_doc");
	while (child->info[++i])
		resize[i] = ft_strdup(child->info[i]);
	free_d2(child->info);
	child->info = resize;
	child->size[2]--;
	child->redir[2] = true;
	child->redir[0] = true;
}

static void	here_doc(t_child *child, char *key)
{
	char	*line;
	int		fd;
	char	*key_nl;

	fd = open(".here_doc", O_RDWR | O_TRUNC | O_CREAT, 0644);
	close(child->fdpipe[child->id + 1][1]);
	child->fdpipe[child->id + 1][1] = fd;
	key_nl = ft_strjoin(key, "\n");
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
			break ;
		if (!ft_strncmp(key_nl, line, ft_strlen(line)))
		{
			free(line);
			break ;
		}
		write(child->fdpipe[child->id + 1][1], line, ft_strlen(line));
		free(line);
	}
	free(key_nl);
	resize_cat(child);
}

extern void	unify_fdio(t_child *child)
{
	int		fd;
	size_t	i;

	i = -1;
	while (child->info[++i])
	{
		if (ft_strchr(child->info[i], OUTPUT))
		{
			if (ft_strlen(child->info[i]) == 1)
				fd = open(child->info[++i], O_RDWR | O_TRUNC | O_CREAT, 0644);
			else
				fd = open(child->info[++i], O_RDWR | O_TRUNC | O_CREAT, 0644);
			close(child->fdpipe[child->id + 1][1]);
			child->fdpipe[child->id + 1][1] = fd;
		}
		if (ft_strchr(child->info[i], INPUT))
		{
			if (ft_strlen(child->info[i]) == 1)
				fd = open(child->info[++i], O_RDONLY);
			else
				here_doc(child, child->info[i + 1]);
			close(child->fdpipe[child->id][0]);
			child->fdpipe[child->id][0] = fd;
		}
	}
}

static char	*expand_var(t_prompt *p, t_child *child, size_t i)
{
	char	*line;
	int		fd;
	char	*var;

	child->builtin = true;
	fd = open(p->envpath, O_RDONLY);
	if (child->info[0][0] == '$')
		child->echo = true;
	line = ft_strtrim(child->info[i], "$\"");
	free(child->info[i]);
	child->info[i] = ft_strjoin(line, "=");
	free(line);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (!ft_strncmp(line, child->info[i], ft_strlen(child->info[i])))
		{
			line[ft_strlen(line) - 1] = 0;
			close(fd);
			var = ft_strtrim(line, child->info[i]);
			free(line);
			return (var);
		}
		free(line);
	}
	close(fd);
	return (NULL);
}

char	**ft_realloc_child(char **temp)
{
	int	size;
	char	**d2;
	int	index;

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

void	unify_cmd(t_prompt *p, t_child *child)
{
	char	**temp;
	size_t	index;
	size_t	i;

	index = 0;
	i = 0;
	cmd_size(child);
	temp = (char **)ft_calloc(sizeof(char *), child->size[1] + 1);
	while (index < child->size[2] && child->info[index])
	{
		if (ft_strchr(child->info[index], '\''))
			temp[i++] = ft_strtrim(child->info[index], "\'");
		else if (ft_strchr(child->info[index], '$'))
				temp[i++] = expand_var(p, child, index);
		else if (child->info[index] && *child->info[index])
			temp[i++] = ft_strdup(child->info[index]);
		index++;
	}
	if (child->echo)
		temp = ft_realloc_child(temp);
	free_d2(child->info);
	child->info = temp;
}
