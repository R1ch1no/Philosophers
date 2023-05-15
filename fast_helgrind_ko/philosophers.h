/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:57:19 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/15 15:51:19 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philosoph
{
	long long		position;
	long long		start_time;
	long long		timestamp;
	long long		taken_fork;
	long long		nb_ate;
	long long		last_ate;
	int				left_fork;
	int				right_fork;
	struct s_stats	*rules;
}					t_philosph;

typedef struct s_stats
{
	long long		nb_philosoph;
	long long		time_to_eat;
	long long		time_to_die;
	long long		time_to_sleep;
	long long		to_eat;
	long long		done_eat;
	long long		all_ate;
	int				death;
	t_philosph		*philo;
	pthread_mutex_t	*eat;
	pthread_mutex_t	wait;
	pthread_mutex_t	count;
	pthread_mutex_t	dead;
	pthread_mutex_t	print;
}					t_stats;

char				*ft_itoa(int n);
void				phil_init_two(t_stats *stats);
void				sleep_think(t_philosph *philo);
void				ft_printer(char *message, t_stats *stats, long long pos);
void				philo_die(t_stats *stats);
void				*ft_commander(void *philo);
void				ft_unfork(t_philosph *philo);
void				ft_usleep(long long waiting, t_philosph *philo);
int					ft_mutex_init(t_stats *stats);
int					ft_stop(t_philosph *philo);
int					ft_check_params(int argc, char **argv);
int					ft_mutex_init(t_stats *stats);
int					check_nums(char **argv);
int					ft_atoi(const char *str);
int					is_dead(t_philosph *philo);
int					ft_start(t_stats *stats);
int					ft_done_eating(t_stats *stats, long long pos);
int					mutex_init_part_two(t_stats *stats);
int					ft_will_die(t_stats *stats, long long pos);
int					mutex_destroy_join(long pos, t_stats *stats,
						pthread_t *philo);
long long			ft_timestamp(void);

#endif