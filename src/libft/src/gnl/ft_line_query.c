/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_query.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 19:50:55 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/29 19:52:24 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/libft.h"

char	*ft_line_query(int fd)
{
	char	*line;
	char	*buf;
	ssize_t	size;

	if (fd < 0 || fd == 2)
		return (NULL);
	size = 1;
	while (size && ft_str
}
