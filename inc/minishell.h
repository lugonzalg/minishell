/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:28:19 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/22 20:56:52 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H  

# define OUTPUT '>'
# define INPUT	'<'
# define NL		"\n"

#include <stdbool.h>
#include <unistd.h>


typedef struct	s_prompt
{
	char		**path;
	char		**d2_prompt;
	char		*user;
	char		*prompt;
	char		*tmp;
	char		*envpath;
	char		*builtpath;
	pid_t		*id;
}	t_prompt;

typedef struct	s_child
{
	char		**info;
	int	 		**fdpipe;
	char		*path;
	char		*ttypath;
	size_t		size[4]; // [0] -> pipes // [1] -> new_info // [2] -> info // [3] -> cmd END
	bool		redir[3]; // [0] -> inredir // [1] -> outredir // here_doc
	bool		builtin;
	size_t		id;
	int			tty;
}	t_child;

typedef struct	s_global
{
	int		error;
	pid_t	killid;
}	t_global;

static t_global	g_glob;

void 		check_redir(t_prompt *p, t_child *child);
extern int	command_pos(t_prompt *p, t_child *child);
extern void	set_child(t_prompt *p, t_child *child);
extern void	prompt_io(t_prompt *p);
extern void	print_intro(void);
extern void	set_str(t_prompt *p);
int	go_exit(int num);

extern void	unify_fdio(t_child *child);
extern void	unify_cmd(t_prompt *p, t_child *child);

int		ft_checkbuiltins(char *str, t_prompt *p);
void	ft_builtins(t_child *child, t_prompt *p);
void	ft_putenv(char **env, t_prompt *p);
extern void	free_d2(char **dat);
extern void	free_p(t_prompt *p);
extern void	free_child(t_child *child);

extern void	driver_talk(void);
#endif
