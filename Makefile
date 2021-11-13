NAME = minishell

MK_DIR = mkdir -p
RM = rm -rf
CC = clang
INC = -I /usr/include/readline \
	  -I /Users/$(USER)/.brew/opt/readline/include \
	  -I ${PWD} \
	  -I ${PWD}/src/libft \
	  -I ${PWD}/src/ft_printf \

CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address 
LFLAGS = -L //Users/$(USER)/.brew/opt/readline/lib \
		 -L usr/include/readline \
		 -L bin/ -lreadline -lft -lftprintf
BIN = libft.a libftprintf.a minishell

FILES = minishell/main \
		minishell/print_intro \
		minishell/prompt \
		minishell/set_data \
		minishell/unify		\
		minishell/builtins

SRC_DIR = src/
OBJ_DIR = obj/
SRC = $(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJ = $(addprefix $(SRC_DIR), $(addsuffix .o, $(FILES)))

.c.o:
	@echo "\033[1;34m\tCompiling source files into object files\033[0;m"
	${CC} ${CFLAGS} ${INC} -c $^ -o $@

$(NAME): ${OBJ}
	@${SHELL} ./src/script/file_manager.sh
	${CC} ${CFLAGS} -o ${NAME} ${OBJ} ${LFLAGS}

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

re: fclean all

.PHONY: all clean fclean re
