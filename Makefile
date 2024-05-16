# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rogalio <rmouchel@student.42.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 15:23:18 by rogalio           #+#    #+#              #
#    Updated: 2024/05/16 15:07:25 by rogalio          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
THREADS = -pthread

NAME = philo
SRCS = src/main.c src/parsing.c src/monitor.c src/philosophers.c src/utils.c src/threads.c
OBJDIR = obj/
OBJS = $(SRCS:src/%.c=$(OBJDIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(THREADS) $(OBJS) -o $(NAME)

$(OBJDIR)%.o: src/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
