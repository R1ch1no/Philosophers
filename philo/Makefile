# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/02 14:03:41 by rkurnava          #+#    #+#              #
#    Updated: 2023/10/20 13:13:55 by rkurnava         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	philo

CC = cc

CFLAGS =	-Wall -Werror -Wextra #-fsanitize=thread #testingflag

INCLUDES = ./philosophers.h

SRC =	main.c eat_sleep_think.c utils_one.c utils_two.c one_philo.c \

all: $(NAME)

OBJ  =	$(SRC:.c=.o)

$(OBJ) : $(INCLUDES)

$(NAME) : $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(NAME)
	@echo "$(NAME) created"

clean :
	@rm -f $(OBJ)
	@echo "Done cleaning"

fclean : clean
	@rm -f philo
	@echo "Everything cleaned"

re : fclean all

.PHONY:	all clean fclean re
