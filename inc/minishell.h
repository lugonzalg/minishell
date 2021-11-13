/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:28:19 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/13 19:32:30 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H  

# define OUTPUT '>'
# define INPUT	'<'
# define NL		"\n"

# include <stdbool.h>
# include <unistd.h>

typedef struct s_prompt
{
	char				**path;
	char				**d2_prompt;
	char				*user;
	char				*prompt;
	char				*tmp;
	char				*envpath;
	pid_t				*id;
	struct s_builtin	*head;
}	t_prompt;

typedef struct s_child
{
	char		**info;
	int			**fdpipe;
	char		*path;
	char		*ttypath;
	size_t		size[4]; // [0] -> pipes // [1] -> new_info // [2] -> info // [3] -> cmd END
	bool		redir[2]; // [0] -> inredir // [1] -> outredir
	size_t		id;
	int			tty;
}	t_child;

typedef void	(*t_no_ch)(t_prompt *p);
typedef void	(*t_no_p)(t_child *child);
typedef void	(*t_ptr)(t_prompt *, t_child *);

typedef union u_ptrs
{
	t_ptr				b1;
	t_no_ch				b2;
	t_no_p				b3;
}	t_ptrs;

typedef struct s_builtin
{
	t_ptrs				*ptr;
	char				*key;
	struct s_builtin	*next;
}	t_builtin;

void		check_redir(t_prompt *p, t_child *child);
extern int	command_pos(t_prompt *p, t_child *child);
extern void	set_child(t_prompt *p, t_child *child);
extern void	prompt_io(t_prompt *p);
extern void	print_intro(void);
extern void	set_prompt(t_prompt *p);

extern void	unify_fdio(t_child *child);
extern void	unify_cmd(t_child *child);

extern int	ft_checkbuiltins(char *p);
extern void	ft_builtins(t_child *child, t_prompt *p);
extern void	ft_putenv(char **env, t_prompt *p);
extern void	free_d2(char **dat);
extern void	free_p(t_prompt *p);
extern void	free_child(t_child *child);

extern void ft_echo(t_prompt *p, t_child *child);
extern void ft_exit(t_prompt *p, t_child *child);
extern void ft_cd(t_prompt *p, t_child *child);
extern void ft_pwd(t_prompt *p, t_child *child);
extern void ft_export(t_prompt *p, t_child *child);
extern void ft_unset(t_prompt *p, t_child *child);
extern void ft_env(t_prompt *p);
#endif
