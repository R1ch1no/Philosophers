/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iengels <iengels@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:50:22 by iengels           #+#    #+#             */
/*   Updated: 2023/05/05 17:18:49 by iengels          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

/*
    data structure
*/

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				eat_count;
	int				is_eating;
	long long		ate_last;
	pthread_t		thread_id;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				n_philos;
	int				t_die;
	int				t_eat;
	int				t_to_sleep;
	int				n_times_to_eat;
	int				dead;
	int				full;
	int				error;
	long long		start_time;
	pthread_mutex_t	m_check;
	pthread_mutex_t	m_print;
	pthread_mutex_t	*m_fork;
	t_philo			philos[300];
}					t_data;

/*
    ft_init.c
*/

int					ft_init_philo(t_data *d);
int					ft_init_data(t_data *d, int ac, char **av);
int					ft_start_threads(t_data *d);
void				ft_join_and_destroy(t_data *d);
int					ft_init_mutex(t_data *d);

/*
    ft_routine.c
*/

void				*ft_routine(void *philo);
void				ft_take_forks(t_philo *p);
void				ft_eat(t_philo *p);
void				ft_feast_or_famine(t_data *d);

/*
    ft_utils.c
*/

int					ft_strcmp(char *s1, char *s2);
int					ft_strlen(char *str);
int					ft_isspace(char c);
int					ft_atoi(const char *str);
int					ft_isdigit(char c);

/*
	ft_time.c
*/

int					ft_all_ate(t_data *d, int i);
long long			ft_timestamp(void);
void				ft_usleep(long long time);
void				ft_sleep(t_philo *p);

/*
    ft_writing.c
*/

void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_print(t_philo *p, char *str);

/*
    ft_errorchecks.c
*/

int					ft_input_limits(t_data *d, int ac, char **av);
int					ft_is_digitstr(char *str);
int					ft_int_limit_check(char *str);
int					ft_error_cleanup(t_data *d, int i, int j);
void				*ft_one_philo(void *philo);

#endif