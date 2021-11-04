/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:28:19 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/04 20:40:29 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H  

# define OUTPUT '>'
# define INPUT	'<'
# define NL		"\n"

typedef struct	s_string
{
	char		**path;
	char		**d2_prompt;
	char		*user;
	char		*prompt;
	char		*tmp;
}	t_string;

typedef struct	s_child
{
	char		**info;
	int		 	**fdpipe;
	int		 	*fdout;
	char		*path;
	short int	size[4]; // [0] -> pipes // [1] -> fdout // [2] -> cmd START // [3] -> cmd END
	short int	redir[2]; // [0] -> inredir // [1] -> outredir
	short int	id;
}	t_child;

void 		check_redir(t_string *str, t_child *child);
extern int	command_pos(t_string *str, t_child *child);
extern void	set_child(t_string *str, t_child *child);
extern void	prompt_io(t_string *str);
extern void	print_intro(void);
extern void	set_str(t_string *str);

#endif
