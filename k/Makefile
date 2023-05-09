# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iengels <iengels@student.42vienna.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 16:15:05 by iengels           #+#    #+#              #
#    Updated: 2023/05/05 01:07:17 by iengels          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC =	main.c \
		ft_init.c \
		ft_routine.c	\
		ft_utils.c \
		ft_writing.c	\
		ft_errorchecks.c	\
		ft_time.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): 
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:	
		rm -f *.o

fclean:	clean 
		rm -f philo

re: fclean all

.PHONY: all clean fclean re