/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:28:19 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/09 19:57:37 by mikgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H  

# define OUTPUT '>'
# define INPUT	'<'
# define NL		"\n"

#include <stdbool.h>

typedef struct	s_string
{
	char		**path;
	char		**d2_prompt;
	char		*user;
	char		*prompt;
	char		*tmp;
	char		*envpath;
}	t_string;

typedef struct	s_child
{
	char		**info;
	int	 	**fdpipe;
	char		*path;
	size_t		size[4]; // [0] -> pipes // [1] -> new_info // [2] -> info // [3] -> cmd END
	bool		redir[2]; // [0] -> inredir // [1] -> outredir
	size_t		id;
	int		tty;
	char		*ttypath;
}	t_child;

void 		check_redir(t_string *str, t_child *child);
extern int	command_pos(t_string *str, t_child *child);
extern void	set_child(t_string *str, t_child *child);
extern void	prompt_io(t_string *str);
extern void	print_intro(void);
extern void	set_str(t_string *str);

extern void	unify_fdio(t_child *child);
extern void	unify_cmd(t_child *child);

int		ft_checkbuiltins(char *str);
void	ft_builtins(t_child *child, t_string *str);
void	ft_putenv(char **env, t_string *str);

#endif
