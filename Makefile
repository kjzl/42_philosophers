NAME = philo
CFLAGS = -Wall -Wextra -Werror -g3
# CFLAGS = -Wall -Wextra -Werror -g -Og
# CFLAGS = -g -Og

SRCS = \
	main.c\
	philo.c\
	philo_util.c\
	log.c\
	util.c\
	academy.c\
	strsl_atoi.c\
    strsl_chr.c\
    strsl_move.c\

OBJ = $(SRCS:.c=.o)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -lpthread -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
