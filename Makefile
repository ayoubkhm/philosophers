# Makefile

NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = main.c init.c philosopher.c utils.c monitor.c parsing.c destroy.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lpthread

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all