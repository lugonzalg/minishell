/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 13:31:05 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/22 13:31:07 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if (c >= 65 && c <= 122)
	{	
		if (c > 90 && c < 97)
			return (0);
		return (1);
	}
	return (0);
}
