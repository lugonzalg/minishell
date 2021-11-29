/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:28:19 by lugonzal          #+#    #+#             */
/*   Updated: 2021/11/28 21:18:16 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H  

# define OUTPUT 	'>'
# define INPUT		'<'
# define NL			"\n"
# define S_QUOTE	"\'"
# define D_QUOTE	"\""
# define PATH_MAX	4096

# include <stdbool.h>
# include <unistd.h>

typedef struct s_prompt
{
	char		**path;
	char		**d2_prompt;
	char		*user;
	char		*prompt;
	char		*tmp;
	char		*envpath;
	char		*builtpath;
	pid_t		*id;
	int			sizenv;
	size_t		on;
}	t_prompt;

typedef struct s_child
{
	char		**info;
	int			**fdpipe;
	char		*path;
	char		*ttypath;
	size_t		size[4]; // [0] -> pipes // [1] -> new_info // [2] -> info // [3] -> cmd END
	bool		redir[3]; // [0] -> inredir // [1] -> outredir // here_doc
	bool		builtin;
	size_t		id;
	int			tty;
	bool		echo;
}	t_child;

typedef struct	s_global
{
	int		error;
	pid_t	killid;
}	t_global;

typedef size_t	(*t_len)(char *, char);

static t_global	g_glob;

extern void			check_redir(t_prompt *p, t_child *child);
extern void			command_pos(t_prompt *p, t_child *child);

extern void			prompt_io(t_prompt *p);
extern void			print_intro(void);

extern void			set_child(t_prompt *p, t_child *child);
extern void			set_str(t_prompt *p);

extern int			go_exit(int num);
extern void			sig_handler(int signo);

extern void			unify_fdio(t_child *child);
extern void			unify_cmd(t_prompt *p, t_child *child);

extern void			free_d2(char **dat);
extern void			free_p(t_prompt *p);
extern void			free_child(t_child *child);

extern char			**ft_split_ptr(const char *s, char c, t_len len, t_prompt *p);
extern char			*ft_cut(char *s, char **s_ptr, char c, t_prompt *p);
extern size_t		ft_lenp(char *s, char c);
extern size_t		ft_len_redir(char *s, char c);
extern void			ft_quote(char *s, char **n_str);
extern char			*ft_find_quote(const char *s);

extern int			ft_checkbuiltins(char *str, t_prompt *p);
extern void			ft_builtins(t_child *child, t_prompt *p);

extern void			ft_putenv(char **env, t_prompt *p);
extern void			ft_echo(t_child *child);
extern void			deletenv(t_child	*child, t_prompt *p);
extern void			envinclude(t_child	*child, t_prompt *p);
extern void			showenv(t_prompt *p);
extern char			*expand_var(t_prompt *p, t_child *child, size_t i);
extern char			**ft_realloc_child(char **temp);

extern char			**ft_setpath(t_prompt *p);
extern char			*ft_gnl_query(char *path, char *query);
#endif
