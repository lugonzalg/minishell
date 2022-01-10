NAME =		minishell

MK_DIR = 	mkdir -p
RM = 		rm -rf
CC = 		clang
INC = 		-I /usr/include/readline \
	  		-I /Users/$(USER)/.brew/opt/readline/include \
	  		-I ${PWD} \
	  		-I ${PWD}/src/libft \
	  		-I ${PWD}/src/ft_printf \

CFLAGS = 	-Wall -Werror -Wextra -g3 -fsanitize=address 
LIB = 		-L /Users/$(USER)/.brew/opt/readline/lib \
		 	-L usr/include/readline \
		 	-L bin/ -lreadline -lft -lftprintf -ltermcap

BIN = 		libft.a libftprintf.a minishell

FILES = 	minishell/main \
			minishell/error/ft_error_check \
			minishell/error/ft_fd_error \
			minishell/prompt/prompt \
			minishell/prompt/prompt_error \
			minishell/prompt/pipe \
			minishell/set_data \
			minishell/unify_paths/unify	\
			minishell/unify_paths/unify_fd \
			minishell/unify_paths/command_path \
			minishell/builtins/builtins \
			minishell/builtins/handle_pwd \
			minishell/builtins/builtins_utils \
			minishell/builtins/handle_builtins \
			minishell/split/split_ptr \
			minishell/split/split_tools \
			minishell/quote_expand/ft_expand \
			minishell/quote_expand/quote \
			minishell/wildcard/handlewc \
			minishell/wildcard/expandwc \
			minishell/wildcard/dirinfowc \
			minishell/wildcard/extrawc \
			minishell/wildcard/utilswc 

SRC_DIR = 	src/
OBJ_DIR = 	obj/
SRC = 		$(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJ = 		$(addprefix $(SRC_DIR), $(addsuffix .o, $(FILES)))

.c.o:
	@echo "\033[1;34m\tCompiling source files into object files\033[0;m"
	${CC} ${CFLAGS} ${INC} -c $^ -o $@

$(NAME): ${OBJ}
	@${SHELL} ./src/script/file_manager.sh
	${CC} ${CFLAGS} -o ${NAME} ${OBJ} ${LIB}

all: $(NAME)

clean:
	@${MAKE} -C src/libft clean
	@${MAKE} -C src/ft_printf clean
	${RM} bin
	${RM} ${OBJ}
	${RM} ${OBJ_DIR}

fclean: clean
	@${MAKE} -C src/libft fclean
	@${MAKE} -C src/ft_printf fclean
	${RM} bin
	${RM} ${BIN}
	${RM} minishell.dSYM

re: fclean all

.PHONY: all clean fclean re
