/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unify.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 21:18:51 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/22 21:32:36 by lugonzal         ###   ########.fr       */
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

	resize = (char **)malloc(sizeof(char *) * 3);
	resize[0] = ft_strdup(child->info[0]);
	resize[1] = ft_strdup(".here_doc");
	resize[2] = NULL;
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
//ADDED BY MERGE
/*
static int trim_redir(char *str, char key, size_t count)
{
	char	*data;
	char	*redir;
	size_t	start;
	size_t	len;
	(void)key;

	data = NULL;
	redir = NULL;
	len = -1;
	if (ft_strchr(str, '\"'))
	{
		start = ft_strchr(str, '\"') - str;
		len =  ft_strrchr(str, '\"') - ft_strchr(str, '\"');
		data = ft_substr(str + 1, start, len);
		redir = ft_substr(str, len, 2048);
		if (count == 1)
			len = open(redir, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		else
			(void)here_doc;
			len = open(redir, O_WRONLY | O_APPEND | O_CREAT, 0644);
	}
	free(str);
	free(redir);
	str = data;
	return (len);
}

static size_t	count_char(char *str, char key)
{
	size_t	count;

	count = 0;
	while (*str)
	{
		if (*str == key)
			count++;
		str++;
	}
	return (count);
}

static int	isolate_fdio(char *str, char redir)
{
	//char	*redir_pos;
	size_t	count;
	size_t	pos;

	count = count_char(str, redir);
	if (count > 2)
		return (-1);
	pos = ft_strrchr(str, redir) - ft_strchr(str, redir);
	return (trim_redir(str, redir, count));
}
*/
///////////////////////////////////
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
				fd = isolate_fdio(child->info[i++], OUTPUT);
			close(child->fdpipe[child->id + 1][1]);
			child->fdpipe[child->id + 1][1] = fd;
		}
		if (ft_strchr(child->info[i], INPUT))
		{
			if (ft_strlen(child->info[i]) == 1)
				fd = open(child->info[++i], O_RDONLY);
			else
				here_doc(child, child->info[i + 1]);
				//fd = isolate_fdio(child->info[i++], INPUT);
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

void	unify_cmd(t_prompt *p, t_child *child)
{
	char	**temp;
	size_t	index;
	size_t	i;

	index = 0;
	i = 0;
	cmd_size(child);
	temp = (char **)ft_calloc(sizeof(char *), child->size[1] + 1);
	while (index < child->size[2])
	{
		if (ft_strchr(child->info[index], '\''))
			temp[i++] = ft_strtrim(child->info[index], "\'");
		else if (ft_strchr(child->info[index], '$'))
				temp[i++] = expand_var(p, child, index);
		else if (child->info[index] && *child->info[index])
			temp[i++] = ft_strdup(child->info[index]);
		index++;
	}
	free_d2(child->info);
	child->info = temp;
}


			/*else if (ft_strlen(child->info[i]) == 2)
				fd = open(child->info[++i], O_RDWR | O_APPEND | O_CREAT, 0644);
			else
				exit(0); //SET FAILURE*/


			/*else if (ft_strlen(child->info[i]) == 2)
			{
				here_doc(child, child->info[i + 1]);
				continue ;
			}
			else
				exit(0); //SET FAILURE*/
