/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:12:56 by rkurnava          #+#    #+#             */
/*   Updated: 2023/04/27 18:28:38 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	timestamp(void)
{
	struct timeval	tv;
	long long		timestamp;

	gettimeofday(&tv, NULL);
	timestamp = (((tv.tv_sec * 1000000) + tv.tv_usec) / 1000);
	return (timestamp);
}

void	wait_time(long long waiting)
{
	long long	timestamp_begin;
	long long	timestamp_end;

	timestamp_begin = timestamp();
	timestamp_end = timestamp_begin;
	while ((timestamp_end - timestamp_begin) < waiting)
		timestamp_end = timestamp();
	return ;
}

int	ft_phil_init(int argc, char **argv, t_stats *stats)
{
	long	pos;

	pos = 0;
	stats->nb_philosoph = ft_atoi(argv[1]);
	stats->time_to_die = ft_atoi(argv[2]);
	stats->time_to_eat = ft_atoi(argv[3]);
	stats->time_to_sleep = ft_atoi(argv[4]);
	stats->to_eat = 0;
	stats->pos = -1;
	if (argc == 6)
		stats->to_eat = ft_atoi(argv[5]);
	stats->philo = malloc(sizeof(t_philosph) * stats->nb_philosoph);
	if (!stats->philo)
		return (write(1, "Could not allocate!\n", 20) && 1);
	while (pos < stats->nb_philosoph)
	{
		stats->philo[pos].position = pos;
		stats->philo[pos].fork = 1;
		stats->philo[pos].nb_ate = 0;
		stats->philo[pos].last_ate = 0;
		stats->philo[pos].start_time = 0;
		pos += 1;
	}
	return (0);
}

void	philo_eat(t_stats *stats)
{
	stats->philo[stats->pos].fork = 0;
	stats->philo[stats->pos + 1].fork = 0;
	stats->philo[stats->pos].taken_fork = timestamp();
	printf("%lli Philo %lli has taken fork\n",
			stats->philo[stats->pos].taken_fork
				- stats->philo[stats->pos].start_time,
			stats->pos);
	stats->philo[stats->pos].last_ate = timestamp();
	printf("%lli Philo %lli is eating\n", stats->philo[stats->pos].last_ate
			- stats->philo[stats->pos].start_time, stats->pos);
	stats->philo[stats->pos].nb_ate += 1;
	pthread_mutex_unlock(&stats->mutex);
}

void	philo_think(t_stats *stats)
{
	stats->philo[stats->pos].timestamp = timestamp();
	printf("%lli Philo %lli is thinking\n", stats->philo[stats->pos].timestamp
			- stats->philo[stats->pos].start_time, stats->pos);
}

void	*philo_start(void *philo)
{
	struct timeval	tv;
	t_stats			*philos;
	long long		timestamp;
	long long		pas;

	philos = (struct s_stats *)philo;
	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&philos->mutex);
	philos->pos += 1;
	pas = philos->pos;
	timestamp = (((tv.tv_sec * 1000000) + tv.tv_usec));
	philos->philo[pas].start_time = timestamp / 1000;
	if (pas + 1 < philos->nb_philosoph && philos->philo[pas].fork == 1
		&& philos->philo[pas + 1].fork == 1)
	{
		philo_eat(philos);
		wait_time(philos->time_to_eat);
		philos->philo[pas].fork = 1;
		philos->philo[pas + 1].fork = 1;
	}
	else
	{
		philo_think(philos);
		wait_time(philos->time_to_sleep);
		pthread_mutex_unlock(&philos->mutex);
	}
	return (NULL);
}

void	ft_start(t_stats *stats)
{
	pthread_t	philo[stats->nb_philosoph];
	long		pos;

	pos = 0;
	while (pos < stats->nb_philosoph)
	{
		if (pthread_create(&philo[pos], NULL, philo_start, stats) != 0)
		{
			write(1, "Thread creation error!\n", 24);
			return ;
		}
		pos++;
	}
	pos = 0;
	while (pos < stats->nb_philosoph)
	{
		if (pthread_join(philo[pos], NULL) != 0)
		{
			write(1, "Thread joining error!\n", 22);
			return ;
		}
		pos++;
	}
}

int	main(int argc, char **argv)
{
	t_stats *stats;

	if (argc < 5 || argc > 6)
	{
		write(1, "Wrong number of arguments\n", 26);
		return (0);
	}
	stats = malloc(sizeof(t_stats));
	if (!stats)
		return (write(1, "Could not allocate!\n", 20) && 0);
	if (ft_phil_init(argc, argv, stats) == 1)
	{
		free(stats);
		return (0);
	}
	pthread_mutex_init(&stats->mutex, NULL);
	ft_start(stats);
	pthread_mutex_destroy(&stats->mutex);
	if (stats->nb_philosoph < 1 || stats->time_to_die < 0
		|| stats->time_to_eat < 0 || stats->time_to_sleep < 0
		|| stats->to_eat < 0)
	{
		free(stats->philo);
		free(stats);
		pthread_mutex_destroy(&stats->mutex);
		return (0);
	}
	pthread_mutex_destroy(&stats->mutex);
	free(stats->philo);
	free(stats);
	return (0);
}