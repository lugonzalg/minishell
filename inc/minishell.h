/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:28:19 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/02 18:42:21 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H  

typedef struct	s_string
{
	char	**path;
	char	**d2_prompt;
	char	*user;
	char	*prompt;
	char	*tmp;
	char	*trim_file;
}	t_string;

typedef struct	s_child
{
	char		**info;
	int			**fd;
	char		*path;
	short int	num;
}	t_child;

extern void	print_intro(void);
extern void	set_str(t_string *str);

#endif
