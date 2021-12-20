/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lugonzal <lugonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:28:19 by lugonzal          #+#    #+#             */
/*   Updated: 2021/12/17 18:33:42 by lugonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H  

# define OUTPUT 	'>'
# define INPUT		'<'
# define NL			'\n'
# define S_QUOTE	'\''
# define D_QUOTE	'\"'
# define PIPE		'|'
# define AND		'&'
# define DOLLAR		'$'
# define SPACE		' '
# define PATH_MAX	4096

# include <stdbool.h>
# include <unistd.h>

typedef struct s_prompt
{
	char		**path;
	char		*home;
	char		**d2_prompt;
	char		*user;
	char		*prompt;
	char		*tmp;
	char		*envpath;
	char		*builtpath;
	pid_t		*id;
	int			sizenv;
	char		*temppath;
}	t_prompt;

typedef struct s_child
{
	char		**info;
	int			**fdpipe;
	char		*path;
	char		*ttypath;
	size_t		size[4];
	bool		redir[3];
	bool		builtin;
	size_t		id;
	int			tty;
	bool		echo;
}	t_child;

typedef size_t		(*t_len)(char *, char);

typedef struct s_global
{
	int		error;
	int		here_doc;
	pid_t	killid;
}	t_global;

t_global			g_glob;

/*PROMPT*/
extern void			ft_prompt_io(t_prompt *p);
extern void			ft_check_redir(t_prompt *p, t_child *child);

/*SIGNAL & ERROR*/
extern int			ft_go_exit(int num);
extern void			ft_sig_handler(int signo);
extern int			ft_fdcheck(t_child *child, char redir);
extern bool			ft_check_prompt(t_prompt *p);
extern char			*ft_trim_error(char *info, char redir, int on);

/*UNIFY*/
extern void			ft_unify_fdio(t_child *child);
extern void			ft_unify_cmd(t_prompt *p, t_child *child);
extern int			ft_putpath(t_child *child);
extern char			**ft_setpath(t_prompt *p);

/*SET DATA*/
extern void			ft_set_child(t_prompt *p, t_child *child);
extern void			ft_restart_data(t_child *child);
extern void			ft_free_d2(char **dat);
extern void			ft_free_p(t_prompt *p);
extern void			ft_free_child(t_child *child);

/*PROMPT*/
extern char			**ft_split_ptr(const char *s, char c, t_len len);
extern size_t		ft_lenp(char *s, char c);
extern size_t		ft_len_redir(char *s, char c);
extern char			*ft_cut(char *s, char **s_ptr, char c);
extern void			ft_process_io(t_prompt *p);

/*BUILTINS*/
extern int			ft_checkbuiltins(char *str, t_prompt *p);
extern void			ft_builtins(t_child *child, t_prompt *p);
/***********************ENV************************************/
extern void			ft_showenv(t_prompt *p);
/***********************EXPORT*********************************/
extern void			ft_envinclude(t_child	*child, t_prompt *p);
extern int			ft_check_env(t_child *child, char *line, t_prompt *p);
extern void			ft_deletenv(t_child	*child, t_prompt *p);
extern void			ft_deletenv_2(t_prompt *p);
/*********************ECHO*************************************/
extern void			ft_echo(t_child *child);
/*********************CHANGE_DIR*******************************/
extern int			ft_changedir(t_child *child, t_prompt *p);
extern int			ft_changediraux(t_child *child, t_prompt *p);
/************************PWD***********************************/
extern void			ft_putpwd(char *str, t_prompt *p);
extern char			*ft_search_pwd(t_prompt *p);
extern void			ft_changepwd(char *str, t_prompt *p);
extern void			ft_deletpwd(char *str, t_prompt *p);
extern void			ft_env_fail(t_child *child);

/*EXPAND & QUOTE*/
extern void			ft_expand(t_prompt *p, t_child *child);
extern int			ft_quote_error(char *str);

/*TOOLS*/
extern char			*ft_gnl_query(char *path, char *query);
extern size_t		ft_query_len(char *s, char c);
#endif
