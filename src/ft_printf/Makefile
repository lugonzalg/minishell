NAME =		libftprintf.a
CC = 		clang
CFLAGS = 	-Wall -Werror -Wextra -g
RM = 		rm -rf
INC =		-I ./inc -I ${PWD}/src/libft/inc
AR = 		ar rcs

FILES = ft_printf \
		handle_types \
		writf

SRC_DIR = src/
SRCS = $(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJS = $(addprefix $(SRC_DIR), $(addsuffix .o, $(FILES)))

.c.o: $(SRCS) 
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $@ $^
	ranlib $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
