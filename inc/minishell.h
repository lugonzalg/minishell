/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:28:19 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/03 15:10:38 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H  

# define OUTPUT '>'
# define INPUT '<'

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
	int		 	**fdpipe;
	char		*path;
	short int	redir[2];
	short int	id;
	short int	num;
	short int	pos;
	short int	size;
}	t_child;

void 		check_redir(t_string *str, t_child *child);
extern int	command_pos(t_string *str, t_child *child);
extern void	set_child(t_child *child, t_string *str);
extern void	prompt_io(t_string *str);
extern void	print_intro(void);
extern void	set_str(t_string *str);

#endif
