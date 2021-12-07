/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unify.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 21:18:51 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/07 19:46:57 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
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

char	*ft_puterror(t_child *child, size_t index)
{
	if (ft_strnstr(child->info[index], "$?", 3))
		return (ft_itoa(g_glob.error));
	return (NULL);
}

void	unify_cmd(t_prompt *p, t_child *child)
{
	char	**temp;
	size_t	index;
	size_t	i;

	(void)p;
	index = 0;
	i = 0;
	cmd_size(child);
	temp = (char **)ft_calloc(sizeof(char *), child->size[1] + 1);
	while (index < child->size[2] && child->info[index])
	{
		if (child->info[index] && *child->info[index])
			temp[i++] = ft_strdup(child->info[index]);
		index++;
	}
	if (child->echo)
		temp = ft_realloc_child(temp);
	free_d2(child->info);
	child->info = temp;
}
