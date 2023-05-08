/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:57:19 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/07 21:16:55 by rkurnava         ###   ########.fr       */
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
	int				fork_avail;
	int				slept;
	int				think;
}					t_philosph;

typedef struct s_stats
{
	long long		pos;
	long long		nb_philosoph;
	long long		time_to_eat;
	long long		time_to_die;
	long long		time_to_sleep;
	long long		to_eat;
	int				death;
	t_philosph		*philo;
	pthread_mutex_t	eat;
	pthread_mutex_t	dead;
	pthread_mutex_t	print;
	pthread_mutex_t	pause;
}					t_stats;
void				ft_printer(char *message, t_stats *stats, long long pas);
int					ft_atoi(const char *str);
char				*ft_itoa(int n);
long long			ft_timestamp(void);
int					philo_die(t_stats *stats, long long pas);
int				wait_time(long long waiting, t_stats *stats, long long pas);
void				ft_commander(t_stats *stat, long long pas);

#endif