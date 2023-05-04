# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/02 14:03:41 by rkurnava          #+#    #+#              #
#    Updated: 2023/05/04 11:08:53 by rkurnava         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	philo

CC = cc

CFLAGS =	-Wall -Werror -Wextra

SRC = main.c eat_sleep_think.c utils.c\

all: $(NAME)

OBJ  =	$(SRC:.c=.o)

$(NAME) : $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(NAME)
	@echo "$(NAME) created"

%.o : %.c
	@$(CC) $(CFLAGS) -c -g -o $@ $<

clean :
	@rm -f $(OBJ)
	@echo "Done cleaning"

fclean : clean
	@rm -f philo
	@echo "Everything cleaned"

re : fclean all

.PHONY:	all clean fclean re
